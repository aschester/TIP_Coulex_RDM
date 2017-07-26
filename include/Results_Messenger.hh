#ifndef Results_Messenger_h
#define Results_Messenger_h 1

#include "Results.hh"
#include "globals.hh"
#include "G4UImessenger.hh"
#include "G4UIdirectory.hh"
#include "G4UIcmdWithoutParameter.hh"
#include "G4UIcmdWithAnInteger.hh"
#include "G4UIcmdWith3Vector.hh"

class Results_Messenger: public G4UImessenger
{
public:
  Results_Messenger(Results*);
  ~Results_Messenger();
  
  void SetNewValue(G4UIcommand*, G4String);
  
private:
  Results* results; 
  
  G4UIdirectory*             ResDir;
  G4UIdirectory*             TreDir;
  G4UIdirectory*             ProjDir;
  G4UIdirectory*             RecDir;
  G4UIdirectory*             CSDir;
  G4UIdirectory*             GDir;
  G4UIdirectory*             PDir;
  G4UIdirectory*             GRDir;
  G4UIdirectory*             SPDir;

  
  // tree
  G4UIcmdWithoutParameter*   TRepCmd;
  G4UIcmdWithoutParameter*   TVCmd;
  G4UIcmdWithoutParameter*   TCCmd;
  G4UIcmdWithAString*        TSCmd;
  
  // projectile
  G4UIcmdWithoutParameter*   PLSACmd;
  
  // recoil
  G4UIcmdWithoutParameter*   RLSACmd; 
  
  // reaction
  G4UIcmdWithoutParameter*   CSZCmd; 
  G4UIcmdWithoutParameter*   FCMCmd;
  G4UIcmdWithoutParameter*   DCACmd;
  
  // gamma
  G4UIcmdWithAnInteger*      GRSCmd;
  G4UIcmdWithAnInteger*      GRSABCmd;
  G4UIcmdWith3Vector*        FWHMCmd;
  G4UIcmdWith3Vector*        DCPCmd;
  G4UIcmdWith3Vector*        DRPRCmd;
  
  // CsI
  G4UIcmdWithAnInteger*      PRESCmd;
  G4UIcmdWithAnInteger*      PDESCmd;
  G4UIcmdWithAnInteger*      PRLYSCmd;
  G4UIcmdWithAnInteger*      PDLYSCmd;
  G4UIcmdWithoutParameter*   PRFCmd;
  G4UIcmdWith3Vector*        SLYCmd;
  
  // groups
  G4UIcmdWithoutParameter*   RCSPCmd;
  G4UIcmdWithoutParameter*   RRSPCmd;
  G4UIcmdWithoutParameter*   PCSPCmd;
  G4UIcmdWithoutParameter*   PRSPCmd;
  G4UIcmdWithoutParameter*   RCSDCmd;
  G4UIcmdWithoutParameter*   CDFCmd;
};


#endif
