
#include "TrackerCsISD.hh"


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

TrackerCsISD::TrackerCsISD(G4String name)
:G4VSensitiveDetector(name)
{
  G4String HCname;
  collectionName.insert(HCname="CsICollection");
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

TrackerCsISD::~TrackerCsISD(){ }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void TrackerCsISD::Initialize(G4HCofThisEvent*)
{


  CsICollection = new TrackerCsIHitsCollection
                          (SensitiveDetectorName,collectionName[0]); 
 
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4bool TrackerCsISD::ProcessHits(G4Step* aStep,G4TouchableHistory*)
{
  G4double DE = - aStep->GetDeltaEnergy();
  if(DE<0.001*eV) return false;

  G4Track* theTrack;
  theTrack=aStep->GetTrack();
  const G4DynamicParticle* aParticle= theTrack->GetDynamicParticle();
  const G4String type =  aParticle->GetDefinition()->GetParticleType();
  const G4double len=aStep->GetStepLength();

  if(type=="nucleus")
    {
    
      char name[132],s1[10],s2[10],s3[10];
      strcpy(name,aStep->GetPostStepPoint()->GetTouchable()->GetVolume()->GetName()); 
      sscanf(name,"%s %s %s",s1,s2,s3);

      G4StepPoint* vi;
      G4StepPoint* vf;

      vi=aStep->GetPreStepPoint(); 
      vf=aStep->GetPostStepPoint(); 

      // printf("vi.X %f vi.Y %f vi.Z %f\n",vi->GetPosition().getX()/um,vi->GetPosition().getY()/um,vi->GetPosition().getZ()/um);
      // printf("vf.X %f vf.Y %f vf.Z %f\n",vf->GetPosition().getX()/um,vf->GetPosition().getY()/um,vf->GetPosition().getZ()/um);

      if(strcmp(s1,"CsI")==0)
	{
	  if(len>0.)
	    {
	      TrackerCsIHit* newHitI = new TrackerCsIHit();
	      
	      newHitI->SetBeta(vi->GetBeta());
	      newHitI->SetKE(vi->GetKineticEnergy());
	      newHitI->SetEdep(aStep->GetTotalEnergyDeposit());
	      newHitI->SetPos(vi->GetPosition());
	      newHitI->SetMom(vi->GetMomentum());
	      newHitI->SetA(aParticle->GetParticleDefinition()->GetAtomicMass());
	      newHitI->SetZ(aParticle->GetParticleDefinition()->GetAtomicNumber());
	      newHitI->SetWeight(aStep->GetTrack()->GetWeight());
	      newHitI->SetId(atoi(s2));
	      newHitI->SetRingId(atoi(s3));
	      newHitI->SetPathLength(len);
	      CsICollection->insert( newHitI );
	      newHitI->Draw();
	      // printf("newHitI len %f\n",len/um);
	      // newHitI->Print(); 
	      // getc(stdin);
	    }
	  G4TrackStatus TrackStatus;
	  TrackStatus=aStep->GetTrack()->GetTrackStatus();
	  if((TrackStatus==fStopButAlive) || (TrackStatus==fStopAndKill))
	    {
	      TrackerCsIHit* newHitF = new TrackerCsIHit();
	      newHitF->SetBeta(vf->GetBeta());
	      newHitF->SetKE(vf->GetKineticEnergy());
	      newHitF->SetEdep(aStep->GetTotalEnergyDeposit());
	      newHitF->SetMom(vf->GetMomentum());
	      newHitF->SetPos(vf->GetPosition());
	      newHitF->SetA(aParticle->GetParticleDefinition()->GetAtomicMass());
	      newHitF->SetZ(aParticle->GetParticleDefinition()->GetAtomicNumber());
	      newHitF->SetWeight(aStep->GetTrack()->GetWeight());
	      newHitF->SetId(atoi(s2));
	      newHitF->SetRingId(atoi(s3));
	      newHitF->SetPathLength(len); // energy deposit = 0 at this step - how to handle w/o path == 0?
	      CsICollection->insert( newHitF );
	      newHitF->Draw();
	      // printf("newHitF\n");
	      // newHitF->Print(); 
	      // getc(stdin);
	    }

	    return true;
	}
      else
	{
	  //    G4cout << "Energy deposit in the " << name << G4endl;
	  //    G4cout << "E="<<G4BestUnit(edep,"Energy")<<G4endl;
	  //    G4cout <<"Event ignored" <<G4endl;
	  //	    getc(stdin);
	  return false;
	}
    }
  return false;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void TrackerCsISD::EndOfEvent(G4HCofThisEvent* HCE)
{
   G4int i;
   G4int NbHits = CsICollection->entries();

        if (NbHits>0) 
	  if (false) 
	   { 
	   
	   G4cout << "\n-------->Hits Collection: in this event they are " << NbHits 
            << " hits for CsI tracking: " << G4endl;
	   for (i=0;i<NbHits;i++) (*CsICollection)[i]->Print();
	   //	   	    getc(stdin);
	   }


  static G4int HCID = -1;
  if(HCID<0)
  { HCID = G4SDManager::GetSDMpointer()->GetCollectionID(collectionName[0]); }
  HCE->AddHitsCollection( HCID, CsICollection ); 
 }


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

