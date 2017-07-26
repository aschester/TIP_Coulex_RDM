#include "Results.hh"

Results::Results(Projectile* proj,Recoil* rec,DetectorConstruction* det):theProjectile(proj),theRecoil(rec),theDetector(det)
{
  h=NULL;
  g=NULL;  
  c=NULL;
  TreeCreate();  
  IonFill=0;
  soh=sizeof(rHit);
  sogh=sizeof(GHit);
  soi=sizeof(gun);
  sos=sizeof(stat);
  sor=sizeof(react);
  memset(CP,0,sizeof(CP));

  // HPGe FWHM response parameters
  // These are measured from runs 36140/36141 152Eu calibration data
  // F=2.27558;
  // G=3.78329;
  // H=0.66543;

  // No FWHM response
  F=0.;
  G=0.;
  H=0.;

  // Energy calibration parameters
  A=0.;
  B=1.;
  C=0.;  
}

Results::~Results()
{
tree->Delete();
}
//---------------------------------------------------------
void Results::SetupRun()
{
  int i,j;

  Ap=theProjectile->getA();
  Zp=theProjectile->getZ();
  Ar=theRecoil->getA();
  Zr=theRecoil->getZ();

  dDensity=theDetector->GetPlunger()->GetStopperDensity();
  CsIDensity=theDetector->GetCsIArray()->GetCsIDetector()->GetCsIDensity();
  // printf("stopper density is %9.3f g/cm^3\n",dDensity);
  // printf("CsI density is     %9.3f g/cm^3\n",CsIDensity);
  // getc(stdin);

// setup Birks constant for LY calculation and get CsI positions
  for(i=0;i<NCsI;i++)
    {
      kB[i]=theDetector->GetCsIArray()->GetBirksConstant(i); // Birks constant in um/MeV
      kBm[i]=kB[i]*CsIDensity/10.;                           // Birks constant in (mg/cm^2)/MeV
      S[i]=theDetector->GetCsIArray()->GetLYScaling(i);
      // printf("ipos %2d index %2d kB %.4f kBm %.7f S %.3f\n",i+1,i,kB[i],kBm[i],S[i]);
      PP[i]=theDetector->GetCsIArray()->GetCsIDetectorPosition(i);
      //printf("CsI detector %2d positions in cm: x %5.1f y %5.1f z %5.1f\n",i+1,PP[i].getX(),PP[i].getY(),PP[i].getZ());
    }

  // get HPGe crystal positions
  for(i=0;i<GN;i++)
    for(j=0;j<GS;j++)
      {
	CP[i][j]=theDetector->GetDetectorCrystalPosition(i,j);
	//printf("HPGe position %d crystal %d x %f y %f z %f\n",i,j,CP[i][j].getX(),CP[i][j].getY(),CP[i][j].getZ());
	//printf("%d %d %f %f %f\n",i+1,j,CP[i][j].getX(),CP[i][j].getY(),CP[i][j].getZ());
      }
  //getc(stdin);
}
//---------------------------------------------------------
void Results::TreeCreate()
{
 if(tree==NULL)
    {
      tree=new TTree("tree","tree");

      // gamma info
      tree->Branch("Gfold",&GHit.Gfold,"Gfold/I");
      tree->Branch("GId",GHit.GId,"GId[Gfold]/I");
      tree->Branch("GSeg",GHit.GSeg,"GSeg[Gfold]/I");
      tree->Branch("GRing",GHit.GRing,"GRing[Gfold]/I");
      tree->Branch("Gx",GHit.Gx,"Gx[Gfold]/D");
      tree->Branch("Gy",GHit.Gy,"Gy[Gfold]/D");
      tree->Branch("Gz",GHit.Gz,"Gz[Gfold]/D");
      tree->Branch("GE",GHit.GE,"GE[Gfold]/D");
      tree->Branch("GW",GHit.GW,"GW[Gfold]/D");
      tree->Branch("GfoldAddBack",&GHit.GfoldAB,"GfoldAB/I");
      tree->Branch("GIdAddBack",GHit.GIdAB,"GIdAB[GfoldAB]/I");
      tree->Branch("GSegAddBack",GHit.GSegAB,"GSegAB[GfoldAB]/I");
      tree->Branch("GRingAddBack",GHit.GRingAB,"GRingAB[GfoldAB]/I");
      tree->Branch("GEAddBack",GHit.GEAB,"GEAB[GfoldAB]/D");
      tree->Branch("GWAddBack",GHit.GWAB,"GWAB[GfoldAB]/D");

      // ion info
      // can include path and eloss for any IonInf but they are all == 0
      tree->Branch("pHit",&pHit,"x/D:y/D:z/D:px/D:py/D:pz/D:E/D:b/D:w/D:path/D:LY/D:Id/I:ring/I:fold/I");
      tree->Branch("rHit",&rHit,"x/D:y/D:z/D:px/D:py/D:pz/D:E/D:b/D:w/D:path/D:LY/D:Id/I:ring/I:fold/I");
      tree->Branch("projGun",&gun,"x/D:y/D:z/D:px/D:py/D:pz/D:E/D:b/D:w/D:path/D:Eloss/D:df/D:t/D");
      tree->Branch("projBackingIn",&pBIn,"x/D:y/D:z/D:px/D:py/D:pz/D:E/D:b/D:w/D:path/D:Eloss/D:df/D:t/D");
      tree->Branch("projTargetIn",&pTIn,"x/D:y/D:z/D:px/D:py/D:pz/D:E/D:b/D:w/D:path/D:Eloss/D:df/D:t/D");
      tree->Branch("projReactionIn",&pRIn,"x/D:y/D:z/D:px/D:py/D:pz/D:E/D:b/D:w/D:path/D:Eloss/D:df/D:t/D");
      tree->Branch("projReactionOut",&pROut,"x/D:y/D:z/D:px/D:py/D:pz/D:E/D:b/D:w/D:path/D:Eloss/D:df/D:t/D");
      tree->Branch("projTargetOut",&pTOut,"x/D:y/D:z/D:px/D:py/D:pz/D:E/D:b/D:w/D:path/D:Eloss/D:df/D:t/D");
      tree->Branch("projDegraderIn",&pDIn,"x/D:y/D:z/D:px/D:py/D:pz/D:E/D:b/D:w/D:path/D:Eloss/D:df/D:t/D");
      tree->Branch("projDegraderOut",&pDOut,"x/D:y/D:z/D:px/D:py/D:pz/D:E/D:b/D:w/D:path/D:Eloss/D:df/D:t/D");
      tree->Branch("projDec",&pDec,"x/D:y/D:z/D:px/D:py/D:pz/D:E/D:b/D:w/D:path/D:Eloss/D:df/D:t/D");      
      tree->Branch("recReactionOut",&rROut,"x/D:y/D:z/D:px/D:py/D:pz/D:E/D:b/D:w/D:path/D:Eloss/D:df/D:t/D");
      tree->Branch("recTargetOut",&rTOut,"x/D:y/D:z/D:px/D:py/D:pz/D:E/D:b/D:w/D:path/D:Eloss/D:df/D:t/D");
      tree->Branch("recDegraderIn",&rDIn,"x/D:y/D:z/D:px/D:py/D:pz/D:E/D:b/D:w/D:path/D:Eloss/D:df/D:t/D");
      tree->Branch("recDegraderOut",&rDOut,"x/D:y/D:z/D:px/D:py/D:pz/D:E/D:b/D:w/D:path/D:Eloss/D:df/D:t/D");
      tree->Branch("recDegraderTrack",&rDTrack,"x/D:y/D:z/D:px/D:py/D:pz/D:E/D:b/D:w/D:path/D:Eloss/D:df/D:t/D");
      tree->Branch("recDec",&rDec,"x/D:y/D:z/D:px/D:py/D:pz/D:E/D:b/D:w/D:path/D:Eloss/D:df/D:t/D");

      // reaction info
      tree->Branch("reactionInfo",&react,"tpL/D:tpCM/D:trL/D:trCM/D:dc/D");
   }

}
//---------------------------------------------------------
void Results::TreeClear()
{
  tree->Delete("all");
  tree=NULL;
  TreeCreate();
}
//---------------------------------------------------------
void Results::TreeReport()
{
  tree->Print(); 
}
//---------------------------------------------------------
void Results::TreeView()
{
  gStyle->SetPalette(1);
  tree->StartViewer(); 
}
//---------------------------------------------------------
void Results::TreeSave(G4String name)
{

/************************************************
   Does not properly save using f.Write()!
   
   Tree prints successfully (is filled) but 
   the saved file does not contain any information 
   by default!

   I have a fix but I'm not sure why it
   works, the two should be interchangeable:
   TFile::Write() makes a loop in the memory.
  ************************************************/

  TDirectory *dir=new TDirectory();
  dir=gDirectory;
  TFile f(name,"recreate");
  f.cd();
  tree->AutoSave();
  tree->SetDirectory(dir);
  //tree->Print();
  tree->Write(); // this seems to fix it
  // f.Write(); 
  f.Close();
  dir->cd();
  G4cout<<"Tree of simulated parameters saved in file "<<name<<G4endl;
}
//---------------------------------------------------------
void Results::TreeRead(G4String)
{
 
}

