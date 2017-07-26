#include "DetectorConstruction.hh"

/*====================================================================*/
DetectorConstruction::DetectorConstruction()
{
  // parameters to suppress:

  griffinFwdBackPosition = 11.0*cm;
  detectorRadialDistance = 11.0*cm ;
  
  // Shield Selection Default

  useTigressPositions = true;

  detectorShieldSelect = 1 ; // Include suppressors by default. 
  extensionSuppressorLocation = 0 ; // Back by default (Detector Forward)
  hevimetSelector = 1 ; // Chooses whether or not to include a hevimet (default yes)

  customDetectorNumber 		= 1 ; // det_num
  customDetectorPosition  = 1 ; // pos_num
  customDetectorVal				= 0 ; // Unused for now (Oct 2013)

 griffinMessenger = new GriffinMessenger(this);

  griffinDetectorsMapIndex = 0;
  for(G4int i = 0; i < 16; i++)
  {
      griffinDetectorsMap[i] = 0;
  }

}

/*====================================================================*/
DetectorConstruction::~DetectorConstruction()
{
  delete ExperimentalHallMessenger; 
}

/*====================================================================*/
G4VPhysicalVolume* DetectorConstruction::Construct()
{

  Materials* materials=new Materials();

//Experimental Hall

   Experimental_Hall* ExperimentalHall = new Experimental_Hall(materials);
   ExpHall_phys=ExperimentalHall->Construct();
   ExpHall_log=ExperimentalHall->GetLogVolume();
   ExperimentalHall->Report();
   ExperimentalHallMessenger = new Experimental_Hall_Messenger(ExperimentalHall);

   thePlunger = new Plunger(ExpHall_log,materials);
   thePlunger->Construct();
   thePlunger->Report();
   PlungerMessenger = new Plunger_Messenger(thePlunger);

   aCsI_array = new CsI_array(ExpHall_log,materials);
   aCsI_array->Construct();
   aCsI_array->Report();
  
   theChamber = new Chamber(ExpHall_log,materials);
   theChamber->Construct();
   theChamber->Report();
   ChamberMessenger = new Chamber_Messenger(theChamber); 
  //------------------------------------------------ 
  // Sensitive detectors
  //------------------------------------------------ 

   G4SDManager* SDman = G4SDManager::GetSDMpointer();
 
   //------------------------------------------------
   // Detectors sensitive for ion tracking
   //------------------------------------------------
   TrackerIon = new TrackerIonSD("IonTracker");
   TrackerIonSDMessenger = new TrackerIonSD_Messenger(TrackerIon);
   SDman->AddNewDetector( TrackerIon );
   thePlunger->GetBackingLog()->SetSensitiveDetector(TrackerIon);
   thePlunger->GetTargetLog()->SetSensitiveDetector(TrackerIon);
   thePlunger->GetStopperLog()->SetSensitiveDetector(TrackerIon);
   ExpHall_log->SetSensitiveDetector(TrackerIon);

   TrackerCsI = new TrackerCsISD("CsITracker");
   SDman->AddNewDetector( TrackerCsI );
   aCsI_array->MakeSensitive(TrackerCsI);
 
   return ExpHall_phys;
}

/*====================================================================*/
void DetectorConstruction::UpdateGeometry()
{
  G4RunManager::GetRunManager()->DefineWorldVolume(Construct());
}

/*====================================================================*/
// Temporary Function for testing purposes
void DetectorConstruction::AddDetectionSystemGriffinCustomDetector( G4int )
{

  G4int det_num = this->customDetectorNumber ; 
  griffinDetectorsMap[griffinDetectorsMapIndex] = this->customDetectorNumber ; 
  griffinDetectorsMapIndex++;
  
  // NOTE: ndet served no purpose in this case but I left it in just in case this needs to be modified later. The position of a detector placed using this function must be set using
  // SetDeadLayer.
 
  DetectionSystemGriffin* pGriffinCustom = new DetectionSystemGriffin( this->extensionSuppressorLocation , this->detectorShieldSelect, this->detectorRadialDistance, this->hevimetSelector ); // Select Forward (0) or Back (1)
  
  pGriffinCustom->BuildDeadLayerSpecificCrystal(this->customDetectorNumber-1);

  pGriffinCustom->PlaceDeadLayerSpecificCrystal( ExpHall_log, this->customDetectorNumber-1, this->customDetectorPosition-1, useTigressPositions ) ;

  pGriffinCustom->BuildEverythingButCrystals();

  pGriffinCustom->PlaceEverythingButCrystals( ExpHall_log, this->customDetectorNumber-1, this->customDetectorPosition-1, useTigressPositions ) ;

  for(int i=0 ; i<GS ; i++)
    {
      DetectorCrystalPosition[det_num-1][i]=pGriffinCustom->GetCrystalPosition(i);
    }
}

