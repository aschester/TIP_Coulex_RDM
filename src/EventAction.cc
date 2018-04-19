#include "EventAction.hh"
 
EventAction::EventAction(Results* RE,RunAction* RA,Projectile* proj,Recoil* rec,DetectorConstruction* det):results(RE),run_action(RA),theProjectile(proj),theRecoil(rec),theDetector(det)
//EventAction::EventAction(Results* RE,RunAction* RA):results(RE),run_action(RA)
{ 
  ionCollectionID=-1;
  CsICollectionID=-1;
  soa=16*4*sizeof(G4double);
  sov=16*4*sizeof(G4ThreeVector);
  soc=sizeof(TrackerCsIHitsCollection);
  soi=sizeof(TrackerIonHitsCollection);
  SetTriggerGammaPinCoinc();
  CsIThreshold=3*MeV;

  // initialize info for CsI trigger - values at compilation!
  Ap=theProjectile->getA();
  Zp=theProjectile->getZ();
  Ar=theRecoil->getA();
  Zr=theRecoil->getZ();

  // global scope, allocated only once
  CsI=new TrackerCsIHitsCollection();
  HI=new TrackerIonHitsCollection();

}


EventAction::~EventAction()
{
  /* This causes a segfault */
  //delete CsI;
  //delete HI;
  ;
}

void EventAction::BeginOfEventAction(const G4Event*)
{
 
  G4SDManager * SDman = G4SDManager::GetSDMpointer();

  if(ionCollectionID<0)
    ionCollectionID=SDman->GetCollectionID("ionCollection");

  if(CsICollectionID<0)
    CsICollectionID=SDman->GetCollectionID("CsICollection");
 
  memset(GriffinCrystEnergyDet,0,soa);
  memset(GriffinCrystWeightDet,0,soa);
  memset(GriffinCrystPosDet,0,sov);
  GriffinFold=0;
  // G4cout<<"+++++ Begin of event "<<evt->GetEventID()<<G4endl;

  Ap=theProjectile->getA();
  Zp=theProjectile->getZ();
  Ar=theRecoil->getA();
  Zr=theRecoil->getZ();
  // printf("Ap %d Zp %d Ar %d Zr %d\n",Ap,Zp,Ar,Zr);
  // printf("CsI threshold is %f MeV\n",CsIThreshold);
  // getc(stdin);
 }
 