//---------------------------------------------------------
void Results::TreeAdd(G4String )
{

}
//---------------------------------------------------------
void Results::FillTree(G4int evtNb, TrackerIonHitsCollection* IonCollection,TrackerCsIHitsCollection* CsICollection,G4double gw[GN][GS],G4double ge[GN][GS],G4ThreeVector gp[GN][GS])
{
 G4int Nt=IonCollection->entries();

 memset(&stat,0,sos);
 memset(&rHit,0,soh);
 memset(&pHit,0,soh);
 memset(&gun,0,soi);
 memset(&pBIn,0,soi);
 memset(&pTIn,0,soi);
 memset(&pRIn,0,soi);
 memset(&pROut,0,soi);
 memset(&pTOut,0,soi);
 memset(&pDIn,0,soi);
 memset(&pDOut,0,soi);
 memset(&pDec,0,soi);
 memset(&rROut,0,soi);
 memset(&rTOut,0,soi);
 memset(&rDIn,0,soi);
 memset(&rDOut,0,soi);
 memset(&rDec,0,soi);
 memset(&rDTrack,0,soi);
 memset(&react,0,sor);

 // for(G4int i=0;i<NCsI;i++)
 //   printf("pos %2d index %2d kB %.4f kBm %.7f S %.3f\n",i+1,i,kB[i],kBm[i],S[i]);
 // getc(stdin);

 if(Nt>0) 
   {
     stat.evNb=evtNb;
     stat.Ap=Ap;
     stat.Zp=Zp;
     stat.Ar=Ar;
     stat.Zr=Zr;

     ACM = (Double_t)Ap/((Double_t)Ap+(Double_t)Ar);

     for(Int_t i=0;i<Nt;i++)
       {
       switch((*IonCollection)[i]->GetFlag())
	 {
	 case GUN_FLAG:
	   if((*IonCollection)[i]->GetA()==Ap)
	     if((*IonCollection)[i]->GetZ()==Zp)
	       {
		 gun.x=(*IonCollection)[i]->GetPos().getX()/mm;
		 gun.y=(*IonCollection)[i]->GetPos().getY()/mm;
		 gun.z=(*IonCollection)[i]->GetPos().getZ()/mm;
		 gun.px=(*IonCollection)[i]->GetMom().getX()/MeV;
		 gun.py=(*IonCollection)[i]->GetMom().getY()/MeV;
		 gun.pz=(*IonCollection)[i]->GetMom().getZ()/MeV;
		 gun.b=(*IonCollection)[i]->GetBeta();
		 gun.E=(*IonCollection)[i]->GetKE()/MeV;		 
		 gun.w=(*IonCollection)[i]->GetWeight();
		 gun.t=(*IonCollection)[i]->GetTime()/ns;
	       }
	   break;
	 case BACKING_IN_FLAG:
	   if((*IonCollection)[i]->GetA()==Ap)
	     if((*IonCollection)[i]->GetZ()==Zp)
	       {
		 pBIn.x=(*IonCollection)[i]->GetPos().getX()/mm;
		 pBIn.y=(*IonCollection)[i]->GetPos().getY()/mm;
		 pBIn.z=(*IonCollection)[i]->GetPos().getZ()/mm;
		 pBIn.px=(*IonCollection)[i]->GetMom().getX()/MeV;
		 pBIn.py=(*IonCollection)[i]->GetMom().getY()/MeV;
		 pBIn.pz=(*IonCollection)[i]->GetMom().getZ()/MeV;
		 pBIn.b=(*IonCollection)[i]->GetBeta();
		 pBIn.E=(*IonCollection)[i]->GetKE()/MeV;		 
		 pBIn.w=(*IonCollection)[i]->GetWeight();
		 pBIn.t=(*IonCollection)[i]->GetTime()/ns;
	       }
	   break;
	 case TARGET_IN_FLAG:
	   if((*IonCollection)[i]->GetA()==Ap)
	     if((*IonCollection)[i]->GetZ()==Zp)
	       {
		 pTIn.x=(*IonCollection)[i]->GetPos().getX()/mm;
		 pTIn.y=(*IonCollection)[i]->GetPos().getY()/mm;
		 pTIn.z=(*IonCollection)[i]->GetPos().getZ()/mm;
		 pTIn.px=(*IonCollection)[i]->GetMom().getX()/MeV;
		 pTIn.py=(*IonCollection)[i]->GetMom().getY()/MeV;
		 pTIn.pz=(*IonCollection)[i]->GetMom().getZ()/MeV;
		 pTIn.b=(*IonCollection)[i]->GetBeta();
		 pTIn.E=(*IonCollection)[i]->GetKE()/MeV;		 
		 pTIn.w=(*IonCollection)[i]->GetWeight();
		 pTIn.t=(*IonCollection)[i]->GetTime()/ns;
	       }
	   break;
	 case TARGET_OUT_FLAG:
	   if((*IonCollection)[i]->GetA()==Ap)
	     if((*IonCollection)[i]->GetZ()==Zp)
	       {
		 pTOut.x=(*IonCollection)[i]->GetPos().getX()/mm;
		 pTOut.y=(*IonCollection)[i]->GetPos().getY()/mm;
		 pTOut.z=(*IonCollection)[i]->GetPos().getZ()/mm;
		 pTOut.px=(*IonCollection)[i]->GetMom().getX()/MeV;
		 pTOut.py=(*IonCollection)[i]->GetMom().getY()/MeV;
		 pTOut.pz=(*IonCollection)[i]->GetMom().getZ()/MeV;
		 pTOut.b=(*IonCollection)[i]->GetBeta();
		 pTOut.E=(*IonCollection)[i]->GetKE()/MeV;		 
		 pTOut.w=(*IonCollection)[i]->GetWeight();
		 pTOut.t=(*IonCollection)[i]->GetTime()/ns;
		 if(pTOut.px != pTOut.px)
		   {
		     printf("ERROR\nProjectile target exit momentum is nan!\n");
		     printf("px %f py %f pz %f\n",pTOut.px,pTOut.py,pTOut.pz);
		     printf("x %f y %f z %f\n",pTOut.x,pTOut.y,pTOut.z);
		     printf("px %f py %f pz %f\n",pTOut.px,pTOut.py,pTOut.pz);
		     printf("beta %f E %f w %f\n",pTOut.b,pTOut.E,pTOut.w);
		   }
	       }
	   if((*IonCollection)[i]->GetA()==Ar)
	     if((*IonCollection)[i]->GetZ()==Zr)
	       {
		 rTOut.x=(*IonCollection)[i]->GetPos().getX()/mm;
		 rTOut.y=(*IonCollection)[i]->GetPos().getY()/mm;
		 rTOut.z=(*IonCollection)[i]->GetPos().getZ()/mm;
		 rTOut.px=(*IonCollection)[i]->GetMom().getX()/MeV;
		 rTOut.py=(*IonCollection)[i]->GetMom().getY()/MeV;
		 rTOut.pz=(*IonCollection)[i]->GetMom().getZ()/MeV;
		 rTOut.b=(*IonCollection)[i]->GetBeta();
		 rTOut.E=(*IonCollection)[i]->GetKE()/MeV;		 
		 rTOut.w=(*IonCollection)[i]->GetWeight();
		 rTOut.t=(*IonCollection)[i]->GetTime()/ns;
	       }
	   break;

	 case DEGRADER_IN_FLAG:
	   if((*IonCollection)[i]->GetA()==Ap)
	     if((*IonCollection)[i]->GetZ()==Zp)
	       {
		 pDIn.x=(*IonCollection)[i]->GetPos().getX()/mm;
		 pDIn.y=(*IonCollection)[i]->GetPos().getY()/mm;
		 pDIn.z=(*IonCollection)[i]->GetPos().getZ()/mm;
		 pDIn.px=(*IonCollection)[i]->GetMom().getX()/MeV;
		 pDIn.py=(*IonCollection)[i]->GetMom().getY()/MeV;
		 pDIn.pz=(*IonCollection)[i]->GetMom().getZ()/MeV;
		 pDIn.b=(*IonCollection)[i]->GetBeta();
		 pDIn.E=(*IonCollection)[i]->GetKE()/MeV;		 
		 pDIn.w=(*IonCollection)[i]->GetWeight();
		 pDIn.t=(*IonCollection)[i]->GetTime()/ns;
	       }
	   if((*IonCollection)[i]->GetA()==Ar)
	     if((*IonCollection)[i]->GetZ()==Zr)
	       {
		 rDIn.x=(*IonCollection)[i]->GetPos().getX()/mm;
		 rDIn.y=(*IonCollection)[i]->GetPos().getY()/mm;
		 rDIn.z=(*IonCollection)[i]->GetPos().getZ()/mm;
		 rDIn.px=(*IonCollection)[i]->GetMom().getX()/MeV;
		 rDIn.py=(*IonCollection)[i]->GetMom().getY()/MeV;
		 rDIn.pz=(*IonCollection)[i]->GetMom().getZ()/MeV;
		 rDIn.b=(*IonCollection)[i]->GetBeta();
		 rDIn.E=(*IonCollection)[i]->GetKE()/MeV;		 
		 rDIn.w=(*IonCollection)[i]->GetWeight();
		 rDIn.t=(*IonCollection)[i]->GetTime()/ns;
		 rDiPos.set(rDIn.x,rDIn.y,rDIn.z); // set initial position when the recoil enters the degrader
		 rDTrack.Eloss=rDIn.E;             // assume all energy lost in degrader
	       }
	   break;
	 case DEGRADER_TRACK_FLAG:
	   if((*IonCollection)[i]->GetA()==Ar)
	     if((*IonCollection)[i]->GetZ()==Zr)
	       {
		 rDTrack.x=(*IonCollection)[i]->GetPos().getX()/mm;
		 rDTrack.y=(*IonCollection)[i]->GetPos().getY()/mm;
		 rDTrack.z=(*IonCollection)[i]->GetPos().getZ()/mm;
		 rDTrack.px=(*IonCollection)[i]->GetMom().getX()/MeV;
		 rDTrack.py=(*IonCollection)[i]->GetMom().getY()/MeV;
		 rDTrack.pz=(*IonCollection)[i]->GetMom().getZ()/MeV;
		 rDTrack.b=(*IonCollection)[i]->GetBeta();
		 rDTrack.E=(*IonCollection)[i]->GetKE()/MeV;		 
		 rDTrack.w=(*IonCollection)[i]->GetWeight();
		 rDTrack.t=(*IonCollection)[i]->GetTime()/ns;
		 rDPos.set(rDTrack.x,rDTrack.y,rDTrack.z);  // set current position when the recoil enters the degrader
		 rDTrack.path+=CalculatePath(rDiPos,rDPos); // calculate the path step distance
		 rDiPos=rDPos;                              // reset initial position
	       }
	   break;
	 case DEGRADER_OUT_FLAG:
	   if((*IonCollection)[i]->GetA()==Ap)
	     if((*IonCollection)[i]->GetZ()==Zp)
	       {
		 pDOut.x=(*IonCollection)[i]->GetPos().getX()/mm;
		 pDOut.y=(*IonCollection)[i]->GetPos().getY()/mm;
		 pDOut.z=(*IonCollection)[i]->GetPos().getZ()/mm;
		 pDOut.px=(*IonCollection)[i]->GetMom().getX()/MeV;
		 pDOut.py=(*IonCollection)[i]->GetMom().getY()/MeV;
		 pDOut.pz=(*IonCollection)[i]->GetMom().getZ()/MeV;
		 pDOut.b=(*IonCollection)[i]->GetBeta();
		 pDOut.E=(*IonCollection)[i]->GetKE()/MeV;		 
		 pDOut.w=(*IonCollection)[i]->GetWeight();
		 pDOut.t=(*IonCollection)[i]->GetTime()/ns;
	       }
	   if((*IonCollection)[i]->GetA()==Ar)
	     if((*IonCollection)[i]->GetZ()==Zr)
	       {
		 rDOut.x=(*IonCollection)[i]->GetPos().getX()/mm;
		 rDOut.y=(*IonCollection)[i]->GetPos().getY()/mm;
		 rDOut.z=(*IonCollection)[i]->GetPos().getZ()/mm;
		 rDOut.px=(*IonCollection)[i]->GetMom().getX()/MeV;
		 rDOut.py=(*IonCollection)[i]->GetMom().getY()/MeV;
		 rDOut.pz=(*IonCollection)[i]->GetMom().getZ()/MeV;
		 rDOut.b=(*IonCollection)[i]->GetBeta();
		 rDOut.E=(*IonCollection)[i]->GetKE()/MeV;		 
		 rDOut.w=(*IonCollection)[i]->GetWeight();
		 rDOut.t=(*IonCollection)[i]->GetTime()/ns;
		 rDPos.set(rDOut.x,rDOut.y,rDOut.z);        // set current position when the recoil leaves the degrader
		 rDTrack.path+=CalculatePath(rDiPos,rDPos); // calculate the path step distance
		 rDTrack.Eloss-=rDOut.E;                    // subtract EOut if recoil leaves degrader
	       }
	   break;
	 default:
	   break;
	 }
       }

    for(Int_t i=0;i<Nt;i++)
      {
       switch((*IonCollection)[i]->GetPFlag())
	 {
	 case REACTION_IN_FLAG:
	   if((*IonCollection)[i]->GetA()==Ap)
	     if((*IonCollection)[i]->GetZ()==Zp)
	       {
		 pRIn.x=(*IonCollection)[i]->GetPos().getX()/mm;
		 pRIn.y=(*IonCollection)[i]->GetPos().getY()/mm;
		 pRIn.z=(*IonCollection)[i]->GetPos().getZ()/mm;
		 pRIn.px=(*IonCollection)[i]->GetMom().getX()/MeV;
		 pRIn.py=(*IonCollection)[i]->GetMom().getY()/MeV;
		 pRIn.pz=(*IonCollection)[i]->GetMom().getZ()/MeV;
		 pRIn.b=(*IonCollection)[i]->GetBeta();
		 pRIn.E=(*IonCollection)[i]->GetKE()/MeV;		 
		 pRIn.w=(*IonCollection)[i]->GetWeight();
		 pRIn.t=(*IonCollection)[i]->GetTime()/ns;
		 p1.set(pRIn.px,pRIn.py,pRIn.pz); // projectile momentum vector before reaction
	       }
	   break;
	 case REACTION_OUT_FLAG:
	   if((*IonCollection)[i]->GetA()==Ap)
	     if((*IonCollection)[i]->GetZ()==Zp)
	       {
		 pROut.x=(*IonCollection)[i]->GetPos().getX()/mm;
		 pROut.y=(*IonCollection)[i]->GetPos().getY()/mm;
		 pROut.z=(*IonCollection)[i]->GetPos().getZ()/mm;
		 pROut.px=(*IonCollection)[i]->GetMom().getX()/MeV;
		 pROut.py=(*IonCollection)[i]->GetMom().getY()/MeV;
		 pROut.pz=(*IonCollection)[i]->GetMom().getZ()/MeV;
		 pROut.b=(*IonCollection)[i]->GetBeta();
		 pROut.E=(*IonCollection)[i]->GetKE()/MeV;		 
		 pROut.w=(*IonCollection)[i]->GetWeight();
		 pROut.t=(*IonCollection)[i]->GetTime()/ns;
		 p2.set(pROut.px,pROut.py,pROut.pz); // projectile momentum vector after reaction
		 react.tpL=CalculateTheta(p1,p2);
		 pCM=p2-ACM*p1;
		 react.tpCM=CalculateTheta(p1,pCM);
		 react.dc=CalculateDistCloseApproach(pRIn.E,DEG2RAD*react.tpCM);
		 // printf("KEIn %.4f tpCM %.4f dc %.4f\n",pRIn.E,react.tpCM,react.dc);
	       }
	   if((*IonCollection)[i]->GetA()==Ar)
	     if((*IonCollection)[i]->GetZ()==Zr)
	       {
		 rROut.x=(*IonCollection)[i]->GetPos().getX()/mm;
		 rROut.y=(*IonCollection)[i]->GetPos().getY()/mm;
		 rROut.z=(*IonCollection)[i]->GetPos().getZ()/mm;
		 rROut.px=(*IonCollection)[i]->GetMom().getX()/MeV;
		 rROut.py=(*IonCollection)[i]->GetMom().getY()/MeV;
		 rROut.pz=(*IonCollection)[i]->GetMom().getZ()/MeV;
		 rROut.b=(*IonCollection)[i]->GetBeta();
		 rROut.E=(*IonCollection)[i]->GetKE()/MeV;		 
		 rROut.w=(*IonCollection)[i]->GetWeight();
		 rROut.t=(*IonCollection)[i]->GetTime()/ns;
		 p2.set(rROut.px,rROut.py,rROut.pz); // recoil momentum vector after reaction
		 react.trL=CalculateTheta(p1,p2);
		 pCM=p2-p1+ACM*p1;
		 react.trCM=CalculateTheta(p1,pCM);
		 // printf("i %d trL %.2f trCM %.2f\n",i,react.trL,react.trCM);
	       }
	   break;
	 case DECAY_FLAG:
	   if((*IonCollection)[i]->GetA()==Ap)
	     if((*IonCollection)[i]->GetZ()==Zp)
	       {
		 pDec.x=(*IonCollection)[i]->GetPos().getX()/mm;
		 pDec.y=(*IonCollection)[i]->GetPos().getY()/mm;
		 pDec.z=(*IonCollection)[i]->GetPos().getZ()/mm;
		 pDec.px=(*IonCollection)[i]->GetMom().getX()/MeV;
		 pDec.py=(*IonCollection)[i]->GetMom().getY()/MeV;
		 pDec.pz=(*IonCollection)[i]->GetMom().getZ()/MeV;
		 // printf("px %f py %f pz %f\n",pDec.px,pDec.py,pDec.pz);
		 // getc(stdin);
		 pDec.b=(*IonCollection)[i]->GetBeta();
		 pDec.E=(*IonCollection)[i]->GetKE()/MeV;		 
		 pDec.w=(*IonCollection)[i]->GetWeight();
		 pDec.t=(*IonCollection)[i]->GetTime()/ns;
		 if(pDec.px != pDec.px)
		   {
		     printf("ERROR\nProjectile decay momentum is nan!\n");
		     printf("px %f py %f pz %f\n",pDec.px,pDec.py,pDec.pz);
		     printf("x %f y %f z %f\n",pDec.x,pDec.y,pDec.z);
		     printf("px %f py %f pz %f\n",pDec.px,pDec.py,pDec.pz);
		     printf("beta %f E %f w %f\n",pDec.b,pDec.E,pDec.w);
		   }
	       }
	   if((*IonCollection)[i]->GetA()==Ar)
	     if((*IonCollection)[i]->GetZ()==Zr)
	       {
		 rDec.x=(*IonCollection)[i]->GetPos().getX()/mm;
		 rDec.y=(*IonCollection)[i]->GetPos().getY()/mm;
		 rDec.z=(*IonCollection)[i]->GetPos().getZ()/mm;
		 rDec.px=(*IonCollection)[i]->GetMom().getX()/MeV;
		 rDec.py=(*IonCollection)[i]->GetMom().getY()/MeV;
		 rDec.pz=(*IonCollection)[i]->GetMom().getZ()/MeV;
		 rDec.b=(*IonCollection)[i]->GetBeta();
		 rDec.E=(*IonCollection)[i]->GetKE()/MeV;		 
		 rDec.w=(*IonCollection)[i]->GetWeight();
		 rDec.t=(*IonCollection)[i]->GetTime()/ns;
	       }
	   break;
	   
	 default:
	   break;
	 }
      }
    
     // convert paths to mg/cm^2 based on material
     rDTrack.path = (rDTrack.path/cm)*dDensity*1000.;
     // printf("DEGRADER: EIn %9.3f [MeV]  path %9.3f [mg/cm^2]  energy loss %9.3f [MeV] EOut %9.3f [MeV] \n                               path %9.3f [um]                                   check %8.3f [MeV]\n",rDIn.E/MeV,rDTrack.path,rDTrack.Eloss/MeV,rDOut.E/MeV,(rDTrack.path/dDensity)*10,rDIn.E/MeV-rDTrack.Eloss/MeV);
     // getc(stdin);
   }      
 // G4cout<<" "<<G4endl;
 
 // G4cout<<stat.evNb<<" "<<pRIn.px<<
 //   " "<<pRIn.py<<" "<<pRIn.pz<<
 //   " "<<pRIn.E<<G4endl;
 // G4cout<<stat.evNb<<" "<<pROut.px<<
 //   " "<<pROut.py<<" "<<pROut.pz<<
 //   " "<<pROut.E<<G4endl;
 // G4cout<<stat.evNb<<" "<<rROut.px<<
 //   " "<<rROut.py<<" "<<rROut.pz<<
 //   " "<<rROut.E<<G4endl;
 // G4cout<<stat.evNb<<" "<<pRIn.px-pROut.px-rROut.px<<
 //   " "<<pRIn.py-pROut.py-rROut.py<<" "<<pRIn.pz-pROut.pz-rROut.pz<<
 //   " "<<pRIn.E-pROut.E-rROut.E<<G4endl;
 // getc(stdin);
 
 G4int Np=CsICollection->entries();
 G4double dE,dEdx,dL;
 if(Np>0)
   {
     for(Int_t i=0;i<Np;i++)
       {
	 // G4cout<<"*"<<(*CsICollection)[i]->GetA()<<" "<<(*CsICollection)[i]->GetZ()<<" "<<(*CsICollection)[i]->GetKE()/MeV<<" "<<(*CsICollection)[i]->GetId()<<" "<<(*CsICollection)[i]->GetPos().getX()/um<<" "<<(*CsICollection)[i]->GetPos().getY()/um<<" "<<(*CsICollection)[i]->GetPos().getZ()/um<<G4endl;

	 if((*CsICollection)[i]->GetA()==Ar)
	   if((*CsICollection)[i]->GetZ()==Zr)

	     {
	       if(rHit.E==0.)
		 {
		   rHit.x=(*CsICollection)[i]->GetPos().getX()/mm;
		   rHit.y=(*CsICollection)[i]->GetPos().getY()/mm;
		   rHit.z=(*CsICollection)[i]->GetPos().getZ()/mm;
		   rHit.px=(*CsICollection)[i]->GetMom().getX()/MeV;
		   rHit.py=(*CsICollection)[i]->GetMom().getY()/MeV;
		   rHit.pz=(*CsICollection)[i]->GetMom().getZ()/MeV;
		   rHit.b=(*CsICollection)[i]->GetBeta();
		   rHit.E=(*CsICollection)[i]->GetEdep()/MeV;
		   dE=(*CsICollection)[i]->GetEdep()/MeV;		 
		   rHit.w=(*CsICollection)[i]->GetWeight();
		   rHit.Id=(*CsICollection)[i]->GetId();
		   rHit.fold++;
		   rHit.ring=(*CsICollection)[i]->GetRingId();

		   // in microns
		   // rHit.path=(*CsICollection)[i]->GetPathLength()/um; 
		   // dEdx=((*CsICollection)[i]->GetEdep()/MeV)/((*CsICollection)[i]->GetPathLength()/um); 

		   // in mg/cm^2
		   rHit.path=((*CsICollection)[i]->GetPathLength()/cm)*CsIDensity*1000; 
		   dEdx=((*CsICollection)[i]->GetEdep()/MeV)/(((*CsICollection)[i]->GetPathLength()/cm)*CsIDensity*1000);
 
		   dL=CalculateBirksLawStep(rHit.Id,dE,dEdx); // make sure to check units of kB!
		   rHit.LY+=dL;

		   // printf("RECOIL: id %d ring %d initial KE %9.3f MeV\nrecoil: dE %9.3f path %9.3f  dE/dx %9.3f  dL %9.3f  LY %9.3f\n",rHit.Id,rHit.ring,(*CsICollection)[i]->GetKE()/MeV,rHit.E,rHit.path,dEdx,dL,rHit.LY);

		   // G4cout<<"+"<<i<<" "<<(*CsICollection)[i]->GetA()<<" "<<(*CsICollection)[i]->GetZ()<<" "<<(*CsICollection)[i]->GetEdep()/MeV<<" "<<(*CsICollection)[i]->GetId()<<" "<<(*CsICollection)[i]->GetPos().getX()/um<<" "<<(*CsICollection)[i]->GetPos().getY()/um<<" "<<(*CsICollection)[i]->GetPos().getZ()/um<<" "<<(*CsICollection)[i]->GetPathLength()/um<<" "<<((*CsICollection)[i]->GetEdep()/MeV)/((*CsICollection)[i]->GetPathLength()/um)<<G4endl;
		   // getc(stdin);

		 }
	       else
		 {

		   // G4cout<<"-"<<i<<" "<<(*CsICollection)[i]->GetA()<<" "<<(*CsICollection)[i]->GetZ()<<" "<<(*CsICollection)[i]->GetEdep()/MeV<<" "<<(*CsICollection)[i]->GetId()<<" "<<(*CsICollection)[i]->GetPos().getX()/um<<" "<<(*CsICollection)[i]->GetPos().getY()/um<<" "<<(*CsICollection)[i]->GetPos().getZ()/um<<" "<<(*CsICollection)[i]->GetPathLength()/um<<" "<<((*CsICollection)[i]->GetEdep()/MeV)/((*CsICollection)[i]->GetPathLength()/um)<<G4endl;

		   if((*CsICollection)[i]->GetId()==rHit.Id)
		     {
		       rHit.E+=(*CsICollection)[i]->GetEdep()/MeV;
		       dE=(*CsICollection)[i]->GetEdep()/MeV;		 

		       // in microns
		       // rHit.path+=(*CsICollection)[i]->GetPathLength()/um;
		       // dEdx=((*CsICollection)[i]->GetEdep()/MeV)/((*CsICollection)[i]->GetPathLength()/um);

		       // in mg/cm^2
		       rHit.path+=((*CsICollection)[i]->GetPathLength()/cm)*CsIDensity*1000; 
		       dEdx=((*CsICollection)[i]->GetEdep()/MeV)/(((*CsICollection)[i]->GetPathLength()/cm)*CsIDensity*1000);
 
		       dL=CalculateBirksLawStep(rHit.Id,dE,dEdx); // make sure to check units of kB!
		       rHit.LY+=dL;

		        // printf("recoil: dE %9.3f path %9.3f  dE/dx %9.3f  dL %9.3f  LY %9.3f\n",dE,rHit.path,dEdx,dL,rHit.LY);
		     }	
		 }
	     }
	 
	 if((*CsICollection)[i]->GetA()==Ap)
	   if((*CsICollection)[i]->GetZ()==Zp)
	     {
	       if(pHit.E==0.)
		 {
		   pHit.x=(*CsICollection)[i]->GetPos().getX()/mm;
		   pHit.y=(*CsICollection)[i]->GetPos().getY()/mm;
		   pHit.z=(*CsICollection)[i]->GetPos().getZ()/mm;
		   pHit.px=(*CsICollection)[i]->GetMom().getX()/MeV;
		   pHit.py=(*CsICollection)[i]->GetMom().getY()/MeV;
		   pHit.pz=(*CsICollection)[i]->GetMom().getZ()/MeV;
		   pHit.b=(*CsICollection)[i]->GetBeta();
		   pHit.E=(*CsICollection)[i]->GetEdep()/MeV;
		   dE=(*CsICollection)[i]->GetEdep()/MeV;		 
		   pHit.w=(*CsICollection)[i]->GetWeight();
		   pHit.Id=(*CsICollection)[i]->GetId();
		   pHit.fold++;
		   pHit.ring=(*CsICollection)[i]->GetRingId();

		   // in microns
		   // pHit.path=(*CsICollection)[i]->GetPathLength()/um; 
		   // dEdx=((*CsICollection)[i]->GetEdep()/MeV)/((*CsICollection)[i]->GetPathLength()/um); 

		   // in mg/cm^2
		   pHit.path=((*CsICollection)[i]->GetPathLength()/cm)*CsIDensity*1000; 
		   dEdx=((*CsICollection)[i]->GetEdep()/MeV)/(((*CsICollection)[i]->GetPathLength()/cm)*CsIDensity*1000);
 
		   dL=CalculateBirksLawStep(pHit.Id,dE,dEdx); // make sure to check units of kB!
		   pHit.LY+=dL;

		   // printf("PROJECTILE: id %d ring %d\nprojectile: dE %9.3f path %9.3f  dE/dx %9.3f  dL %9.3f  LY %9.3f\n",pHit.Id,pHit.ring,pHit.E,pHit.path,dEdx,dL,pHit.LY);
		 }
	       else
		 {
		   if((*CsICollection)[i]->GetId()==pHit.Id)
		     {
		       pHit.E+=(*CsICollection)[i]->GetEdep()/MeV;
		       dE=(*CsICollection)[i]->GetEdep()/MeV;		 
		       
		       // in microns
		       // pHit.path+=(*CsICollection)[i]->GetPathLength()/um;
		       // dEdx=((*CsICollection)[i]->GetEdep()/MeV)/((*CsICollection)[i]->GetPathLength()/um);
		       
		       // in mg/cm^2
		       pHit.path+=((*CsICollection)[i]->GetPathLength()/cm)*CsIDensity*1000; 
		       dEdx=((*CsICollection)[i]->GetEdep()/MeV)/(((*CsICollection)[i]->GetPathLength()/cm)*CsIDensity*1000);
 		       
		       dL=CalculateBirksLawStep(pHit.Id,dE,dEdx); // make sure to check units of kB!
		       pHit.LY+=dL;

		       // printf("projectile: dE %9.3f path %9.3f  dE/dx %9.3f  dL %9.3f  LY %9.3f\n",dE,pHit.path,dEdx,dL,pHit.LY);
		     }	
		 }
	     }
       }

     // printf("\nCsI recoil: fold %d id %d ring %d path: %9.3f [mg/cm^2]  Energy: %9.3f (%.3f) [MeV]  Light Output: %9.3f [arb.]\n            path: %9.3f [um]",rHit.fold,rHit.Id,rHit.ring,rHit.path,rHit.E,rDOut.E,rHit.LY,rHit.path/CsIDensity*10);
     // printf("\nCsI projec: fold %d id: %d ring %d path: %9.3f [mg/cm^2]  Energy: %9.3f [MeV]  Light Output: %9.3f [arb.]  \n            path: %9.3f [um]\n",pHit.fold,pHit.Id,pHit.ring,pHit.path,pHit.E,pHit.LY,pHit.path/CsIDensity*10);
      // getc(stdin);
   }
 
  G4int i,j;
  GHit.Gfold=0;
  GHit.GfoldAB=0;
  memset(maxGe,0,sizeof(maxGe));
  memset(GHit.GId,0,sizeof(GHit.GId));
  memset(GHit.GSeg,0,sizeof(GHit.GSeg));
  memset(GHit.GE,0,sizeof(GHit.GE));
  memset(&GHit.GIdAB,0,sizeof(GHit.GIdAB));
  memset(&GHit.GSegAB,0,sizeof(GHit.GSegAB));
  memset(&GHit.GRingAB,0,sizeof(GHit.GRingAB));
  memset(&GHit.GEAB,0,sizeof(GHit.GEAB));
  memset(&GHit.GWAB,0,sizeof(GHit.GWAB));

  //non-addback
  for(i=0;i<GN;i++)//number of positions
    for(j=0;j<GS;j++)//number of crystals
      if(gw[i][j]>0.)
	{
	  GHit.GId[GHit.Gfold]=i+1;
	  GHit.GSeg[GHit.Gfold]=j;
	  GHit.GRing[GHit.Gfold]=RingMap(GHit.GId[GHit.Gfold],GHit.GSeg[GHit.Gfold]);//get the ring in which the hit occured
	  GHit.Gx[GHit.Gfold]=gp[i][j].getX();
	  GHit.Gy[GHit.Gfold]=gp[i][j].getY();
	  GHit.Gz[GHit.Gfold]=gp[i][j].getZ();
	  GHit.GE[GHit.Gfold]=ge[i][j];
	  GHit.GW[GHit.Gfold]=gw[i][j];
	  // printf("Detected: pos %d col %d ring %d energy %f weight %f\n",GHit.GId[GHit.Gfold],GHit.GSeg[GHit.Gfold],GHit.GRing[GHit.Gfold],GHit.GE[GHit.Gfold],GHit.GW[GHit.Gfold]);
	  GHit.Gfold++;
	} 
  // printf("End of event\n");

  for(i=0;i<GN;i++)//number of positions
    {
      for(j=0;j<GS;j++)//number of crystals
	{
	  if(gw[i][j]>0.)
	    {
	      // printf("pos %d col %d energy %f weight %f\n",i+1,j,ge[i][j],gw[i][j]);
	      GHit.GEAB[GHit.GfoldAB]+=ge[i][j];
	      if(ge[i][j]>maxGe[i])
		{
		  GHit.GIdAB[GHit.GfoldAB]=i+1;
		  GHit.GSegAB[GHit.GfoldAB]=j;
		  GHit.GRingAB[GHit.GfoldAB]=RingMap(GHit.GIdAB[GHit.GfoldAB],GHit.GSegAB[GHit.GfoldAB]);//get the ring in which the hit occured
		  GHit.GWAB[GHit.GfoldAB]=gw[i][j];
		  maxGe[i]=ge[i][j];
		}
	    }
	}
      if(GHit.GEAB[GHit.GfoldAB]>0.)
	{
	  // printf("Add back: pos %d col %d ring %d energy %f weight %f\n",GHit.GIdAB[GHit.GfoldAB],GHit.GSegAB[GHit.GfoldAB],GHit.GRingAB[GHit.GfoldAB],GHit.GEAB[GHit.GfoldAB],GHit.GWAB[GHit.GfoldAB]);
	  GHit.GfoldAB++;
	}
    }

  // calculate Doppler factor for decay
  // Gx[0],Gy[0],Gz[0] are first interaction positon
  if(rHit.E>0.)
    for(Int_t i=0;i<GHit.GfoldAB;i++)
      pDec.df = CalculateDopplerFactor(pDec,GHit.Gx[0],GHit.Gy[0],GHit.Gz[0]);
  
  // printf("fold %d\nEND OF EVENT\n",GHit.GfoldAB);
  // getc(stdin);

  tree->Fill();
  IonFill++;
}
//=====================================================================================
void Results::ProjLabScattAngle()
 {
  
   Double_t theta;
   Double_t p;

   memset(&pROut,0,soi);
   c=(TCanvas *)gROOT->FindObject("StdCanv");
   if(c!=NULL)
     c->Close();
   c=new TCanvas("StdCanv","StdCanv",700,500);
   c->Clear();
   c->cd();

   h=(TH1D *)gROOT->FindObject("Proj. Lab. Scatt. Angle");
   if(h==NULL)
     h=new TH1D("Proj. Lab. Scatt. Angle","Proj. Lab. Scatt. Angle",180,0,180);
   h->Reset();

   g=(TH1D *)gROOT->FindObject("Rec. Lab. Scatt. Angle W");
   if(g==NULL)
     g=new TH1D("Proj. Lab. Scatt. Angle W","Proj. Lab. Scatt. Angle W",180,0,180);
   g->Reset();
  
   Int_t N=tree->GetEntries();
 
    for(Int_t i=0;i<N;i++)
    {
      tree->GetEntry(i);     
      p=sqrt(pROut.px*pROut.px+pROut.py*pROut.py+pROut.pz*pROut.pz);
      theta=acos(pROut.pz/p);
      h->Fill(RAD2DEG*theta);     
      g->Fill(RAD2DEG*theta,pROut.w);
    }
   h->GetXaxis()->SetTitle("theta LAB [deg.]");
   h->GetYaxis()->SetTitle("Counts");
   h->SetLineColor(kBlue);
   h->Draw();
   g->SetLineColor(kRed);
   g->Draw("same");
   
 }