/*====================================================================*/
void DetectorConstruction::AddDetectionSystemGriffinCustom(G4int ndet)
{

    G4int det_num;
    G4int pos_num;

    for( det_num = 1; det_num <= ndet; det_num++ ) {
        pos_num = det_num;

        griffinDetectorsMap[griffinDetectorsMapIndex] = det_num;
        griffinDetectorsMapIndex++;

        DetectionSystemGriffin* pGriffinCustom = new DetectionSystemGriffin( this->extensionSuppressorLocation,  this->detectorShieldSelect ,  this->detectorRadialDistance, this->hevimetSelector ) ; // Select Forward (0) or Back (1)

        pGriffinCustom->BuildDeadLayerSpecificCrystal(det_num-1);
        pGriffinCustom->PlaceDeadLayerSpecificCrystal( ExpHall_log, det_num-1, pos_num-1, useTigressPositions ) ;
        pGriffinCustom->BuildEverythingButCrystals();
        pGriffinCustom->PlaceEverythingButCrystals( ExpHall_log, det_num-1, pos_num-1, useTigressPositions ) ;

	for(int i=0 ; i<GS ; i++)
	  {
	    DetectorCrystalPosition[det_num-1][i]=pGriffinCustom->GetCrystalPosition(i);
	  }
    }
}

/*====================================================================*/
void DetectorConstruction::AddDetectionSystemGriffinShieldSelect( G4int ShieldSelect ){
  this->detectorShieldSelect = ShieldSelect ; 
}

/*====================================================================*/
void DetectorConstruction::AddDetectionSystemGriffinSetRadialDistance( G4double detectorDist ){
  this->detectorRadialDistance = detectorDist ; 
}

/*====================================================================*/
void DetectorConstruction::AddDetectionSystemGriffinSetExtensionSuppLocation( G4int detectorPos ){
  this->extensionSuppressorLocation = detectorPos ; 
}

/*====================================================================*/
void DetectorConstruction::AddDetectionSystemGriffinSetDeadLayer( G4ThreeVector params )
{

  this->customDetectorNumber 		= (G4int)params.x(); // det_num
  this->customDetectorPosition  = (G4int)params.y(); // pos_num
  this->customDetectorVal			  = (G4int)params.z(); // Unused at the moment. 

}

/*====================================================================*/
void DetectorConstruction::AddDetectionSystemGriffinForward(G4int ndet)
{
//  G4double theta,phi,position;
//  G4ThreeVector move,direction;

//  DetectionSystemGriffin* pGriffinForward = new DetectionSystemGriffin(0, 1, this->griffinFwdBackPosition); // Select Forward (0) or Back (1)
//  pGriffinForward->Build();

//  for( G4int detector_number = 0; detector_number < ndet; detector_number++ )
//  {
//    direction = G4ThreeVector(sin(theta)*cos(phi),sin(theta)*sin(phi),cos(theta));
//    position = this->griffinFwdBackPosition;
//    move = position * direction;

//    G4RotationMatrix* rotate = new G4RotationMatrix; 		//rotation matrix corresponding to direction vector

//    pGriffinForward->PlaceDetector( ExpHall_log, move, rotate, detector_number ) ;
//  }

    G4int det_num;
    G4int pos_num;
    G4int config  = 0;

    for( det_num = 1; det_num <= ndet; det_num++ ) {
        pos_num = det_num;

        griffinDetectorsMap[griffinDetectorsMapIndex] = det_num;
        griffinDetectorsMapIndex++;

        DetectionSystemGriffin* pGriffinDLS = new DetectionSystemGriffin(config, 1, griffinFwdBackPosition, hevimetSelector); // Select Forward (0) or Back (1)

	// printf("Using TIGRESS positions (1=yes) %d\n",useTigressPositions);
	// getc(stdin);

        pGriffinDLS->BuildDeadLayerSpecificCrystal(det_num-1);
        pGriffinDLS->PlaceDeadLayerSpecificCrystal( ExpHall_log, det_num-1, pos_num-1, useTigressPositions ) ;
        pGriffinDLS->BuildEverythingButCrystals();
        pGriffinDLS->PlaceEverythingButCrystals( ExpHall_log, det_num-1, pos_num-1, useTigressPositions ) ;

	for(int i=0 ; i<GS ; i++)
	  {
	    DetectorCrystalPosition[det_num-1][i]=pGriffinDLS->GetCrystalPosition(i);
	  }

    }
}

