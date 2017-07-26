#include "EventAction_Messenger.hh"


EventAction_Messenger::EventAction_Messenger(EventAction* Chamb)
  :aEventAction(Chamb)
{ 
 
  EventActionDir = new G4UIdirectory("/Trigger/");
  EventActionDir->SetGuidance("Trigger control.");

  NTCmd = new G4UIcmdWithoutParameter("/Trigger/NoTrigger",this);
  NTCmd->SetGuidance("No trigger selected (all events stored).");  
 
  GSCmd = new G4UIcmdWithoutParameter("/Trigger/GammaSingles",this);
  GSCmd->SetGuidance("Select gamma singles events");  

  PSCmd = new G4UIcmdWithoutParameter("/Trigger/CsISingles",this);
  PSCmd->SetGuidance("Select CsI singles events"); 

  PSRCmd = new G4UIcmdWithoutParameter("/Trigger/CsIRecoilSingles",this);
  PSRCmd->SetGuidance("Select CsI recoil singles events");  

  PSPCmd = new G4UIcmdWithoutParameter("/Trigger/CsIProjectileSingles",this);
  PSPCmd->SetGuidance("Select CsI projectile singles events");  

  GPCCmd = new G4UIcmdWithoutParameter("/Trigger/GammaCsICoincidences",this);
  GPCCmd->SetGuidance("Select gamma-CsI coincident events"); 

  GPRCCmd = new G4UIcmdWithoutParameter("/Trigger/GammaCsIRecoilCoincidences",this);
  GPRCCmd->SetGuidance("Select gamma-CsI recoil coincident events");

  GPPCCmd = new G4UIcmdWithoutParameter("/Trigger/GammaCsIProjectileCoincidences",this);
  GPPCCmd->SetGuidance("Select gamma-CsI projectile coincident events");

  CsIThCmd = new G4UIcmdWithADoubleAndUnit("/Trigger/CsIThreshold",this);
  CsIThCmd->SetGuidance("Set the CsI trigger threshold");
  CsIThCmd->SetParameterName("CsI thresh",false);
  CsIThCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

}



EventAction_Messenger::~EventAction_Messenger()
{

  delete EventActionDir;
  delete NTCmd;
  delete GSCmd;
  delete PSCmd;
  delete PSRCmd;
  delete PSPCmd;
  delete GPCCmd;
  delete GPRCCmd;
  delete GPPCCmd;
  delete CsIThCmd;
}


void EventAction_Messenger::SetNewValue(G4UIcommand* command,G4String newValue)
{ 

 // no trigger
 if( command == NTCmd )
   { aEventAction->SetNoTrigger(); }

  // gamma singles
 if( command == GSCmd )
   { aEventAction->SetTriggerGammaSing(); }
 
 // CsI singles (recoil or  projectile)
 if( command == PSCmd )
   { aEventAction->SetTriggerPinSing(); }

 // CsI recoil singles
 if( command == PSRCmd )
   { aEventAction->SetTriggerPinRecoilSing(); }

 // CsI projectile singles
 if( command == PSPCmd )
   { aEventAction->SetTriggerPinProjSing(); }
 
 // gamma-CsI (recoil or  projectile) coinc
 if( command == GPCCmd )
   { aEventAction->SetTriggerGammaPinCoinc(); }

 // gamma-CsI recoil coinc
 if( command == GPRCCmd )
   { aEventAction->SetTriggerGammaPinRecoilCoinc(); }

 // gamma-CsI projectile coinc
 if( command == GPPCCmd )
   { aEventAction->SetTriggerGammaPinProjCoinc(); }

if( command == CsIThCmd )
   { aEventAction->SetCsIThreshold(CsIThCmd->GetNewDoubleValue(newValue));} 

}