//=====================================================================================
void Results::RecLabScattAngle()
 {
 
   Double_t theta;
   Double_t p;
  

   memset(&rROut,0,soi);
   c=(TCanvas *)gROOT->FindObject("StdCanv");
   if(c!=NULL)
     c->Close();

   c=new TCanvas("StdCanv","StdCanv",700,500);
   c->Clear();
   c->cd();
 
   h=(TH1D *)gROOT->FindObject("Rec. Lab. Scatt. Angle");
   if(h==NULL)
     h=new TH1D("Rec. Lab. Scatt. Angle","Rec. Lab. Scatt. Angle",180,0,180);
   h->Reset();

   g=(TH1D *)gROOT->FindObject("Rec. Lab. Scatt. Angle W");
   if(g==NULL)
     g=new TH1D("Rec. Lab. Scatt. Angle W","Rec. Lab. Scatt. Angle W",180,0,180);
   g->Reset();

   Int_t N=tree->GetEntries();
  
   for(Int_t i=0;i<N;i++)
    {
      tree->GetEntry(i);     
      p=sqrt(rROut.px*rROut.px+rROut.py*rROut.py+rROut.pz*rROut.pz);
      theta=acos(rROut.pz/p);
      h->Fill(RAD2DEG*theta);     
      g->Fill(RAD2DEG*theta,rROut.w);
    }
   h->GetXaxis()->SetTitle("theta LAB [deg.]");
   h->GetYaxis()->SetTitle("Counts");
   h->SetLineColor(kBlue);
   h->Draw();
   g->SetLineColor(kRed);
   g->Draw("same");
 }