/*====================================================================*/
void DetectorConstruction::AddDetectionSystemGriffinForwardDetector(G4int ndet)
{
//  G4double theta,phi,position;
//  G4ThreeVector move,direction;

//  DetectionSystemGriffin* pGriffinForward = new DetectionSystemGriffin(0, 1, this->griffinFwdBackPosition); // Select Forward (0) or Back (1)
//  pGriffinForward->Build();

//  direction = G4ThreeVector(sin(theta)*cos(phi),sin(theta)*sin(phi),cos(theta));
//  position = this->griffinFwdBackPosition;
//  move = position * direction;

//  G4RotationMatrix* rotate = new G4RotationMatrix; 		//rotation matrix corresponding to direction vector

//  pGriffinForward->PlaceDetector( ExpHall_log, move, rotate, ndet ) ;

  G4int det_num = ndet;
  G4int pos_num = ndet;
  G4int config  = 0;
  griffinDetectorsMap[griffinDetectorsMapIndex] = det_num;
  griffinDetectorsMapIndex++;


  DetectionSystemGriffin* pGriffinDLS = new DetectionSystemGriffin(config, 1, griffinFwdBackPosition, hevimetSelector ); // Select Forward (0) or Back (1)

  pGriffinDLS->BuildDeadLayerSpecificCrystal(det_num-1);
  pGriffinDLS->PlaceDeadLayerSpecificCrystal( ExpHall_log, det_num-1, pos_num-1, useTigressPositions ) ;
  pGriffinDLS->BuildEverythingButCrystals();
  pGriffinDLS->PlaceEverythingButCrystals( ExpHall_log, det_num-1, pos_num-1, useTigressPositions ) ;

  for(int i=0 ; i<GS ; i++)
    {
      DetectorCrystalPosition[det_num-1][i]=pGriffinDLS->GetCrystalPosition(i);
    }
  
}

/*====================================================================*/
void DetectorConstruction::AddDetectionSystemGriffinBack(G4int ndet)
{
//  G4double theta,phi,position;
//  G4ThreeVector move,direction;


//  DetectionSystemGriffin* pGriffinBack = new DetectionSystemGriffin(1, 1, this->griffinFwdBackPosition ) ; // Select Forward (0) or Back (1)
//  pGriffinBack->Build();

//  for(G4int detector_number = 0; detector_number < ndet; detector_number++)
//  {
//    direction = G4ThreeVector(sin(theta)*cos(phi),sin(theta)*sin(phi),cos(theta));
//    position = this->griffinFwdBackPosition;
//    move = position * direction;

//    G4RotationMatrix* rotate = new G4RotationMatrix; 		//rotation matrix corresponding to direction vector

//    pGriffinBack->PlaceDetector( ExpHall_log, move, rotate, detector_number ) ;
//  }

  G4int det_num;
  G4int pos_num;
  G4int config  = 1;

  for( det_num = 1; det_num <= ndet; det_num++ ) {
      pos_num = det_num;

      griffinDetectorsMap[griffinDetectorsMapIndex] = det_num;
      griffinDetectorsMapIndex++;

      DetectionSystemGriffin* pGriffinDLS = new DetectionSystemGriffin(config, 1, griffinFwdBackPosition, hevimetSelector ); // Select Forward (0) or Back (1)

      G4cout<<" Adding detector "<<det_num<<" for back configuration "<<G4endl;
      //   getc(stdin);
      pGriffinDLS->BuildDeadLayerSpecificCrystal(det_num-1);
      pGriffinDLS->PlaceDeadLayerSpecificCrystal( ExpHall_log, det_num-1, pos_num-1, useTigressPositions ) ;
      pGriffinDLS->BuildEverythingButCrystals();
      pGriffinDLS->PlaceEverythingButCrystals( ExpHall_log, det_num-1, pos_num-1, useTigressPositions ) ;

	for(int i=0 ; i<GS ; i++)
	  {
	    DetectorCrystalPosition[det_num-1][i]=pGriffinDLS->GetCrystalPosition(i);
	  }
  }
}