void EventAction::EndOfEventAction(const G4Event* evt)
{
  // G4cout<<"+++++ End of event "<<evt->GetEventID()<<G4endl;

  G4int evtNb=evt->GetEventID();

  if (evtNb%100 == 0) 
    {
      gettimeofday(&tc,NULL);
      ts=run_action->GetStartTime();
      timersub(&tc,&ts,&td);
      t=td.tv_sec;
      rate=(float)evtNb/t;
      G4cout<<" Number of processed events "<<std::setw(9)<<evtNb<<" in "<<std::setw(9)<<t<<" sec. at "<<std::fixed<<std::setw(9)<<std::setprecision(2)<<rate<<" events per second\r"<<std::flush;

  }

  eventTrigger=0;

  /* 
     possible nasty memory issue 
     replace with evt->GetHCofThisEvent()
  */
  // G4HCofThisEvent * HCE = evt->GetHCofThisEvent();
  G4int Np;

/* 
     ASC notes 27 Aug 2015:
     Bitwise OR assignment x|=y -> x = x|y. 
     eventTrigger 8 is CsI system trigger i.e. trigger on recoil OR projectile
*/  

  // CsI trigger
  if(evt->GetHCofThisEvent()!=NULL) 
    {  
      // overwrites each time if hit collection exists
      // rather than allocating new memory
      CsI=(TrackerCsIHitsCollection*)(evt->GetHCofThisEvent()->GetHC(CsICollectionID)); 
      HI=(TrackerIonHitsCollection*)(evt->GetHCofThisEvent()->GetHC(ionCollectionID));
      Np=CsI->entries();
      if(Np>0) 
	{
	  G4double rECsI=0;
	  G4double pECsI=0.;
	  for(int i=0;i<Np;i++)
	    {
	      // recoil
		if((*CsI)[i]->GetA()==Ar)
		  if((*CsI)[i]->GetZ()==Zr)
		    {
		      rECsI+=(*CsI)[i]->GetEdep()/MeV;
		      // printf("recoil CsI partial energy deposit is %9.3f MeV\n",rECsI);
		    }
	      // projectile
		if((*CsI)[i]->GetA()==Ap)
		  if((*CsI)[i]->GetZ()==Zp)
		    {
		      pECsI+=(*CsI)[i]->GetEdep()/MeV;
		      // printf("proj   CsI partial energy deposit is %9.3f MeV\n",pECsI);
		    }
	    }
	  
	  if(rECsI>CsIThreshold)
	    eventTrigger|=2;

	  if(pECsI>CsIThreshold)
	    eventTrigger|=4;

	  if((eventTrigger&2)||(eventTrigger&4))
	    eventTrigger|=8;
	  
	  // printf("recoil CsI total energy deposit is %9.3f\n",rECsI);
	  // printf("proj   CsI total energy deposit is %9.3f\n",pECsI);
	  // printf("CsI eventTrigger is %d\n",eventTrigger);
	  // getc(stdin);
	}   
      // end CsI trigger
  
      // HPGe trigger
      // now inside the evt->GetHCofThisEvent() check - ASC 12 Feb 2016
      if(GriffinFold>0)
	      eventTrigger|=1;
      // end HPGe trigger
      
      // printf("HPGe fold is %d\n",GriffinFold);
      // printf("HPGe eventTrigger is %d\n",eventTrigger);
       //printf("eventTrigger is %d setTrigger is %d\n",eventTrigger,setTrigger);
       //getc(stdin);
    }
  
  if((eventTrigger&setTrigger)==setTrigger)
  // if(eventTrigger==11) // explicit recoil only trigger
      {
	if(GriffinFold>0)
	  for(G4int det=0;det<16;det++)
	    for(G4int cry=0;cry<4;cry++)
	      if( GriffinCrystEnergyDet[det][cry]>0)
		GriffinCrystPosDet[det][cry]/=GriffinCrystEnergyDet[det][cry];
	results->FillTree(evtNb,HI,CsI,GriffinCrystWeightDet,GriffinCrystEnergyDet,GriffinCrystPosDet);

	// print good event info
	// printf("trigger %d & %d = %d \n",eventTrigger,setTrigger,eventTrigger&setTrigger);
	// printf("event fulfills trigger conditions %d\n",eventTrigger);
	// getc(stdin);
      }
  // getc(stdin);
}

//*********************************************************************//
void EventAction::AddGriffinCrystDet(G4double de, G4double w, G4ThreeVector pos, G4int det, G4int cry) 
{
  //printf("pos %d cry %d de %f\n",det,cry,de);
  if(GriffinCrystWeightDet[det][cry]==0.)
    {
      GriffinCrystWeightDet[det][cry]=w;
      // printf("AddGriffinCrystDet weight is %f\n",GriffinCrystWeightDet[det][cry]);
      // getc(stdin);
      GriffinCrystEnergyDet[det][cry]=de;
      GriffinCrystPosDet[det][cry]=de*pos;
      GriffinFold++;
      return;
    }

  if(GriffinCrystWeightDet[det][cry]==w)
    {
      GriffinCrystEnergyDet[det][cry]+=de;
      GriffinCrystPosDet[det][cry]+= de*pos;
    }
  else
    {
      G4cout<<G4endl;
      G4cout<<" Bad weight: detector "<<det<<" crystal "<<cry<<" weight "<<w<<" old weight "<<GriffinCrystWeightDet[det][cry]<<G4endl;
      G4cout<<" Terminating execution "<<G4endl;
      G4cout<<" Find the reason, make a fix "<<G4endl;
      G4cout<<" Resist a temptation to patch or swipe under a carpet "<<G4endl;
      G4cout<<" May the force be with you, press ENTER "<<G4endl;
      getc(stdin);
      exit(0);
    }
}