//=====================================================================================
void Results::CrossSectionZProfile()
 {
 
   Double_t zmin=1000.,zmax=-1000.;
   Double_t s;

   memset(&rROut,0,soi);
   memset(&pRIn,0,soi);
   c=(TCanvas *)gROOT->FindObject("StdCanv");
   if(c!=NULL)
     c->Close();

   c=new TCanvas("StdCanv","StdCanv",700,500);
   c->Clear();
   c->cd();
 
   Int_t N=tree->GetEntries();
   for(Int_t i=0;i<N;i++)
    {
      tree->GetEntry(i); 
      if(pRIn.z==0) printf("%d\n",i);
      if(pRIn.z<zmin) zmin=pRIn.z;
      if(pRIn.z>zmax) zmax=pRIn.z;
    }
 
   s=zmax-zmin;
   s*=0.1;
   zmin-=s;
   zmax+=s;
   
   h=(TH1D *)gROOT->FindObject("Cross Section vs. Z");
   if(h==NULL)
     h=new TH1D("Cross Section vs. Z","Cross Section vs. Z",100,zmin,zmax);
   h->Reset();

   g=(TH1D *)gROOT->FindObject("Cross Section vs. Z W");
   if(g==NULL)
     g=new TH1D("Cross Section vs. Z W","Cross Section vs. Z W",100,zmin,zmax);
   g->Reset();
  
   for(Int_t i=0;i<N;i++)
    {
      tree->GetEntry(i);     
      h->Fill(rROut.z);     
      g->Fill(rROut.z,rROut.w);
    }

   g->Divide(h);
   h->Divide(h);
   g->Scale(TargetFaceCrossSection);
   h->Scale(TargetFaceCrossSection);
   h->GetXaxis()->SetTitle("Z");
   h->GetYaxis()->SetTitle("Cross section [b]");
   h->SetLineColor(kBlue);
   h->Draw();
   g->SetLineColor(kRed);
   g->Draw("same");
 }
