#ifndef PhysicsList_h
#define PhysicsList_h 1

#include "G4VUserPhysicsList.hh"
#include "G4ParticleTypes.hh"
#include "G4ProcessManager.hh"

//gamma
#include "G4ComptonScattering.hh"
#include "G4GammaConversion.hh"
#include "G4PhotoElectricEffect.hh"

//e-/e+
#include "G4eBremsstrahlung.hh"
#include "G4eIonisation.hh"
#include "G4eMultipleScattering.hh"
#include "G4eplusAnnihilation.hh"

// ions
#include "G4ionIonisation.hh"
#include "G4hIonisation.hh"
#include "G4IonTable.hh"
#include "G4IonConstructor.hh"
#include "G4StepLimiter.hh"
#include "G4hMultipleScattering.hh"
//#include "G4IonParametrisedLossModel.hh" // geant default
#include "IonParametrisedLossModel.hh"     // custom
//#include "G4IonStoppingData.hh"          // geant default
#include "IonStoppingData.hh"              // custom
#include "G4IonDEDXScalingICRU73.hh"
#include "G4NuclearStopping.hh"
#include "Recoil.hh"
#include "Projectile.hh"
#include "Reaction.hh"
#include "Reaction_Messenger.hh"

class PhysicsList: public G4VUserPhysicsList
{
  public:
  PhysicsList(Projectile*,Recoil*);
    ~PhysicsList();
  Reaction* getReaction(){return theReaction;};
  IonStoppingData* getStoppingData(){return theStoppingData;};
  
  protected:
    // Construct particle and physics process
  void ConstructParticle();
  void ConstructProcess();
  void ConstructEM();
  void SetCuts();
  
 private:
  Recoil             *theRecoil;
  Projectile         *theProjectile;
  Reaction           *theReaction;
  Reaction_Messenger *theReactionMessenger;
  IonStoppingData    *theStoppingData; // for access to custom stopping power data


};

#endif