/*====================================================================*/
void DetectorConstruction::AddDetectionSystemGriffinBackDetector(G4int ndet)
{
//  G4double theta,phi,position;
//  G4ThreeVector move,direction;


//  DetectionSystemGriffin* pGriffinBack = new DetectionSystemGriffin(1, 1, this->griffinFwdBackPosition ); // Select Forward (0) or Back (1)
//  pGriffinBack->Build();

//  direction = G4ThreeVector(sin(theta)*cos(phi),sin(theta)*sin(phi),cos(theta));
//  position = this->griffinFwdBackPosition;
//  move = position * direction;

//  G4RotationMatrix* rotate = new G4RotationMatrix; 		//rotation matrix corresponding to direction vector

//  pGriffinBack->PlaceDetector( ExpHall_log, move, rotate, ndet ) ;

    G4int det_num = ndet;
    G4int pos_num = ndet;
    G4int config  = 1;

    griffinDetectorsMap[griffinDetectorsMapIndex] = det_num;
    griffinDetectorsMapIndex++;

    DetectionSystemGriffin* pGriffinDLS = new DetectionSystemGriffin(config, 1, griffinFwdBackPosition, hevimetSelector); // Select Forward (0) or Back (1)

    pGriffinDLS->BuildDeadLayerSpecificCrystal(det_num-1);
    pGriffinDLS->PlaceDeadLayerSpecificCrystal( ExpHall_log, det_num-1, pos_num-1, useTigressPositions ) ;
    pGriffinDLS->BuildEverythingButCrystals();
    pGriffinDLS->PlaceEverythingButCrystals( ExpHall_log, det_num-1, pos_num-1, useTigressPositions ) ;

    for(int i=0 ; i<GS ; i++)
      {
	DetectorCrystalPosition[det_num-1][i]=pGriffinDLS->GetCrystalPosition(i);
      }
    
}

/*====================================================================*/
void DetectorConstruction::AddDetectionSystemGriffinHevimet(G4int input)
{
  // Includes hevimet. 
  this->hevimetSelector = input ; 

}

// This will be reaplced with the addGriffinCustomDetector function. The dead layer must be set using
// the SetCustomDeadLayer command. This will take longer for many different detectors in different configurations, 
// but it is possible to place multiple custom detectors using addGriffinCustom as well. 
//void DetectorConstruction::AddDetectionSystemGriffinPositionConfig(G4ThreeVector input)
//{
//  G4int det_num = (G4int)input.x();
//  G4int pos_num = (G4int)input.y();
//  G4int config  = (G4int)input.z();


////  DetectionSystemGriffin* pGriffinBack = new DetectionSystemGriffin( config, 1, this->griffinFwdBackPosition ); // Select Forward (0) or Back (1)
////  pGriffinBack->BuildDeadLayerSpecificDetector(det_num-1);
////  pGriffinBack->PlaceDeadLayerSpecificDetector( ExpHall_log, det_num-1, pos_num-1 ) ;

//  griffinDetectorsMap[griffinDetectorsMapIndex] = det_num;
//  griffinDetectorsMapIndex++;

//  DetectionSystemGriffin* pGriffinDLS = new DetectionSystemGriffin(config, 1, this->griffinFwdBackPosition); // Select Forward (0) or Back (1)

//  pGriffinDLS->BuildDeadLayerSpecificCrystal(det_num-1);
//  pGriffinDLS->PlaceDeadLayerSpecificCrystal( ExpHall_log, det_num-1, pos_num-1 ) ;
//  pGriffinDLS->BuildEverythingButCrystals();
//  pGriffinDLS->PlaceEverythingButCrystals( ExpHall_log, det_num-1, pos_num-1 ) ;

//}

/*====================================================================*/
void DetectorConstruction::SetPosZ_TIP(G4double z)
{
  // This is a modified version of Jonathan's method

  // chamber at origin
  // reset physical volume Z position
  theChamber->SetPosZ(z); 

  // set plunger wrt to current plunger position
  // preserve shifts wrt to chamber over translation
  thePlunger->SetPosZ( thePlunger->GetPosZ() + z );

  // CsI array downstream of origin
  aCsI_array->SetPosZ( aCsI_array->GetPosZ() + z );
  
  G4RunManager::GetRunManager()->GeometryHasBeenModified();
 }

/*====================================================================*/
void DetectorConstruction::ShiftPlunger(G4double z)
{
  // shift plunger wrt chamber
  thePlunger->SetPosZ( theChamber->GetPosZ() + z );
}

/*====================================================================*/
void DetectorConstruction::Report()
{
  G4cout<<"---------- Detector Construction Report ----------"<<G4endl;
  G4cout<<"Plunger Report"<<G4endl;
  G4cout<<"-----------------"<<G4endl;
  thePlunger->Report();
  G4cout<<"CsI array Report"<<G4endl;
  G4cout<<"-----------------"<<G4endl;
  aCsI_array->Report();
  G4cout<<"Chamber Report"<<G4endl;
  G4cout<<"-----------------"<<G4endl;
  theChamber->Report();
  G4cout<<"---------- End Detector Construction Report ----------"<<G4endl;
}