//=====================================================================================
void Results::TargetFaceF_CM()
 {
  Double_t theta;
  Double_t pfx,pfy,pfz,pi,pf,dotProduct,A;
  
   memset(&pRIn,0,soi);
   memset(&pROut,0,soi);

   c=(TCanvas *)gROOT->FindObject("StdCanv");
   if(c!=NULL)
     c->Close();

   c=new TCanvas("StdCanv","StdCanv",700,500);
   c->Clear();
   c->cd();
 
   h=(TH1D *)gROOT->FindObject("CM Scatt. Angle");
   if(h==NULL)
     h=new TH1D("CM Scatt. Angle","CM Scatt. Angle",180,0,180);
   h->Reset();

   g=(TH1D *)gROOT->FindObject("CM Scatt. Angle W");
   if(g==NULL)
     g=new TH1D("CM Scatt. Angle W","CM Scatt. Angle W",180,0,180);
   g->Reset();

   Int_t N=tree->GetEntries();
   Double_t norm=0.; // new "normalization" scales curve by sum of weights
   for(Int_t i=0;i<N;i++)
    {
      tree->GetEntry(i);
      A=(Double_t)Ap/((Double_t)Ap+(Double_t)Ar);
      pfx=pROut.px-A*pRIn.px;
      pfy=pROut.py-A*pRIn.py;
      pfz=pROut.pz-A*pRIn.pz;
      pf=sqrt(pfx*pfx+pfy*pfy+pfz*pfz);
      // not really pi but transformation to CM only scales length, doesn't change direction!
      pi=sqrt(pRIn.px*pRIn.px+pRIn.py*pRIn.py+pRIn.pz*pRIn.pz); 
      dotProduct=pfx*pRIn.px+pfy*pRIn.py+pfz*pRIn.pz;
      theta=RAD2DEG*acos(dotProduct/pi/pf); // in degrees
      // printf("%.2f theta %.2f\n",RAD2DEG,theta);
      h->Fill(theta);     
      g->Fill(theta,rROut.w);
      norm+=rROut.w;
    }
   g->GetXaxis()->SetTitle("theta CM [deg.]");
   g->GetYaxis()->SetTitle("Counts");
   // h->SetLineColor(kBlue);
   // h->Draw();
   g->SetLineColor(kRed);
   g->Draw();

   vector <G4double> *th=new vector <G4double>;
   vector <G4double> *ff=new vector <G4double>;
   th=theProjectile->getth();
   ff=theProjectile->getf();
   
   vector<G4double>::iterator it=th->begin();
  
   Int_t n=0,i;
   for(;it<th->end();it++) n++;
   Double_t x[20],y[20];
   if(n>20) n=20;
   // old norm for integral == 1 - incomplete
   //Double_t tn=theProjectile->getthef();
   //printf("tn %.9f old norm %.9f \n",tn,0.5*tn*RAD2DEG/PI);
   //Double_t norm=0.5*tn*RAD2DEG/PI;
   // norm=N/norm;   
   for(i=0;i<n;i++)
     {
       // printf("%f %f\n",(*th)[i],(*ff)[i]);
       x[i]=(*th)[i];
       y[i]=norm*(*ff)[i]*sin((*th)[i]/RAD2DEG); // using new norm, see above
     }
   TGraph *gr1 = new TGraph (n, x, y);
   gr1->Draw("same");

 }

//=====================================================================================
void Results::PlotDistCloseApproach()
 {
   Double_t theta;
   Double_t pfx,pfy,pfz,pi,pf,dotProduct,A;
   Double_t EIn;
   Double_t dc;
   
   memset(&pRIn,0,soi);
   memset(&pROut,0,soi);

   c=(TCanvas *)gROOT->FindObject("StdCanv");
   if(c!=NULL)
     c->Close();

   c=new TCanvas("StdCanv","StdCanv",700,500);
   c->Clear();
   c->cd();

   g=(TH1D *)gROOT->FindObject("Distance of closest approach W");
   if(g==NULL)
     g=new TH1D("Distance of closest approach W","Distance of closest approach W",200,0,20);
   g->Reset();

   Int_t N=tree->GetEntries();
  
   for(Int_t i=0;i<N;i++)
    {
      tree->GetEntry(i);

      // Calculate theta in the CM
      A=(Double_t)Ap/((Double_t)Ap+(Double_t)Ar);
      pfx=pROut.px-A*pRIn.px;
      pfy=pROut.py-A*pRIn.py;
      pfz=pROut.pz-A*pRIn.pz;
      pf=sqrt(pfx*pfx+pfy*pfy+pfz*pfz);
      // not really pi but transformation to CM only scales length, doesn't change direction!
      pi=sqrt(pRIn.px*pRIn.px+pRIn.py*pRIn.py+pRIn.pz*pRIn.pz); 
      dotProduct=pfx*pRIn.px+pfy*pRIn.py+pfz*pRIn.pz;
      theta=acos(dotProduct/pi/pf); // in radians

      // get projectile kinetic energy before reaction
      EIn=pRIn.E;

      // calculate distance of closest approach
      dc=0.5*AHC*(Double_t)Zp*(Double_t)Zr/EIn*(1+(Double_t)Ap/(Double_t)Ar)*(1+1/sin(0.5*theta));

      g->Fill(dc,pROut.w); // y value is weighted
      // printf("Ein %.4f theta %.4f dc %.4f\n",EIn,theta*RAD2DEG,dc);
    }
   g->GetXaxis()->SetTitle("Distance of closest approach [fm]");
   g->GetYaxis()->SetTitle("Counts");
   g->SetLineColor(kRed);
   g->Draw();
 }

//=====================================================================================
G4int Results::RingMap(G4int id,G4int seg)
 {
   G4int r,s;
   s=-1;
   if(seg==0) s=0;
   if(seg==1) s=1;
   if(seg==2) s=1;
   if(seg==3) s=0;
   if(s<0) return -1;

   r=-1;
   switch (id)
     {
     case  1:r=1;break;
     case  2:r=1;break;
     case  3:r=1;break;
     case  4:r=1;break;
     case  5:r=3;break;
     case  6:r=3;break;
     case  7:r=3;break;
     case  8:r=3;break;
     case  9:r=3;break;
     case 10:r=3;break;
     case 11:r=3;break;
     case 12:r=3;break;
     case 13:r=5;break;
     case 14:r=5;break;
     case 15:r=5;break;
     case 16:r=5;break;
     default:break;
     }
   if(r<0) return -1;

   return r+s;
 }
