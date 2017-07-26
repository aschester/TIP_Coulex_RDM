#include "Results_Messenger.hh"

Results_Messenger::Results_Messenger(Results* Res) :results(Res) 
{ 

  ResDir = new G4UIdirectory("/Results/");
  ResDir->SetGuidance("Output control.");
  TreDir = new G4UIdirectory("/Results/Tree/");
  TreDir->SetGuidance("Root tree control.");
  ProjDir = new G4UIdirectory("/Results/Projectile/");
  ProjDir->SetGuidance("Projectile spectra");
  RecDir = new G4UIdirectory("/Results/Recoil/");
  RecDir->SetGuidance("Recoil spectra");
  CSDir = new G4UIdirectory("/Results/ReactionInfo/");
  CSDir->SetGuidance("Reaction-related spectra");
  GDir = new G4UIdirectory("/Results/Gamma/");
  GDir->SetGuidance("Gamma-ray spectra");
  PDir = new G4UIdirectory("/Results/CsI/");
  PDir->SetGuidance("CsI diode spectra");
  GRDir = new G4UIdirectory("/Results/Groups/");
  GRDir->SetGuidance("Gamma-ray - CsI detector grouping");
  SPDir = new G4UIdirectory("/Results/StoppingPower/");
  SPDir->SetGuidance("Stopping Power information");

  //this is for the tree
  TRepCmd = new G4UIcmdWithoutParameter("/Results/Tree/Report",this);
  TRepCmd->SetGuidance("Report parameters for the Root tree");
  TVCmd = new G4UIcmdWithoutParameter("/Results/Tree/View",this);
  TVCmd->SetGuidance("View and inspect the ROOT tree of simulated parameters");  
  TCCmd = new G4UIcmdWithoutParameter("/Results/Tree/Clear",this);
  TCCmd->SetGuidance("Clears the ROOT tree of simulated parameters");  
  TSCmd = new G4UIcmdWithAString("/Results/Tree/Save",this);
  TSCmd->SetGuidance("Save the ROOT tree of simulated parameters");
  TSCmd->SetParameterName("file name",false);
  TSCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
  TSCmd->SetDefaultValue("test.root");

  //this is for the projectile spectra
  PLSACmd = new G4UIcmdWithoutParameter("/Results/Projectile/LabScatteringAngle",this);
  PLSACmd->SetGuidance("Examine projectile scattering angle in the lab frame");

 //this is for the recoil spectra
  RLSACmd = new G4UIcmdWithoutParameter("/Results/Recoil/LabScatteringAngle",this);
  RLSACmd->SetGuidance("Examine recoil scattering angle in the lab frame");

 //this is for the reaction
  CSZCmd = new G4UIcmdWithoutParameter("/Results/ReactionInfo/ZProfile",this);
  CSZCmd->SetGuidance("Cross section vs. z profile");
  FCMCmd = new G4UIcmdWithoutParameter("/Results/ReactionInfo/FCM",this);
  FCMCmd->SetGuidance("Centre of mass f(theta) function for the gun conditions");
  DCACmd = new G4UIcmdWithoutParameter("/Results/ReactionInfo/DistCloseApproach",this);
  DCACmd->SetGuidance("Distance of closest approach for the reaction");

  //this is for gamma-ray spectra
  GRSCmd = new G4UIcmdWithAnInteger("/Results/Gamma/RingSpectrum",this);
  GRSCmd->SetGuidance("Gamma-ray spectrum for a selected ring");  
  GRSCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
  GRSCmd->SetDefaultValue(1);

  GRSABCmd = new G4UIcmdWithAnInteger("/Results/Gamma/ABRingSpectrum",this);
  GRSABCmd->SetGuidance("Gamma-ray add-back spectrum for a selected ring");  
  GRSABCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
  GRSABCmd->SetDefaultValue(1);

  FWHMCmd = new G4UIcmdWith3Vector("/Results/Gamma/FWHM",this);
  FWHMCmd->SetGuidance("FWHM F, G, and H for gamma-ray spectra");  
  FWHMCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
  G4ThreeVector fwhm;
  fwhm.setX(1.25);fwhm.setY(2.00);fwhm.setZ(0.00);
  FWHMCmd->SetDefaultValue(fwhm);
  FWHMCmd->SetParameterName("F","G","H",true,false);

  DCPCmd = new G4UIcmdWith3Vector("/Results/Gamma/DetCryCsISpectrum",this);
  DCPCmd->SetGuidance("Gamma-ray spectrum for a selected detector, crystal and CsI");  
  DCPCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
  G4ThreeVector dcp;
  dcp.setX(1);dcp.setY(0);dcp.setZ(1);
  DCPCmd->SetDefaultValue(dcp);
  DCPCmd->SetParameterName("Detector Id","Crystal Id","CsI Id",true,false);

  DRPRCmd = new G4UIcmdWith3Vector("/Results/Gamma/DetRingCsIRingSpectrum",this);
  DRPRCmd->SetGuidance("Gamma-ray spectrum for a selected detector and CsI rings");  
  DRPRCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
  G4ThreeVector drpr;
  drpr.setX(1);drpr.setY(1);drpr.setZ(0);
  DRPRCmd->SetDefaultValue(drpr);
  DRPRCmd->SetParameterName("Detector Ring","CsI Ring","Correlation Coefficient",true,false);

  //this is for CsI diode spectra
  PRESCmd = new G4UIcmdWithAnInteger("/Results/CsI/RingEnergySpectrum",this);
  PRESCmd->SetGuidance("CsI-diode energy spectrum for a selected ring");  
  PRESCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
  PRESCmd->SetDefaultValue(1);

  PDESCmd = new G4UIcmdWithAnInteger("/Results/CsI/DetectorEnergySpectrum",this);
  PDESCmd->SetGuidance("CsI-diode energy spectrum for a selected detector");  
  PDESCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
  PDESCmd->SetDefaultValue(1);

  PRLYSCmd = new G4UIcmdWithAnInteger("/Results/CsI/RingLYSpectrum",this);
  PRLYSCmd->SetGuidance("CsI-diode light yield spectrum for a selected ring");  
  PRLYSCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
  PRLYSCmd->SetDefaultValue(1);

  PDLYSCmd = new G4UIcmdWithAnInteger("/Results/CsI/DetectorLYSpectrum",this);
  PDLYSCmd->SetGuidance("CsI-diode light yield spectrum for a selected detector");  
  PDLYSCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
  PDLYSCmd->SetDefaultValue(1);

  PRFCmd = new G4UIcmdWithoutParameter("/Results/CsI/Fold",this);
  PRFCmd->SetGuidance("CsI fold histogram");

  SLYCmd = new G4UIcmdWith3Vector("/Results/CsI/SetLYResponse",this);
  SLYCmd->SetGuidance("Set Birk's constant and LY scaling factor for CsI detectors");  
  SLYCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
  G4ThreeVector ly;
  ly.setX(-1);ly.setY(1.5);ly.setZ(1000);
  SLYCmd->SetDefaultValue(ly);
  SLYCmd->SetParameterName("CsI detector","kB [um/MeV]","S",true,false);

  //this is for groups
  RCSPCmd = new G4UIcmdWithoutParameter("/Results/Groups/CalculateCrystalPositions",this);
  RCSPCmd->SetGuidance("Calculates position of TIGRESS crystals");
  RRSPCmd = new G4UIcmdWithoutParameter("/Results/Groups/ReportCrystalPositions",this);
  RRSPCmd->SetGuidance("Reports position of TIGRESS crystals");
  PCSPCmd = new G4UIcmdWithoutParameter("/Results/Groups/CalculateCsIPositions",this);
  PCSPCmd->SetGuidance("Calculates position of CsI diodes");
  PRSPCmd = new G4UIcmdWithoutParameter("/Results/Groups/ReportCsIPositions",this);
  PRSPCmd->SetGuidance("Reports position of CsI diodes");
  RCSDCmd = new G4UIcmdWithoutParameter("/Results/Groups/CosineDistribution",this);
  RCSDCmd->SetGuidance("Histogram of cosines between CsI and TIGRESS crystals");
  CDFCmd = new G4UIcmdWithoutParameter("/Results/Groups/SaveDopplerFactorData",this);
  CDFCmd->SetGuidance("Save histogram and text file of doppler shift factors");
}

