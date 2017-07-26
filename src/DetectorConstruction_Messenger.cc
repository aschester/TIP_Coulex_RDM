#include "DetectorConstruction_Messenger.hh"

DetectorConstruction_Messenger::DetectorConstruction_Messenger(DetectorConstruction* Det)
  :theDetector(Det)
{ 
  // directories 
  DetectorDir = new G4UIdirectory("/DetectorConstruction/");
  DetectorDir->SetGuidance("Detector construction control.");

  // commands
  RepCmd = new G4UIcmdWithoutParameter("/DetectorConstruction/Report",this);
  RepCmd->SetGuidance("Report TIP construction parameters");

  STCmd = new G4UIcmdWithADoubleAndUnit("/DetectorConstruction/SetZ_TIP",this);
  STCmd->SetGuidance("Set the position of the TIP chamber along the z (beam) axis");
  STCmd->SetParameterName("choice",false);
  STCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

  SPCmd = new G4UIcmdWithADoubleAndUnit("/DetectorConstruction/ShiftPlunger",this);
  SPCmd->SetGuidance("Shift the plunger along the z (beam) axis wrt chamber center");
  SPCmd->SetParameterName("choice",false);
  SPCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
}


DetectorConstruction_Messenger::~DetectorConstruction_Messenger()
{
  // directories
  delete DetectorDir;

  // commands
  delete RepCmd;
  delete STCmd;
  delete SPCmd;
}


void DetectorConstruction_Messenger::SetNewValue(G4UIcommand* command,G4String newValue)
{ 
  if( command == RepCmd )
    { theDetector->Report(); }

  if( command == STCmd )
    { theDetector->SetPosZ_TIP(STCmd->GetNewDoubleValue(newValue)); }

  if( command == SPCmd )
    { theDetector->ShiftPlunger(SPCmd->GetNewDoubleValue(newValue)); }
  
}