//=====================================================================================
void Results::CsIRingEnergySpectrum(G4int ring)
 {
 
   c=(TCanvas *)gROOT->FindObject("StdCanv");
   if(c!=NULL)
     c->Close();

   c=new TCanvas("StdCanv","StdCanv",700,500);
   c->Clear();
   c->cd();
 
   h=(TH1D *)gROOT->FindObject("ring CsI rec wgt E");
   if(h==NULL)
     h=new TH1D("ring CsI rec wgt E","ring CsI rec wgt E",1024,0,256);
   h->Reset();

   g=(TH1D *)gROOT->FindObject("ring CsI proj wgt E");
   if(g==NULL)
     g=new TH1D("ring CsI proj wgt E","ring CsI proj wgt E",1024,0,256);
   g->Reset();

   Int_t N=tree->GetEntries();
  
   for(Int_t i=0;i<N;i++)
     {
       tree->GetEntry(i);
       if(rHit.ring==ring)
	   {
	     //h->Fill(rHit.E);     
	     h->Fill(rHit.E,rHit.w);
	   }
       if(pHit.ring==ring)
       	   {
       	     //h->Fill(pHit.E);     
       	     g->Fill(pHit.E,pHit.w);
       	   }
     }
	   
   g->GetXaxis()->SetTitle("Energy [MeV]");
   g->GetYaxis()->SetTitle("Counts");
   h->SetLineColor(kBlue);
   g->SetLineColor(kRed);
   h->Draw();
   g->Draw("same");
 }

//=====================================================================================
void Results::CsIDetectorEnergySpectrum(G4int id)
 {
 
   c=(TCanvas *)gROOT->FindObject("StdCanv");
   if(c!=NULL)
     c->Close();

   c=new TCanvas("StdCanv","StdCanv",700,500);
   c->Clear();
   c->cd();
 
   h=(TH1D *)gROOT->FindObject("det CsI rec wgt E");
   if(h==NULL)
     h=new TH1D("det CsI rec wgt E","det CsI rec wgt E",1024,0,256);
   h->Reset();

   g=(TH1D *)gROOT->FindObject("det CsI proj wgt E");
   if(g==NULL)
     g=new TH1D("det CsI proj wgt E","det CsI proj wgt E",1024,0,256);
   g->Reset();

   Int_t N=tree->GetEntries();
  
   for(Int_t i=0;i<N;i++)
     {
       tree->GetEntry(i);
       if(rHit.Id==id)
	   {
	     //h->Fill(rHit.E);     
	     h->Fill(rHit.E,rHit.w);
	   }
       if(pHit.Id==id)
       	   {
       	     //h->Fill(pHit.E);     
       	     g->Fill(pHit.E,pHit.w);
       	   }
     }
	   
   g->GetXaxis()->SetTitle("Energy [MeV]");
   g->GetYaxis()->SetTitle("Counts");
   h->SetLineColor(kBlue);
   g->SetLineColor(kRed);
   h->Draw();
   g->Draw("same");
 }

//=====================================================================================
void Results::CsIRingLYSpectrum(G4int ring)
 {
 
   c=(TCanvas *)gROOT->FindObject("StdCanv");
   if(c!=NULL)
     c->Close();

   c=new TCanvas("StdCanv","StdCanv",700,500);
   c->Clear();
   c->cd();
 
   h=(TH1D *)gROOT->FindObject("ring CsI LY");
   if(h==NULL)
     h=new TH1D("ring CsI LY","ring CsI LY",S8K,0,S32K);
   h->Reset();

   g=(TH1D *)gROOT->FindObject("ring CsI LY W");
   if(g==NULL)
     g=new TH1D("ring CsI LY W","ring CsI LY W",S8K,0,S32K);
   g->Reset();

   Int_t N=tree->GetEntries();
  
   for(Int_t i=0;i<N;i++)
     {
       tree->GetEntry(i);
       if(rHit.ring==ring)
	   {
	     h->Fill(rHit.LY);     
	     g->Fill(rHit.LY,rHit.w);
	   }
       if(pHit.ring==ring)
       	   {
       	     h->Fill(pHit.LY);     
       	     g->Fill(pHit.LY,pHit.w);
       	   }
     }
	   
   g->GetXaxis()->SetTitle("Light Yield [arb.]");
   g->GetYaxis()->SetTitle("Counts");
   h->SetLineColor(kBlue);
   h->Draw();
   g->SetLineColor(kRed);
   g->Draw("same");
 }

//=====================================================================================
void Results::CsIDetectorLYSpectrum(G4int id)
 {
 
   c=(TCanvas *)gROOT->FindObject("StdCanv");
   if(c!=NULL)
     c->Close();

   c=new TCanvas("StdCanv","StdCanv",700,500);
   c->Clear();
   c->cd();
 
   h=(TH1D *)gROOT->FindObject("det CsI LY");
   if(h==NULL)
     h=new TH1D("det CsI LY","det CsI LY",S8K,0,S32K);
   h->Reset();

   g=(TH1D *)gROOT->FindObject("det CsI LY W");
   if(g==NULL)
     g=new TH1D("det CsI LY W","det CsI LY W",S8K,0,S32K);
   g->Reset();

   Int_t N=tree->GetEntries();
  
   for(Int_t i=0;i<N;i++)
     {
       tree->GetEntry(i);
       if(rHit.Id==id)
	   {
	     h->Fill(rHit.LY);     
	     g->Fill(rHit.LY,rHit.w);
	   }
       if(pHit.Id==id)
       	   {
       	     h->Fill(pHit.LY);     
       	     g->Fill(pHit.LY,pHit.w);
       	   }
     }
	   
   g->GetXaxis()->SetTitle("Light Yield [arb.]");
   g->GetYaxis()->SetTitle("Counts");
   h->SetLineColor(kBlue);
   h->Draw();
   g->SetLineColor(kRed);
   g->Draw("same");
 }

//=====================================================================================
void Results::CsIFold()
 {
 
   c=(TCanvas *)gROOT->FindObject("StdCanv");
   if(c!=NULL)
     c->Close();

   c=new TCanvas("StdCanv","StdCanv",700,500);
   c->Clear();
   c->cd();
 
   h=(TH1D *)gROOT->FindObject("CsI Fold");
   if(h==NULL)
     h=new TH1D("CsI Fold","CsI Fold",10,0,10);
   h->Reset();

   Int_t N=tree->GetEntries();
  
   for(Int_t i=0;i<N;i++)
     {
       tree->GetEntry(i);
       h->Fill(rHit.fold+pHit.fold); //
     }
	   
   h->GetXaxis()->SetTitle("CsI Fold");
   h->GetYaxis()->SetTitle("Counts");
   h->SetLineColor(kBlue);
   h->Draw();
  }

//=====================================================================================
void Results::GammaRingSpectrum(G4int ring)
 {

   // display interactive spectra from macro
   //theApp=new TApplication("App", &ac, av);

   c=(TCanvas *)gROOT->FindObject("StdCanv");
   if(c!=NULL)
     c->Close();

   c=new TCanvas("StdCanv","StdCanv",700,500);
   c->Clear();
   c->cd();
 
   h=(TH1D *)gROOT->FindObject("Gamma");
   if(h==NULL)
     h=new TH1D("Gamma","Gamma",4096,0,4096);
   h->Reset();

   g=(TH1D *)gROOT->FindObject("Gamma W");
   if(g==NULL)
     g=new TH1D("Gamma W","Gamma W",4096,0,4096);
   g->Reset();

   Int_t N=tree->GetEntries();
  
   for(Int_t i=0;i<N;i++)
     {
       tree->GetEntry(i);
       for(Int_t j=0;j<GHit.Gfold;j++)
	 if(GHit.GRing[j]==ring)
	   {
	     h->Fill(FWHM_response(GHit.GE[j]));     
	     g->Fill(FWHM_response(GHit.GE[j]),GHit.GW[j]);
	   }
     }
	   
   g->GetXaxis()->SetTitle("Energy [keV]");
   g->GetYaxis()->SetTitle("Counts");
   // h->SetLineColor(kBlue);
   // h->Draw();
   g->SetLineColor(kRed);
   g->Draw("same");

   //theApp->Run(kTRUE);

 }

//=====================================================================================
void Results::GammaABRingSpectrum(G4int ring)
 {

   // display interactive spectra from macro
   //theApp=new TApplication("App", &ac, av);

   c=(TCanvas *)gROOT->FindObject("StdCanv");
   if(c!=NULL)
     c->Close();

   c=new TCanvas("StdCanv","StdCanv",700,500);
   c->Clear();
   c->cd();
 
   h=(TH1D *)gROOT->FindObject("Gamma AB");
   if(h==NULL)
     h=new TH1D("Gamma AB","Gamma AB",4096,0,4096);
   h->Reset();

   g=(TH1D *)gROOT->FindObject("Gamma AB W");
   if(g==NULL)
     g=new TH1D("Gamma AB W","Gamma AB W",4096,0,4096);
   g->Reset();

   Int_t N=tree->GetEntries();
  
   for(Int_t i=0;i<N;i++)
     {
       tree->GetEntry(i);
       for(Int_t j=0;j<GHit.GfoldAB;j++)
	 if(GHit.GRingAB[j]==ring)
	   {
	     h->Fill(FWHM_response(GHit.GEAB[j]));     
	     g->Fill(FWHM_response(GHit.GEAB[j]),GHit.GWAB[j]);
	   }
     }
	   
   g->GetXaxis()->SetTitle("Energy [keV]");
   g->GetYaxis()->SetTitle("Counts");
   // h->SetLineColor(kBlue);
   // h->Draw();
   g->SetLineColor(kRed);
   g->Draw("same");

   //theApp->Run(kTRUE);

 }

 //=====================================================================================
void Results::DetCryCsIGammaSpectrum(G4int det,G4int cry, G4int pin)
 {
   G4String name="Det. ";
   
   name.append((G4String)det);
   name.append(" Cry. ");
   name.append((G4String)cry);
   name.append(" CsI ");
   name.append((G4String)pin);

   c=(TCanvas *)gROOT->FindObject("StdCanv");
   if(c!=NULL)
     c->Close();

   c=new TCanvas("StdCanv","StdCanv",700,500);
   c->Clear();
   c->cd();
 
   h=(TH1D *)gROOT->FindObject("Gamma CsI");
   if(h==NULL)
     h=new TH1D("Gamma CsI","Gamma CsI",4096,0,4096);
   h->Reset();

   g=(TH1D *)gROOT->FindObject("Gamma CsI W");
   if(g==NULL)
     g=new TH1D("Gamma CsI W","Gamma CsI W",4096,0,4096);
   g->Reset();

   Int_t N=tree->GetEntries();
  
   for(Int_t i=0;i<N;i++)
     {
       tree->GetEntry(i);
       if(rHit.Id==pin)
	 for(Int_t j=0;j<GHit.Gfold;j++)
	   if(GHit.GId[j]==det)
	     if(GHit.GSeg[j]==cry)
	       {
		 h->Fill(FWHM_response(GHit.GE[j]));     
		 g->Fill(FWHM_response(GHit.GE[j]),GHit.GW[j]);
	       }
     }
   
   h->GetXaxis()->SetTitle("Energy [keV]");
   h->GetYaxis()->SetTitle("Counts");
   h->SetLineColor(kBlue);
   h->SetTitle(name);
   h->Draw();
   g->SetLineColor(kRed);
   g->SetTitle(name);
   g->Draw("same");

 }
  //=====================================================================================
