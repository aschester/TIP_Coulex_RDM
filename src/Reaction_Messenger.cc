#include "Reaction_Messenger.hh"


Reaction_Messenger::Reaction_Messenger(Reaction* React)
  :aReaction(React)
{ 
  ReactionDir = new G4UIdirectory("/Reaction/");
  ReactionDir->SetGuidance("Reaction control.");

  SetDCminCmd = new G4UIcmdWithADouble("/Reaction/SetDCmin",this);
  SetDCminCmd->SetGuidance("Set minimum distance of closest approach in fm.");
  SetDCminCmd->SetParameterName("choice",false);
  SetDCminCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

  RepDCminCmd = new G4UIcmdWithoutParameter("/Reaction/ReportDCmin",this);
  RepDCminCmd->SetGuidance("Report minimum distance of closest approach in fm");  
}



Reaction_Messenger::~Reaction_Messenger()
{
  delete SetDCminCmd;
  delete RepDCminCmd;
}


void Reaction_Messenger::SetNewValue(G4UIcommand* command,G4String newValue)
{ 
 if( command == SetDCminCmd )
   { aReaction->SetDCmin(SetDCminCmd->GetNewDoubleValue(newValue));} 

  if( command == RepDCminCmd )
   { aReaction->ReportDCmin();}
}