Results_Messenger::~Results_Messenger()
{

 //  // this is for the directories
  delete ResDir;
  delete TreDir;
  delete ProjDir;
  delete RecDir;
  delete CSDir;
  delete GDir;
  delete PDir;
  delete GRDir;
  delete SPDir;

  // this is for the tree access
    delete TRepCmd;
  delete TVCmd;
  delete TCCmd;
  delete TSCmd;
 
  //this is for the projectile spectra
   delete PLSACmd;

 //this is for the recoil spectra
  delete RLSACmd;

 //  //this is for the reaction
  delete CSZCmd;
  delete FCMCmd;
  delete DCACmd;

  //this is for gamma-ray spectrum
  delete GRSCmd;
  delete GRSABCmd;
  delete FWHMCmd;
  delete DCPCmd;
  delete DRPRCmd;

 //this is for CsI-diode spectrum
  delete PRESCmd;
  delete PDESCmd;
  delete PRLYSCmd;
  delete PDLYSCmd;
  delete PRFCmd;
  delete SLYCmd;

  //this is for the groups
  delete RCSPCmd;
  delete RRSPCmd;
  delete PCSPCmd;
  delete PRSPCmd;
  delete RCSDCmd;
  delete CDFCmd;
}

void Results_Messenger::SetNewValue(G4UIcommand* command,G4String newValue)
{ 
  // this is for the tree access
 if( command == TRepCmd )
    { results->TreeReport(); }
 if( command == TVCmd )
    { results->TreeView(); }
 if( command == TCCmd )
    { results->TreeClear(); }
  if( command == TSCmd )
    { results->TreeSave(newValue); }
 
 //this is for the projectile spectra
 if( command == PLSACmd )
    { results->ProjLabScattAngle(); }

 //this is for the recoil spectra
 if( command == RLSACmd )
    { results->RecLabScattAngle(); }

 //this is for the reaction
 if( command == CSZCmd )
    { results->CrossSectionZProfile(); }
 if( command == FCMCmd )
   { results->TargetFaceF_CM(); }
 if( command == DCACmd )
   { results->PlotDistCloseApproach(); }

 //this is for gamma-ray spectra
 if( command == GRSCmd )
   { results->GammaRingSpectrum(GRSCmd->GetNewIntValue(newValue)); }
 if( command == GRSABCmd )
   { results->GammaABRingSpectrum(GRSCmd->GetNewIntValue(newValue)); }
 if( command == FWHMCmd )
   { 
     G4ThreeVector fwhm=FWHMCmd->GetNew3VectorValue(newValue);
     G4double f=fwhm.getX();
     G4double g=fwhm.getY();
     G4double h=fwhm.getZ();
     results->SetFWHM_response(f,g,h);
   }
 if( command == DCPCmd )
   { 
     G4ThreeVector dcp=DCPCmd->GetNew3VectorValue(newValue);
     G4int det=(G4int)dcp.getX();
     G4int seg=(G4int)dcp.getY();
     G4int pin=(G4int)dcp.getZ();
     results->DetCryCsIGammaSpectrum(det,seg,pin);
   }
 if( command == DRPRCmd )
   { 
     G4ThreeVector drpr=DRPRCmd->GetNew3VectorValue(newValue);
     G4int det=(G4int)drpr.getX();
     G4int pin=(G4int)drpr.getY();
     G4double coeff=drpr.getZ();
     results->DetRingCsIRingGammaSpectrum(det,pin,coeff);
   }

 //this is for CsI-dide spectra
 if( command == PRESCmd )
   { results->CsIRingEnergySpectrum(PRESCmd->GetNewIntValue(newValue)); }
 if( command == PDESCmd )
   { results->CsIDetectorEnergySpectrum(PDESCmd->GetNewIntValue(newValue)); }
 if( command == PRLYSCmd )
   { results->CsIRingLYSpectrum(PRLYSCmd->GetNewIntValue(newValue)); }
if( command == PDLYSCmd )
   { results->CsIDetectorLYSpectrum(PDLYSCmd->GetNewIntValue(newValue)); }
 if( command == PRFCmd )
    { results->CsIFold(); }
 if( command == SLYCmd )
   { 
     G4ThreeVector ly=SLYCmd->GetNew3VectorValue(newValue);
     G4int pos=(G4int)ly.getX();
     G4double kB=ly.getY();
     G4double S=ly.getZ();
     results->SetCsILYResponse(pos,kB,S);
   }

 //this is for the groups
 if( command == RCSPCmd )
   { results->CalculateCrystalPositions(); }
 if( command == RRSPCmd )
   { results->ReportCrystalPositions(); }
 if( command == PCSPCmd )
   { results->CalculateCsIPositions(); }
 if( command == PRSPCmd )
   { results->ReportCsIPositions(); }
 if( command == RCSDCmd )
   { results->GroupCosDist(); }
if( command == CDFCmd )
   { results->SaveDopplerFactorData(); }
}