void Results::DetRingCsIRingGammaSpectrum(G4int detRing,G4int pinRing,G4double coeff)
 {
   char name[132];
   G4double eg,ec;

   sprintf(name,"Gamma Ring %1d CsI Ring %1d",detRing,pinRing);
   c=(TCanvas *)gROOT->FindObject("StdCanv");
   if(c!=NULL)
     c->Close();

   c=new TCanvas("StdCanv1","StdCanv1",700,500);
   c->Clear();
   c->Divide(1,2);

 
   correlation=(TH2D *)gROOT->FindObject("ECsI_EGamma");
   if(correlation==NULL)
     correlation=new TH2D("ECsI_EGamma","ECsI_EGamma",256,0,256,2048,0,4096);
   correlation->Reset();

   h=(TH1D *)gROOT->FindObject("Gamma corr");
   if(h==NULL)
     h=new TH1D("Gamma corr","Gamma corr",2048,0,4096);
   h->Reset();

  
   Int_t N=tree->GetEntries();
  
  for(Int_t i=0;i<N;i++)
     {
       tree->GetEntry(i);
       if(rHit.ring==pinRing)
	 for(Int_t j=0;j<GHit.Gfold;j++)
	   if(RingMap(GHit.GId[j],GHit.GSeg[j])==detRing)
	       {
		 ec=rHit.E/MeV;
		 eg=FWHM_response(GHit.GE[j])-coeff*ec;
		 correlation->Fill(ec,eg,GHit.GW[j]); // should this have the CsI weight as well?
		 h->Fill(eg,GHit.GW[j]);
	       }
     }

   c->cd(1);
   gStyle->SetPalette(1);
   correlation->GetXaxis()->SetTitle("ECsI [MeV]");
   correlation->GetYaxis()->SetTitle("EGamma [keV]");
   correlation->SetTitle(name);
   correlation->Draw("COLZ");
   c->cd(2);
   h->GetXaxis()->SetTitle("Uncorrelated Energy [arb.]");
   h->GetYaxis()->SetTitle("Counts");
   h->SetLineColor(kBlue);
   h->SetTitle(name);
   h->Draw();  
 }
 
//=====================================================================================
void Results::CalculateCrystalPositions()
 {
   Double_t x[GN][GS],y[GN][GS],z[GN][GS],n[GN][GS];
   Double_t xx,yy,zz;
   size_t sod=GN*4*sizeof(Double_t);
   Double_t chZ = theDetector->GetChamber()->GetPosZ();
   memset(x,0,sod);
   memset(y,0,sod);
   memset(z,0,sod);
   memset(n,0,sod);
   Int_t N=tree->GetEntries();
  
   for(Int_t i=0;i<N;i++)
     {
       tree->GetEntry(i);
       for(Int_t j=0;j<GHit.Gfold;j++)
	 {
	   x[GHit.GId[j]-1][GHit.GSeg[j]]+=GHit.Gx[j];
	   y[GHit.GId[j]-1][GHit.GSeg[j]]+=GHit.Gy[j];
	   z[GHit.GId[j]-1][GHit.GSeg[j]]+=GHit.Gz[j];
	   n[GHit.GId[j]-1][GHit.GSeg[j]]++;
	 }
     }

   for(Int_t det=0;det<GN;det++)
     for(Int_t cry=0;cry<GS;cry++)
       {
	 xx=x[det][cry]/n[det][cry];
	 yy=y[det][cry]/n[det][cry];
	 zz=z[det][cry]/n[det][cry]-chZ;
	 CP[det][cry].setX(xx);
	 CP[det][cry].setY(yy);
	 CP[det][cry].setZ(zz);
       }
   ReportCrystalPositions();
 }
//=====================================================================================
void Results::ReportCrystalPositions()
 {
   Double_t xx,yy,zz,rr,theta,phi;
   Double_t chZ = theDetector->GetChamber()->GetPosZ();
   printf("\n----- TIGRESS crystal positions -----\n");
   printf("----- z and angles wrt TIGRESS center -----\n");
   printf("det cry ring     x        y        z        r       theta     phi\n");
   for(Int_t det=0;det<GN;det++)
     for(Int_t cry=0;cry<GS;cry++)
       {
	 xx=CP[det][cry].getX();
	 yy=CP[det][cry].getY();
	 zz=CP[det][cry].getZ()-chZ;
	 rr=sqrt(xx*xx+yy*yy+zz*zz);
	 theta=acos(zz/rr)*RAD2DEG;
	 phi=atan2(-yy,-xx)*RAD2DEG+180; // phi from 0 to 360
	 if(phi<0)
	   phi+=360;
	 printf(" %2d  %2d  %3d %8.3f %8.3f %8.3f %8.3f %8.3f %8.3f\n",det+1,cry,RingMap(det+1,cry),xx,yy,zz,rr,theta,phi);
       }
       
 } 
//=====================================================================================
void Results::CalculateCsIPositions()
 {
   Double_t x[NCsI],y[NCsI],z[NCsI],n[NCsI];
   Double_t xx,yy,zz;
   size_t sod=NCsI*sizeof(Double_t);
   Double_t chZ = theDetector->GetChamber()->GetPosZ();
   memset(x,0,sod);
   memset(y,0,sod);
   memset(z,0,sod);
   memset(n,0,sod);
   Int_t N=tree->GetEntries();
  
   for(Int_t i=0;i<N;i++)
     {
       tree->GetEntry(i);
       if(pHit.E>0)
	 {
	   x[(Int_t)pHit.Id-1]+=pHit.x;
	   y[(Int_t)pHit.Id-1]+=pHit.y;
	   z[(Int_t)pHit.Id-1]+=pHit.z;
	   n[(Int_t)pHit.Id-1]++;
	 }
       if(rHit.E>0)
	 {
	   x[(Int_t)rHit.Id-1]+=rHit.x;
	   y[(Int_t)rHit.Id-1]+=rHit.y;
	   z[(Int_t)rHit.Id-1]+=rHit.z;
	   n[(Int_t)rHit.Id-1]++;
	 }
     }

   for(Int_t det=0;det<NCsI;det++)
       {
	 xx=x[det]/n[det];
	 yy=y[det]/n[det];
	 zz=z[det]/n[det]-chZ;
	 PP[det].setX(xx);
	 PP[det].setY(yy);
	 PP[det].setZ(zz);
       }
   ReportCsIPositions();
 }
//=====================================================================================
void Results::ReportCsIPositions()
 {
   Double_t xx,yy,zz,rr,theta,phi;
   Double_t chZ = theDetector->GetChamber()->GetPosZ();
   printf("\n----- CsI array detector positions -----\n");
   printf("----- z and angles wrt to chamber center -----\n");
   printf("csi      x        y        z        r       theta     phi\n");
   for(Int_t det=0;det<NCsI;det++)
       {
	 xx=PP[det].getX();
	 yy=PP[det].getY();
	 zz=PP[det].getZ()-chZ;
	 rr=sqrt(xx*xx+yy*yy+zz*zz);
	 theta=acos(zz/rr)*RAD2DEG;
	 phi=atan2(-yy,-xx)*RAD2DEG+180;
	 if(phi<0)
	   phi+=360;
	 printf(" %2d  %8.3f %8.3f %8.3f %8.3f %8.3f %8.3f\n",det+1,xx,yy,zz,rr,theta,phi);
       }
       
 } 
//=====================================================================================
void Results::GroupCosDist()
 {   
   //Int_t csi,det,cry;
   Double_t cos;

   c=(TCanvas *)gROOT->FindObject("StdCanv");
   if(c!=NULL)
     c->Close();

   c=new TCanvas("StdCanv","StdCanv",700,500);
   c->Clear();
   c->cd();
 
   h=(TH1D *)gROOT->FindObject("GroupCos");
   if(h==NULL)
     h=new TH1D("GroupCos","GroupCos",200,-1.0,1.0);
   h->Reset();
  
   Int_t N=tree->GetEntries();
   // printf("Total number of tree entries is %d\n",N);
   for(Int_t i=0;i<N;i++)
     {
       tree->GetEntry(i);
       // printf("Getting entry %d...\n",i);       
       if(rHit.E>0)
	 {
	   // printf("CsI recoil hit in position %2d\n",(Int_t)rHit.Id);
	   // printf("Gamma fold is %d\n",GHit.GfoldAB);
	   for(Int_t j=0;j<GHit.GfoldAB;j++)
	     {
	       // printf("Gamma-ray detected at pos %2d col %d\n",GHit.GIdAB[j],GHit.GSegAB[j]);
	       cos=PP[(Int_t)rHit.Id-1].dot(CP[GHit.GIdAB[j]-1][GHit.GSegAB[j]])/PP[(Int_t)rHit.Id-1].mag()/CP[GHit.GIdAB[j]-1][GHit.GSegAB[j]].mag();
	       h->Fill(cos);
	       // printf("cos %9.5f\n",cos);
	     }	   
	 }
       if(pHit.E>0)
	 {
	   // printf("CsI projectile hit in position %2d\n",(Int_t)pHit.Id);
	   // printf("Gamma fold is %d\n",GHit.GfoldAB);
	   for(Int_t j=0;j<GHit.Gfold;j++)
	     {
	       // printf("Gamma-ray detected at pos %2d col %d\n",GHit.GIdAB[j],GHit.GSegAB[j]);
	       cos=PP[(Int_t)rHit.Id-1].dot(CP[GHit.GIdAB[j]-1][GHit.GSegAB[j]])/PP[(Int_t)rHit.Id-1].mag()/CP[GHit.GIdAB[j]-1][GHit.GSegAB[j]].mag();
	       h->Fill(cos);
	       // printf("cos %9.5f\n",cos);
	     }	   
	 }
       // printf("---- End of event ----\n\n");
       // getc(stdin);
     }
        
   h->GetXaxis()->SetTitle("cos(theta)");
   h->GetYaxis()->SetTitle("Counts");
   h->SetLineColor(kRed);
   h->Draw();

 }

//=====================================================================================
void Results::SetFWHM_response(G4double iF,G4double iG, G4double iH)
{
  F=iF;
  G=iG;
  H=iH;
  G4cout<<"FWHM response F="<<F<<" G="<<G<<" H="<<H<<G4endl;
}

//=====================================================================================
G4double Results::FWHM_response(G4double e_in)
{
  G4double e_out,fwhm,sigma,e,channel;
  
  e=e_in/1000.; // e in MeV
  // printf("e %f ein %f\n",e,e_in);
  fwhm=sqrt(F+G*e+H*e*e);
  channel=A+B*e_in+C*e_in*e_in;
  sigma=fwhm/2.35482;
   // printf("channel %f sigma %f\n",channel,sigma);
  if(sigma>0)
    e_out=CLHEP::RandGauss::shoot(channel,sigma);
  else
    e_out=channel;

   // printf("e_out  %f\n",e_out);
   // getc(stdin);
  return e_out;
}

//=====================================================================================
G4double Results::CalculatePath(G4ThreeVector iPos, G4ThreeVector Pos)
{
  G4ThreeVector dPos = Pos-iPos;
  return dPos.mag();
}

//=====================================================================================
void Results::SetCsILYResponse(G4int id,G4double k,G4double s)
{
  G4int i;

  if(id==-1) // all detectors
    {
      for(i=1;i<=NCsI;i++)
	{
	  theDetector->GetCsIArray()->SetBirksConstant(i,k);
	  theDetector->GetCsIArray()->SetLYScaling(i,s);
	}
    }
  else if(id>=1 && id<=NCsI) // single detector
    {
      theDetector->GetCsIArray()->SetBirksConstant(id,k);
      theDetector->GetCsIArray()->SetLYScaling(id,s);
    }
  else
    {
      printf("Error: No CsI for id %d\n",id);
      exit(-1);
    }
}

//=====================================================================================
G4double Results::CalculateBirksLawStep(G4int id,G4double dE,G4double dEdx)
{
  G4double dL;    // differential light yield
  //dL = S[id-1]*dE/(1+kB[id-1]*dEdx); // for dEdx in MeV/um
  dL = S[id-1]*dE/(1+kBm[id-1]*dEdx); // for dEdx in MeV/(mg/cm^2)
  return dL;
}


//=====================================================================================
G4double Results::CalculateTheta(G4ThreeVector p1,G4ThreeVector p2)
{
  G4double theta;
  theta = RAD2DEG*acos(p1.dot(p2)/p1.mag()/p2.mag()); // in degrees
  return theta;
}

//=====================================================================================
G4double Results::CalculateDistCloseApproach(G4double KEIn,G4double theta)
{
  G4double d=0.;
  d=0.5*AHC*(Double_t)Zp*(Double_t)Zr/KEIn*(1+(Double_t)Ap/(Double_t)Ar)*(1+1/sin(0.5*theta));
  return d;
}

//=====================================================================================
G4double Results::CalculateDopplerFactor(IonInf Proj, G4double gx, G4double gy, G4double gz)
{
  G4double df=0.;
  G4ThreeVector ProjR,GammaR;  // positions
  G4ThreeVector eProjp,eGamma; // unit vectors for proj momentum and gamma dir
  G4double epsp=0.000001; // small projectile momentum epsilon = 1 eV to avoid rounding errors
  
  eProjp.set(Proj.px,Proj.py,Proj.pz); // in MeV, see FillTree()
  G4double mag = eProjp.mag(); // initial momentum vector magnitude in MeV

  GammaR.set(gx,gy,gz);
  ProjR.set(Proj.x,Proj.y,Proj.z);
  GammaR-=ProjR; // interaction position - decay position
  
  if(mag > epsp) // for non-zero momenta during decay
    {
      eProjp*=(1./mag); // normalized momentum vector
      eGamma = GammaR*(1./GammaR.mag()); // normalized gamma vector
      df = sqrt(1.-Proj.b*Proj.b)/(1.-Proj.b*eProjp.dot(eGamma));
    }
  else // decays at rest
    df = 1.;
  
  // nan check on df
  if(df != df)
    {
      printf("Parameters:\neProjp.x %f eProjp.y %f eProjp.z %f eProjp.mag %f\n",eProjp.getX(),eProjp.getY(),eProjp.getZ(),eProjp.mag());
      printf("iProj.px %f iProj.py %f iProj.pz %f iProj.mag %f\n",Proj.px,Proj.py,Proj.pz,mag);
      printf("iProj.x %f iProj.y %f iProj.z %f\n",Proj.x,Proj.y,Proj.z);
      printf("Gamma.x %f Gamma.y %f Gamma.z %f Gamma.mag %f\n",GammaR.getX(),GammaR.getY(),GammaR.getZ(),GammaR.mag());
      printf("eGamma.x %f eGamma.y %f eGamma.z %f eGamma.mag %f\n",eGamma.getX(),eGamma.getY(),eGamma.getZ(),eGamma.mag());
      printf("beta %f eProjp.eGamma %f theta %f\n",Proj.b,eProjp.dot(eGamma),RAD2DEG*acos(eProjp.dot(eGamma)));
      printf("-----> df %f <-----\n",df);
      exit(EXIT_FAILURE);
    }

      // printf("Parameters:\neProjp.x %f eProjp.y %f eProjp.z %f eProjp.mag %f\n",eProjp.getX(),eProjp.getY(),eProjp.getZ(),eProjp.mag());
      // printf("iProj.px %f iProj.py %f iProj.pz %f iProj.mag %f\n",Proj.px,Proj.py,Proj.pz,mag);
      // printf("iProj.x %f iProj.y %f iProj.z %f\n",Proj.x,Proj.y,Proj.z);
      // printf("Gamma.x %f Gamma.y %f Gamma.z %f Gamma.mag %f\n",GammaR.getX(),GammaR.getY(),GammaR.getZ(),GammaR.mag());
      // printf("eGamma.x %f eGamma.y %f eGamma.z %f eGamma.mag %f\n",eGamma.getX(),eGamma.getY(),eGamma.getZ(),eGamma.mag());
      // printf("beta %f eProjp.eGamma %f theta %f\n",Proj.b,eProjp.dot(eGamma),RAD2DEG*acos(eProjp.dot(eGamma)));
      // printf("-----> df %f <-----\n",df);
      // getc(stdin);
 
  return df;
}

//=====================================================================================
void Results::SaveDopplerFactorData()
{
  G4double dopplerFactor[GN][GS][NCsI]={0.};
  G4double counter[GN][GS][NCsI]={0.};

   c=(TCanvas *)gROOT->FindObject("StdCanv");
   if(c!=NULL)
     c->Close();
   
   c=new TCanvas("StdCanv","StdCanv",700,500);
   c->Clear();
   c->cd();
 
   d=(TH2D *)gROOT->FindObject("DopplerFactor");
   if(d==NULL)
     d=new TH2D("DopplerFactor","DopplerFactor",64,1,64,24,1,24);
   d->Reset();
  
   Int_t N=tree->GetEntries();
   for(Int_t i=0;i<N;i++)
     {
       tree->GetEntry(i);
       if(rHit.E>0.)
	 {
	   for(Int_t j=0;j<GHit.GfoldAB;j++)
	     {
	       // pos [0,...,15], col [0,...,3], csi [0,...,23]
	       dopplerFactor[GHit.GIdAB[j]-1][GHit.GSegAB[j]][rHit.Id-1] += pDec.df; 
	       counter[GHit.GIdAB[j]-1][GHit.GSegAB[j]][rHit.Id-1] += 1.0;
	     }
	 }
     }
   
   // make histogram and write map to disk
   G4int i,j,k;
   G4int tig,group;
   FILE *df_map;
   FILE *group_map;

   df_map=fopen("Doppler_shift_map.par","w");
   fprintf(df_map,"This is a Doppler shift map for the TIGRESS and CsI array\n");
   fprintf(df_map,"pos   col    csi    df\n");

   group_map=fopen("Doppler_shift_group_map.par","w");
   fprintf(group_map,"This is a Doppler shift group map for the TIGRESS and CsI array\n");
   fprintf(group_map,"pos   col    csi    group\n");

   for(i=0;i<GN;i++)
     for(j=0;j<GS;j++)
       for(k=0;k<NCsI;k++)
	 {
	   if(counter[i][j][k]>0.)
	     {
	       // nan check on df
	       if(dopplerFactor[i][j][k] != dopplerFactor[i][j][k])
		 {
		   printf("dopplerFactor[%d][%d][%d] = %f with counter[%d][%d][%d] %f\n",i,j,k,dopplerFactor[i][j][k],i,j,k,counter[i][j][k]);
		   exit(EXIT_FAILURE);
		 }

	       dopplerFactor[i][j][k] /= counter[i][j][k]; // average
	       tig=i*GS+j;

	       // tig [1,...,64] csi [1,...,24] due to root binning from -1 (!)
	       d->SetBinContent(tig,k+1,dopplerFactor[i][j][k]);
	       fprintf(df_map,"%2d     %d     %2d     %7.6f\n",i+1,j,k+1,dopplerFactor[i][j][k]);

	       // make the groups
	       /***********************
               For 94Sr:
               1           D  > 1.025
               2  1.010 <  D  <= 1.025
               3  0.987 <  D  <= 1.010
               4  0.974 <  D  <= 0.987
               5  0.974 <= D

               For 84Kr:
               1           D  > 1.024
               2  1.011 <  D  <= 1.024
               3  0.987 <  D  <= 1.011
               4  0.976 <  D  <= 0.987
               5  0.976 <= D
		**********************/
	       if(dopplerFactor[i][j][k] > 1.024)
		 {
		   group=1;
		   fprintf(group_map,"%2d     %d     %2d     %d\n",i+1,j,k+1,group);
		 }
	       if(dopplerFactor[i][j][k] > 1.011 && dopplerFactor[i][j][k] <= 1.024)
		 {
		   group=2;
		   fprintf(group_map,"%2d     %d     %2d     %d\n",i+1,j,k+1,group);
		 }
	       if(dopplerFactor[i][j][k] > 0.987 && dopplerFactor[i][j][k] <= 1.011)
		 {
		   group=3;
		   fprintf(group_map,"%2d     %d     %2d     %d\n",i+1,j,k+1,group);
		 }
	       if(dopplerFactor[i][j][k] > 0.976 && dopplerFactor[i][j][k] <= 0.987)
		 {
		   group=4;
		   fprintf(group_map,"%2d     %d     %2d     %d\n",i+1,j,k+1,group);
		 }
	       if(dopplerFactor[i][j][k] <= 0.976)
		 {
		   group=5;
		   fprintf(group_map,"%2d     %d     %2d     %d\n",i+1,j,k+1,group);
		 }

	     }
	 }
   fclose(df_map);
   fclose(group_map);

   // write histogram to disk
   char title[132];
   sprintf(title,"DopplerFactor.root");
   TFile f(title, "recreate");
   d->GetXaxis()->SetTitle("Tigress ID");
   d->GetYaxis()->SetTitle("CsI ID");
   d->SetMaximum(1.06);
   d->SetMinimum(0.94);
   d->SetOption("COLZ");
   d->Write();
}


