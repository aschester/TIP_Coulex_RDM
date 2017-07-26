// Includes Physical Constants and System of Units
#include "G4PhysicalConstants.hh"
#include "G4SystemOfUnits.hh"

#include "DetectorConstruction.hh"
#include "GriffinMessenger.hh"

#include "G4Material.hh"

#include "G4Tubs.hh"
#include "G4Box.hh"
#include "G4Sphere.hh"
#include "G4Para.hh"
#include "G4Cons.hh"
#include "G4Tubs.hh"
#include "G4Trd.hh"
#include "G4Trap.hh"
#include "G4SubtractionSolid.hh"
#include "G4UnionSolid.hh"
#include "G4IntersectionSolid.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"

#include "G4GeometryManager.hh"
#include "G4PhysicalVolumeStore.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4SolidStore.hh"
#include "G4AssemblyVolume.hh"

#include "G4ThreeVector.hh"
#include "G4RotationMatrix.hh"

#include "G4SDManager.hh"

#include "G4VisAttributes.hh"
#include "G4Colour.hh"

#include "DetectionSystemGriffin.hh"

// #include "DetectionSystem8pi.hh"
// #include "Apparatus8piVacuumChamber.hh"
// #include "Apparatus8piVacuumChamberAuxMatShell.hh"
// #include "ApparatusGriffinStructure.hh"

// #include "G4SystemOfUnits.hh" // new version geant4.10 requires units


///////////////////////////////////////////////////////////////////////
// The ::DetectionSystemGriffin constructor instatiates all the
// Logical and Physical Volumes used in the detector geometery, and the
// ::~DetectionSystemGriffin destructor deletes them from the stack
// when they go out of scope
///////////////////////////////////////////////////////////////////////
DetectionSystemGriffin::DetectionSystemGriffin( G4int sel, G4int suppSwitch, G4double detRad, G4int hevimetSel ):
    ///////////////////////////////////////////////////////////////////
    // LogicalVolumes
    ///////////////////////////////////////////////////////////////////

   // LogicalVolumes in ConstructBasicDetectorBlock
    germanium_block_log(0),

    // LogicalVolumes in ConstructComplexDetectorBlock
    germanium_block1_log(0), germanium_hole_log(0), inner_dead_layer_log(0),
    inner_dead_layer_cap_log(0), outer_dead_layer_log(0), inter_crystal_electrodeMat_back_log(0),
    inter_crystal_electrodeMat_front_log(0),

    // LogicalVolumes in ConstructBGOCasing
    back_BGO_log(0), BGO_casing_log(0),

    // LogicalVolumes in ConstructNewSuppressorCasing
    back_quarter_suppressor_shell_log(0),
    right_suppressor_shell_log(0),
    left_suppressor_shell_log(0),
    right_suppressor_shell_extension_log(0),
    left_suppressor_shell_extension_log(0),
    back_quarter_suppressor_log(0), right_suppressor_log(0),
    left_suppressor_log(0), right_suppressor_extension_log(0),
    left_suppressor_extension_log(0),

    // LogicalVolumes in ConstructDetector
    front_face_log(0), right_bent_piece_log(0), left_bent_piece_log(0),
    top_bent_piece_log(0), bottom_bent_piece_log(0), right_wedge_log(0),
    left_wedge_log(0), top_wedge_log(0), bottom_wedge_log(0),
    upper_right_cone_log(0), lower_right_cone_log(0),
    upper_left_cone_log(0), lower_left_cone_log(0),
    upper_right_tube_log(0), lower_right_tube_log(0),
    upper_left_tube_log(0), lower_left_tube_log(0),
    right_side_panel_log(0), left_side_panel_log(0),
    top_side_panel_log(0), bottom_side_panel_log(0),
    rear_plate_log(0), finger_shell_log(0), tank_log(0), tank_liquid_log(0), tank_lid1_log(0), tank_lid2_log(0), 

    // LogicalVolumes in ConstructColdFinger
    end_plate_log(0), finger_log(0), extra_cold_block_log(0),
    triangle_post_log(0), fet_air_hole_log(0), cooling_bar_log(0),
    cooling_side_block_log(0), structureMat_cold_finger_log(0),

    // LogicalVolumes in ConstructNewHeavyMet
    hevimet_log(0)
{
    G4ThreeVector my_move_null(0, 0, 1);
    this->move_null = my_move_null * 0;
    G4RotationMatrix* my_rotate_null = new G4RotationMatrix;
    this->rotate_null = my_rotate_null;

    /////////////////////////////////////////////////////////////////////
    // Coords for GRIFFIN
    // Note that the GRIFFIN lampshade angles are rotated by 45 degrees with respect to those of TIGRESS.
    // Modified coords for TIGRESS are below!
    /////////////////////////////////////////////////////////////////////
    // theta
    this->coords[0][0] 	= 45.0;
    this->coords[1][0] 	= 45.0;
    this->coords[2][0] 	= 45.0;
    this->coords[3][0] 	= 45.0;
    this->coords[4][0] 	= 90.0;
    this->coords[5][0] 	= 90.0;
    this->coords[6][0] 	= 90.0;
    this->coords[7][0] 	= 90.0;
    this->coords[8][0] 	= 90.0;
    this->coords[9][0] 	= 90.0;
    this->coords[10][0] 	= 90.0;
    this->coords[11][0] 	= 90.0;
    this->coords[12][0] 	= 135.0;
    this->coords[13][0] 	= 135.0;
    this->coords[14][0] 	= 135.0;
    this->coords[15][0] 	= 135.0;
    // phi
    this->coords[0][1] 	= 67.5;
    this->coords[1][1] 	= 157.5;
    this->coords[2][1] 	= 247.5;
    this->coords[3][1] 	= 337.5;
    this->coords[4][1] 	= 22.5;
    this->coords[5][1] 	= 67.5;
    this->coords[6][1] 	= 112.5;
    this->coords[7][1] 	= 157.5;
    this->coords[8][1] 	= 202.5;
    this->coords[9][1] 	= 247.5;
    this->coords[10][1] 	= 292.5;
    this->coords[11][1] 	= 337.5;
    this->coords[12][1] 	= 67.5;
    this->coords[13][1] 	= 157.5;
    this->coords[14][1] 	= 247.5;
    this->coords[15][1] 	= 337.5;
    // yaw (alpha)
    this->coords[0][2] 	= 0.0;
    this->coords[1][2] 	= 0.0;
    this->coords[2][2] 	= 0.0;
    this->coords[3][2] 	= 0.0;
    this->coords[4][2] 	= 0.0;
    this->coords[5][2] 	= 0.0;
    this->coords[6][2] 	= 0.0;
    this->coords[7][2] 	= 0.0;
    this->coords[8][2] 	= 0.0;
    this->coords[9][2] 	= 0.0;
    this->coords[10][2] 	= 0.0;
    this->coords[11][2] 	= 0.0;
    this->coords[12][2] 	= 0.0;
    this->coords[13][2] 	= 0.0;
    this->coords[14][2] 	= 0.0;
    this->coords[15][2] 	= 0.0;
    // pitch (beta)
    this->coords[0][3] 	= -45.0;
    this->coords[1][3] 	= -45.0;
    this->coords[2][3] 	= -45.0;
    this->coords[3][3] 	= -45.0;
    this->coords[4][3] 	= 0.0;
    this->coords[5][3] 	= 0.0;
    this->coords[6][3] 	= 0.0;
    this->coords[7][3] 	= 0.0;
    this->coords[8][3] 	= 0.0;
    this->coords[9][3] 	= 0.0;
    this->coords[10][3] 	= 0.0;
    this->coords[11][3] 	= 0.0;
    this->coords[12][3] 	= 45.0;
    this->coords[13][3] 	= 45.0;
    this->coords[14][3] 	= 45.0;
    this->coords[15][3] 	= 45.0;
    // roll (gamma)
    this->coords[0][4] 	= 67.5;
    this->coords[1][4] 	= 157.5;
    this->coords[2][4] 	= 247.5;
    this->coords[3][4] 	= 337.5;
    this->coords[4][4] 	= 22.5;
    this->coords[5][4] 	= 67.5;
    this->coords[6][4] 	= 112.5;
    this->coords[7][4] 	= 157.5;
    this->coords[8][4] 	= 202.5;
    this->coords[9][4] 	= 247.5;
    this->coords[10][4] 	= 292.5;
    this->coords[11][4] 	= 337.5;
    this->coords[12][4] 	= 67.5;
    this->coords[13][4] 	= 157.5;
    this->coords[14][4] 	= 247.5;
    this->coords[15][4] 	= 337.5;

    // Note that the GRIFFIN lampshade angles are rotated by 45 degrees with respect to those of TIGRESS.
    // Uncomment this for TIGRESS!
    // phi
    //  this->coords[0][1] 	= this->coords[0][1] - 45.0;
    //  this->coords[1][1] 	= this->coords[1][1] - 45.0;
    //  this->coords[2][1] 	= this->coords[2][1] - 45.0;
    //  this->coords[3][1] 	= this->coords[3][1] - 45.0;
    //  this->coords[12][1] 	= this->coords[12][1] - 45.0;
    //  this->coords[13][1] 	= this->coords[13][1] - 45.0;
    //  this->coords[14][1] 	= this->coords[14][1] - 45.0;
    //  this->coords[15][1] 	= this->coords[15][1] - 45.0;
    //  // roll (gamma)
    //  this->coords[0][4] 	= this->coords[0][4] - 45.0;
    //  this->coords[1][4] 	= this->coords[1][4] - 45.0;
    //  this->coords[2][4] 	= this->coords[2][4] - 45.0;
    //  this->coords[3][4] 	= this->coords[3][4] - 45.0;
    //  this->coords[12][4] 	= this->coords[12][4] - 45.0;
    //  this->coords[13][4] 	= this->coords[13][4] - 45.0;
    //  this->coords[14][4] 	= this->coords[14][4] - 45.0;
    //  this->coords[15][4] 	= this->coords[15][4] - 45.0;



    // Surfaces were check on June 1st 2015, no geometry overlaps!
    surfCheck = true;

    /////////////////////////////////////////////////////////////////////
    // GRIFFIN/TIGRESS Detector Properties
    /////////////////////////////////////////////////////////////////////
    this->cut_clearance = 0.01*mm;
    this->extra_cut_length = 10.0*mm;

    // Killing Suppressors!
    this->include_extension_suppressors = true;
    this->include_side_suppressors      = true;
    this->include_back_suppressors      = true;		//BM: right place to turn on suppressors?

    //////
    G4int can_pos                                = 1;
    G4int can_position_specifier                 = can_pos;  	// 1=fully forward, 2=flush with BGO, 3=fully back
    G4int detector_format                        = 2; 		// 1=simple, 2=complex, 3=none
    G4String include_detector_can                = "y"; 		// "y" or "n"
    G4String include_BGO                         = "y";
    G4String include_cold_finger                 = "y";
    ///// Included in new system

    if( suppSwitch  == 1)
    { // Include Suppressors
        this->include_extension_suppressors = true;
        this->include_side_suppressors      = true;
        this->include_back_suppressors      = true;
    }
    else if(suppSwitch  == -1)
    { // just side and back
        this->include_extension_suppressors = false;
        this->include_side_suppressors      = true;
        this->include_back_suppressors      = true;
    }
    else
    { // Dont include Suppressors
        this->include_extension_suppressors = false;
        this->include_side_suppressors      = false;
        this->include_back_suppressors      = false;
        include_BGO													= "n" ;
    }

    // GRIFFIN Dead Layers
    // Generated Oct 10th, 2013
    //  this->griffinDeadLayers[0][0] = 1.38*mm;
    //  this->griffinDeadLayers[0][1] = 1.30*mm;
    //  this->griffinDeadLayers[0][2] = 1.61*mm;
    //  this->griffinDeadLayers[0][3] = 1.84*mm;
    //  this->griffinDeadLayers[1][0] = 0.324*mm;
    //  this->griffinDeadLayers[1][1] = 0.506*mm;
    //  this->griffinDeadLayers[1][2] = 0.43*mm;
    //  this->griffinDeadLayers[1][3] = 0.40*mm;
    //  this->griffinDeadLayers[2][0] = 0.203*mm;
    //  this->griffinDeadLayers[2][1] = 0.457*mm;
    //  this->griffinDeadLayers[2][2] = 0.228*mm;
    //  this->griffinDeadLayers[2][3] = 0.614*mm;
    //  this->griffinDeadLayers[3][0] = 1.33*mm;
    //  this->griffinDeadLayers[3][1] = 1.17*mm;
    //  this->griffinDeadLayers[3][2] = 1.83*mm;
    //  this->griffinDeadLayers[3][3] = 1.38*mm;
    //  this->griffinDeadLayers[4][0] = 0.485*mm;
    //  this->griffinDeadLayers[4][1] = 0.488*mm;
    //  this->griffinDeadLayers[4][2] = 0.495*mm;
    //  this->griffinDeadLayers[4][3] = 0.508*mm;
    //  this->griffinDeadLayers[5][0] = 1.75*mm;
    //  this->griffinDeadLayers[5][1] = 1.48*mm;
    //  this->griffinDeadLayers[5][2] = 1.66*mm;
    //  this->griffinDeadLayers[5][3] = 1.47*mm;
    //  this->griffinDeadLayers[6][0] = 1.25*mm;
    //  this->griffinDeadLayers[6][1] = 1.31*mm;
    //  this->griffinDeadLayers[6][2] = 1.21*mm;
    //  this->griffinDeadLayers[6][3] = 1.35*mm;
    //  this->griffinDeadLayers[7][0] = 1.19*mm;
    //  this->griffinDeadLayers[7][1] = 2.07*mm;
    //  this->griffinDeadLayers[7][2] = 1.22*mm;
    //  this->griffinDeadLayers[7][3] = 1.39*mm;


    // Using average dead layers
    this->griffinDeadLayers[0][0] = 1.07*mm;
    this->griffinDeadLayers[0][1] = 1.07*mm;
    this->griffinDeadLayers[0][2] = 1.07*mm;
    this->griffinDeadLayers[0][3] = 1.07*mm;
    this->griffinDeadLayers[1][0] = 1.07*mm;
    this->griffinDeadLayers[1][1] = 1.07*mm;
    this->griffinDeadLayers[1][2] = 1.07*mm;
    this->griffinDeadLayers[1][3] = 1.07*mm;
    this->griffinDeadLayers[2][0] = 1.07*mm;
    this->griffinDeadLayers[2][1] = 1.07*mm;
    this->griffinDeadLayers[2][2] = 1.07*mm;
    this->griffinDeadLayers[2][3] = 1.07*mm;
    this->griffinDeadLayers[3][0] = 1.07*mm;
    this->griffinDeadLayers[3][1] = 1.07*mm;
    this->griffinDeadLayers[3][2] = 1.07*mm;
    this->griffinDeadLayers[3][3] = 1.07*mm;
    this->griffinDeadLayers[4][0] = 1.07*mm;
    this->griffinDeadLayers[4][1] = 1.07*mm;
    this->griffinDeadLayers[4][2] = 1.07*mm;
    this->griffinDeadLayers[4][3] = 1.07*mm;
    this->griffinDeadLayers[5][0] = 1.07*mm;
    this->griffinDeadLayers[5][1] = 1.07*mm;
    this->griffinDeadLayers[5][2] = 1.07*mm;
    this->griffinDeadLayers[5][3] = 1.07*mm;
    this->griffinDeadLayers[6][0] = 1.07*mm;
    this->griffinDeadLayers[6][1] = 1.07*mm;
    this->griffinDeadLayers[6][2] = 1.07*mm;
    this->griffinDeadLayers[6][3] = 1.07*mm;
    this->griffinDeadLayers[7][0] = 1.07*mm;
    this->griffinDeadLayers[7][1] = 1.07*mm;
    this->griffinDeadLayers[7][2] = 1.07*mm;
    this->griffinDeadLayers[7][3] = 1.07*mm;
    this->griffinDeadLayers[8][0] = 1.07*mm;
    this->griffinDeadLayers[8][1] = 1.07*mm;
    this->griffinDeadLayers[8][2] = 1.07*mm;
    this->griffinDeadLayers[8][3] = 1.07*mm;
    this->griffinDeadLayers[9][0] = 1.07*mm;
    this->griffinDeadLayers[9][1] = 1.07*mm;
    this->griffinDeadLayers[9][2] = 1.07*mm;
    this->griffinDeadLayers[9][3] = 1.07*mm;
    this->griffinDeadLayers[10][0] = 1.07*mm;
    this->griffinDeadLayers[10][1] = 1.07*mm;
    this->griffinDeadLayers[10][2] = 1.07*mm;
    this->griffinDeadLayers[10][3] = 1.07*mm;
    this->griffinDeadLayers[11][0] = 1.07*mm;
    this->griffinDeadLayers[11][1] = 1.07*mm;
    this->griffinDeadLayers[11][2] = 1.07*mm;
    this->griffinDeadLayers[11][3] = 1.07*mm;
    this->griffinDeadLayers[12][0] = 1.07*mm;
    this->griffinDeadLayers[12][1] = 1.07*mm;
    this->griffinDeadLayers[12][2] = 1.07*mm;
    this->griffinDeadLayers[12][3] = 1.07*mm;
    this->griffinDeadLayers[13][0] = 1.07*mm;
    this->griffinDeadLayers[13][1] = 1.07*mm;
    this->griffinDeadLayers[13][2] = 1.07*mm;
    this->griffinDeadLayers[13][3] = 1.07*mm;
    this->griffinDeadLayers[14][0] = 1.07*mm;
    this->griffinDeadLayers[14][1] = 1.07*mm;
    this->griffinDeadLayers[14][2] = 1.07*mm;
    this->griffinDeadLayers[14][3] = 1.07*mm;
    this->griffinDeadLayers[15][0] = 1.07*mm;
    this->griffinDeadLayers[15][1] = 1.07*mm;
    this->griffinDeadLayers[15][2] = 1.07*mm;
    this->griffinDeadLayers[15][3] = 1.07*mm;

    this->griffinCrystalColours[0] = G4Colour(0.0,0.0,1.0);
    this->griffinCrystalColours[1] = G4Colour(0.0,1.0,0.0);
    this->griffinCrystalColours[2] = G4Colour(1.0,0.0,0.0);
    this->griffinCrystalColours[3] = G4Colour(1.0,1.0,1.0);

    this->griffinDeadLayerColours[0] = G4Colour(0.0,0.0,0.50);
    this->griffinDeadLayerColours[1] = G4Colour(0.0,0.50,0.0);
    this->griffinDeadLayerColours[2] = G4Colour(0.50,0.0,0.0);
    this->griffinDeadLayerColours[3] = G4Colour(0.3,0.3,0.3);

    this->BGO_material                  = "G4_BGO";
    this->back_suppressor_material 	    = "G4_CESIUM_IODIDE";

    if( sel == 0 ) {
        sdName0 =               "/sd/allGriffinForward0";
        sdName1 =               "/sd/allGriffinForward1";
        sdName2 =               "/sd/allGriffinForward2";
        sdName3 =               "/sd/allGriffinForward3";
        sdName4 =               "/sd/allGriffinForward4";
        sdName5 =               "/sd/allGriffinForward5";

        colNameGe =             "CollectionGriffinForwardGe";
        colNameLeftCasing =     "CollectionGriffinForwardLeftCasing";
        colNameRightCasing =    "CollectionGriffinForwardRightCasing";
        colNameLeftExtension =  "CollectionGriffinForwardLeftExtension";
        colNameRightExtension = "CollectionGriffinForwardRightExtension";
        colNameBackPlug =       "CollectionGriffinForwardBackPlug";
    }
    else if( sel == 1 ) {
        sdName0 =               "/sd/allGriffinBack0";
        sdName1 =               "/sd/allGriffinBack1";
        sdName2 =               "/sd/allGriffinBack2";
        sdName3 =               "/sd/allGriffinBack3";
        sdName4 =               "/sd/allGriffinBack4";
        sdName5 =               "/sd/allGriffinBack5";

        colNameGe =             "CollectionGriffinBackGe";
        colNameLeftCasing =     "CollectionGriffinBackLeftCasing";
        colNameRightCasing =    "CollectionGriffinBackRightCasing";
        colNameLeftExtension =  "CollectionGriffinBackLeftExtension";
        colNameRightExtension = "CollectionGriffinBackRightExtension";
        colNameBackPlug =       "CollectionGriffinBackBackPlug";
    }
    else {
        G4cout << "Error: 123109812 " << G4endl;
        exit(1);
    }

    // These keep track of the number of sensitive detectors. Initialized to zero
    this->germanium_copy_number                  = 4000;
    this->left_suppressor_side_copy_number       = 4100;
    this->right_suppressor_side_copy_number      = 4200;
    this->left_suppressor_extension_copy_number  = 4300;
    this->right_suppressor_extension_copy_number = 4400;
    this->back_suppressor_copy_number            = 4500;

    this->suppressor_position_selector           = sel;  	// 0 = detector forward, 1 = detector back

    this->hevimet_selector                       = hevimetSel ;

    this->inner_dead_layer_thickness 	         = 1.0*mm;
    this->depth_segmentation_adjustment 	     = 0.0*cm;


    // Determine whether the structureMat shells around the suppressors will be included


    this->forward_inner_radius 		             = detRad ;
    this->back_inner_radius 		             = this->forward_inner_radius + 3.5*cm;

    this->suppressor_forward_radius              = 11.0*cm ;
    this->suppressor_back_radius                 = this->suppressor_forward_radius + 3.5*cm ;

    // "Outer" dead layer description: thickness determined from R.G.Helmer paper
    this->outer_dead_layer_thickness	         = 2.5*micrometer;

    //2.00 new: the thickness of the structureMat shell around the suppression material
    this->suppressor_shell_thickness 	         = 0.5*mm;

    //Jun 21, 2005: Epapr7.80: the suppressor extensions were accidentally made too large,
    //so they do not come as far forward as they should.  Shift for back, with hevimet was 1.0*cm
    //Jun 28, 2005: Just! for the back, no hevimet case, change this to 0.5*cm
    this->extension_accidental_back_shift        = 0.5*cm;
    //Set (in this) whether the structureMat shells around the suppressors will be included

    this->germanium_outer_radius 		         = 3.0*cm;	// Radius of the circular part of the germanium
    this->germanium_hole_radius 		         = 0.5*cm;
    this->germanium_length 		                 = 9.0*cm;
    this->germanium_hole_dist_from_face 	     = 1.5*cm;
    this->germanium_dist_from_can_face 	         = 0.55*cm;
    this->germanium_separation 		             = 0.6*mm;  	// Separation between quarter detectors: changed Jan 2005 from 0.7mm

    // This has been disabled Apr 11, 2005 for version 7.69: setting this
    this->inter_crystal_electrodeMat_thickness   = 0.6*mm;
    this->germanium_width 		                 = 56.5*mm;  	// Width of one quarter detector
    this->germanium_bent_length 		         = 3.62*cm;
    this->germanium_shift                        = 1.05*mm;  	// this can't be more than 2.75mm. It is the amount by which
    // one side is cut closer to the center than the other
    this->germanium_corner_cone_end_length       = 3.0*cm; 	// the ending length of the cones

    this->electrodeMat_starting_depth            = this->germanium_corner_cone_end_length-1*micrometer;
    this->detector_block_length                  = 2.0*this->germanium_width; 	// Obsolete
    this->detector_block_height                  = this->germanium_length -this->germanium_bent_length;
    this->detector_total_width                   = 12.4*cm;             // Width of the detector can
    this->detector_total_length                  = 16.0*cm;             // Length of the detector can
    this->bent_end_angle                         = 22.5*M_PI/180;
    this->bent_end_length                        = 4.325*cm;
    this->can_face_thickness                     = 0.15*cm;
    this->can_side_thickness                     = 0.2*cm;
    this->detector_block_trapezoidal_height      = this->germanium_bent_length;

    this->cold_finger_outer_shell_radius         = 2.5*cm;
    this->cold_finger_shell_thickness            = 0.2*cm;
    this->cold_finger_shell_length               = 14.5*cm;

    // Values for the cold finger, and cooling structure (added Jan 2005)
    this->rear_plate_thickness                   = 0.2*cm;
    this->cold_finger_end_plate_thickness        = 10.0*mm; 	// Changed Jan 2005 from 9.0mm
    this->cold_finger_radius                     = 9.0*mm; 	// Changed Jan 2005 from 8.25mm
    this->cold_finger_space                      = 1.0*cm;  	// Space between the cold plate and the germanium crystals
    this->cold_finger_length                     = this->cold_finger_shell_length +this->detector_total_length
            - this->can_face_thickness -this->germanium_dist_from_can_face
            - this->germanium_length -this->cold_finger_space
            - this->cold_finger_end_plate_thickness;
    // New stuff
    this->extra_block_thickness                  = 9.0*mm;
    this->extra_block_distance_from_back_plate   = 10.0*mm;
    this->extra_block_inner_diameter             = 34.0*mm;

    this->triangle_posts_distance_from_crystals  = 2.0*mm;
    this->triangle_post_starting_depth           = 38.0*mm;
    this->fet_air_hole_radius                    = 26.0*mm/2.0;
    this->cooling_side_block_thickness           = 15.0*mm;
    this->cooling_side_block_width               = 20.0*mm;
    this->cooling_side_block_horizontal_depth    = 10.0*mm;
    this->structureMat_cold_finger_thickness     = 26.0*mm;
    this->structureMat_cold_finger_radius        = 22.0*mm/2.0;
    this->cooling_bar_thickness                  = 10.0*mm;
    this->cooling_bar_width                      = 8.0*mm ;

    // Liquid nitrogen tank stuff
    this->coolant_length                         = 39.3*cm;   // Length of the Liquid Nitrogen tank
    this->coolant_radius                         = 12.5*cm;
    this->coolant_thickness                      = 0.5*cm;


    this->hevimet_tip_thickness                  = 1.27*cm;
    //  this->hevimet_tip_angle                      = this->bent_end_angle -atan((this->germanium_width +0.5*this->germanium_separation
    //                                               - this->germanium_bent_length*tan(this->bent_end_angle))/(this->back_inner_radius
    //                                               + this->can_face_thickness +this->germanium_dist_from_can_face));

    this->hevimet_tip_angle                      = this->bent_end_angle -atan((this->germanium_width +0.5*this->germanium_separation
                                                                               - this->germanium_bent_length*tan(this->bent_end_angle))/( this->suppressor_back_radius
                                                                                                                                          + this->can_face_thickness +this->germanium_dist_from_can_face));

    // the difference between the 22.5 angle and the angle that the heavymet makes to the germanium
    this->back_BGO_thickness                     = 3.7*cm;    // 6.0*cm;
    this->BGO_chopped_tip                        = 5.0*mm;
    this->BGO_movable_space                      = 3.0*cm;  	// amount the can face can move back into the BGO
    this->side_suppressor_back_offset            = 1.0*cm;
    this->side_BGO_thickness                     = 2.0*cm;
    this->BGO_can_seperation                     = 0.5*mm;  	// Distance seperating the BGO and the detector can

    this->side_BGO_length                        = this->BGO_movable_space +this->detector_total_length
            + this->back_BGO_thickness +this->BGO_can_seperation;

    this->side_suppressor_length                 = this->detector_total_length -this->bent_end_length
            + this->BGO_can_seperation +this->back_BGO_thickness
            - this->side_suppressor_back_offset -(this->BGO_can_seperation
                                                  + this->BGO_chopped_tip)/tan(this->bent_end_angle);

    this->BGO_trap_length                        = (this->side_BGO_thickness/tan(this->bent_end_angle))
            - (this->BGO_chopped_tip/tan(this->bent_end_angle));

    this->suppressor_extension_thickness         = 1.0*cm;

    //  this->suppressor_extension_length            = (this->back_inner_radius +this->bent_end_length
    //                                               + (this->BGO_can_seperation +this->side_BGO_thickness)
    //                                               / tan(this->bent_end_angle) -this->suppressor_extension_thickness
    //                                               * sin(this->bent_end_angle))/cos(this->bent_end_angle)
    //                                               - this->hevimet_tip_thickness -this->forward_inner_radius; // Original

    this->suppressor_extension_length            = (this->suppressor_back_radius + this->bent_end_length
                                                    + (this->BGO_can_seperation + this->side_BGO_thickness)
                                                    / tan(this->bent_end_angle) - this->suppressor_extension_thickness
                                                    * sin(this->bent_end_angle)) / cos(this->bent_end_angle)
            - this->hevimet_tip_thickness - this->suppressor_forward_radius;

    this->suppressor_extension_length_det        = 	(this->back_inner_radius + this->bent_end_length
                                                     + (this->BGO_can_seperation + this->side_BGO_thickness)
                                                     / tan(this->bent_end_angle) - this->suppressor_extension_thickness
                                                     * sin(this->bent_end_angle)) / cos(this->bent_end_angle)
            - this->hevimet_tip_thickness - this->forward_inner_radius;


    //  this->suppressor_extension_angle 	           = atan(((this->back_inner_radius + this->bent_end_length + (this->BGO_can_seperation
    //                                               + this->side_BGO_thickness) / tan(this->bent_end_angle)
    //                                               - this->suppressor_extension_thickness *sin(this->bent_end_angle))
    //                                               * tan(this->bent_end_angle) - (this->forward_inner_radius + this->hevimet_tip_thickness)
    //                                               * sin(this->bent_end_angle)) / (this->suppressor_extension_length)); // Original

    this->suppressor_extension_angle 	           = atan(((this->suppressor_back_radius + this->bent_end_length + (this->BGO_can_seperation
                                                                                                                    + this->side_BGO_thickness) / tan(this->bent_end_angle)
                                                            - this->suppressor_extension_thickness *sin(this->bent_end_angle))
                                                           * tan(this->bent_end_angle) - (this->suppressor_forward_radius + this->hevimet_tip_thickness)
                                                           * sin(this->bent_end_angle)) / (this->suppressor_extension_length));

    this->HeavyMet_thickness                     = 3.0*cm;   	// This can't be more than bent_end_length +
    // (chopped_tip+seperation)/tan(bent_angle)

    this->HeavyMet_inside_angle                  = atan((this->detector_total_width/2.0)/
                                                        ((this->detector_total_width/2.0 +this->BGO_chopped_tip
                                                          + this->BGO_can_seperation)/tan(this->bent_end_angle)
                                                         + this->BGO_movable_space +this->bent_end_length));

    // used only in new suppressor design, but don't affect old suppressor design
    // As of September 2013, suppressor_shells_include_flag will always be true, so any dependence on it should be removed.


    //     this->air_box_front_width 		           = 2.0*(this->detector_total_width/2.0 +this->BGO_can_seperation
    //                                               + (this->side_BGO_thickness + this->suppressor_shell_thickness*2.0)
    //                                               + ( this->suppressor_extension_length
    //                                               +  (this->suppressor_shell_thickness*2.0
    //                                               * (1.0/tan(this->bent_end_angle)-tan(this->bent_end_angle))) )
    //                                               * sin(this->bent_end_angle) +(this->suppressor_extension_thickness
    //                                               + this->suppressor_shell_thickness*2.0)/cos(this->bent_end_angle)) ; // Original
    //                                               // the longer width of the box's trapezoid

    this->air_box_front_width 		           = 2.0*(this->detector_total_width/2.0 +this->BGO_can_seperation
                                                      + (this->side_BGO_thickness + this->suppressor_shell_thickness*2.0)
                                                      + ( this->suppressor_extension_length
                                                          +  (this->suppressor_shell_thickness*2.0
                                                              * (1.0/tan(this->bent_end_angle)-tan(this->bent_end_angle))) )
                                                      * sin(this->bent_end_angle) +(this->suppressor_extension_thickness
                                                                                    + this->suppressor_shell_thickness*2.0)/cos(this->bent_end_angle)) ;
    // the longer width of the box's trapezoid

    this->air_box_front_width_det                = 2.0*(this->detector_total_width/2.0 +this->BGO_can_seperation
                                                        + (this->side_BGO_thickness + this->suppressor_shell_thickness*2.0)
                                                        + ( this->suppressor_extension_length_det
                                                            +  (this->suppressor_shell_thickness*2.0
                                                                * (1.0/tan(this->bent_end_angle)-tan(this->bent_end_angle))) )
                                                        * sin(this->bent_end_angle) +(this->suppressor_extension_thickness
                                                                                      + this->suppressor_shell_thickness*2.0)/cos(this->bent_end_angle)) ;

    //  this->air_box_front_length                   = (this->air_box_front_width/2.0 -this->forward_inner_radius
    //                                               *sin(this->bent_end_angle))/tan(this->bent_end_angle); // Original

    this->air_box_front_length                   = (this->air_box_front_width/2.0 - this->suppressor_forward_radius
                                                    * sin(this->bent_end_angle)) / tan(this->bent_end_angle);

    this->air_box_front_length_det               =  (this->air_box_front_width_det/2.0 - this->forward_inner_radius
                                                     * sin(this->bent_end_angle)) / tan(this->bent_end_angle) ;

    //  this->air_box_back_length                    = this->detector_total_length +this->cold_finger_shell_length
    //                                               +this->coolant_length +(this->back_inner_radius
    //                                               -this->forward_inner_radius*cos(this->bent_end_angle))
    //                                               -this->air_box_front_length; // Original

    this->air_box_back_length                    = this->detector_total_length + this->cold_finger_shell_length
            + this->coolant_length + ( this->suppressor_back_radius
                                       - this->suppressor_forward_radius * cos(this->bent_end_angle))
            - this->air_box_front_length;

    this->air_box_back_length_det                = this->detector_total_length + this->cold_finger_shell_length
            + this->coolant_length + (this->back_inner_radius
                                      - this->forward_inner_radius * cos(this->bent_end_angle))
            - this->air_box_front_length_det;

    // shift" is applied to everything to place them relative to the front of the air_box
    // the negative is because the shift is along the -ive X-xis

    this->suppShift =                            - (this->air_box_back_length/2.0 + this->air_box_front_length
                                                    - (this->suppressor_forward_radius * (1 - cos(this->bent_end_angle)))
                                                    - this->can_face_thickness/2.0);


    this->shift =                                - (this->air_box_back_length_det/2.0 +this->air_box_front_length_det
                                                    - (this->forward_inner_radius -this->forward_inner_radius*cos(this->bent_end_angle))
                                                    - this->can_face_thickness/2.0); // Original


    this->depth_segmentation_adjustment          = depth_segmentation_adjustment;

    // Gives the proper diameter of the Tigress array based on can face length
    this->rhombi_diameter                        = this->detector_total_width
            - 2.0*this->bent_end_length
            * tan(this->bent_end_angle)
            + 2.0*(this->detector_total_width
                   - 2.0*this->bent_end_length
                   * tan(this->bent_end_angle))
            * cos(M_PI/4.0);

    // This ones used by the new suppressor design

    this->new_rhombi_radius_det                   = this->forward_inner_radius*cos(this->bent_end_angle); // Original

    this->new_rhombi_radius                      	= this->suppressor_forward_radius*cos(this->bent_end_angle);


    if (can_position_specifier == 1) {
        this->detector_position_shift              =  this->bent_end_length +(this->BGO_chopped_tip
                                                                              + this->BGO_can_seperation)/tan(this->bent_end_angle);
    }
    else if (can_position_specifier == 2) {
        this->detector_position_shift              = 0.0*cm;
    }
    else if (can_position_specifier == 3) {
        this->detector_position_shift              = -(this->BGO_movable_space);
    }
    else {
        this->detector_position_shift              = 0.0*cm;
    }

    // this is used to actually apply the necessary detector shift to all the pieces involved
    if(this->suppressor_position_selector == 0) {
        this->applied_back_shift                   = 0.0*cm;
    }
    else if(this->suppressor_position_selector == 1) {
        this->applied_back_shift                   = this->back_inner_radius - this->forward_inner_radius; // 3.5cm
    }

    if(detector_format == 1) {
        this->germanium_selector                   = 0;
    }
    else if (detector_format == 2) {
        this->germanium_selector                   = 1;
    }
    else if(detector_format == 3) {
        this->germanium_selector                   = 2;
    }
    else {
        this->germanium_selector                   = 2;
    }

    if (include_detector_can == "y") {
        this->can_selector                         = 1;
    }
    else if(include_detector_can == "n") {
        this->can_selector                         = 0;
    }
    else {
        this->can_selector                         = 0;
    }

    if(include_BGO == "y") {
        this->BGO_selector                         = 1;
    }
    else if(include_BGO == "n") {
        this->BGO_selector                         = 0;
    }
    else {
        this->BGO_selector                         = 0;
    }

    if(include_cold_finger == "y") {
        this->cold_finger_selector                 = 1;
    }
    else if(include_cold_finger == "n") {
        this->cold_finger_selector                 = 0;
    }
    else {
        this->cold_finger_selector                 = 0;
    }

    if (this->suppressor_position_selector == 0) {
        crystal_dist_from_origin                  = this->forward_inner_radius
                + this->germanium_dist_from_can_face
                + this->can_face_thickness;
    }
    else if (this->suppressor_position_selector == 1) {
        crystal_dist_from_origin                   = this->back_inner_radius
                + this->germanium_dist_from_can_face
                + this->can_face_thickness;
    }

    this->radial_distance                        = this->forward_inner_radius*cos(this->bent_end_angle)
            + this->air_box_back_length/2.0 +this->air_box_front_length;

    //Redacted parameters/////////////////////////////////////////////////////////////
    this->detectorPlacementCxn = 0.4*mm;
    this->trianglePostDim = 1.0*micrometer;

    this->suppressorExtRightX = -2.55*mm;
    this->suppressorExtRightY = 0.24*mm;
    this->suppressorExtRightZ = -0.1*mm;

    this->suppressorExtLeftX = -2.55*mm;
    this->suppressorExtLeftY = 0.24*mm;
    this->suppressorExtLeftZ = 0.1*mm;

    this->wedgeDim = 0.001*mm;

    this->quarterDetectorCxn = 0.01*mm;
    this->quarterDetectorCxnB = 0.1*mm;

    this->electrodeMaterial = "G4_Cu";
    this->structureMaterial = "G4_Al";

}// end ::DetectionSystemGriffin



///// Legacy 8Pi content /////////////////////////////////////////////////////////////////////
// DetectionSystem8pi::DetectionSystem8pi() :
//     // Logical Volumes
//     germanium_block_log(0),
//     germanium_dead_layer_log(0),
//     germanium_vacuum_core_log(0),
//     lower_electrodeMat_electrode_log(0),
//     upper_electrodeMat_electrode_log(0),
//     inner_cage_1_log(0),
//     inner_cage_2_log(0),
//     inner_cage_3_log(0),
//     inner_cage_4_log(0),
//     inner_cage_bottom_log(0),
//     inner_cage_lid_log(0),
//     structureMat_cooling_rod_log(0),
//     electrodeMat_cooling_rod_log(0),
//     cooling_rod_cover_log(0),
//     cooling_rod_cover_lid_log(0),
//     outer_can_side_log(0),
//     outer_can_lid_log(0),
//     beryllium_window_log(0),
//     inner_BGO_annulus_log(0),
//     structureMat_sheath_log(0),
//     outer_lower_BGO_annulus_log(0),
//     outer_upper_BGO_annulus_log(0),
//     liquid_N2_log(0),
//     liquid_N2_side_log(0),
//     liquid_N2_lid_log(0),
//     liquid_N2_bottom_log(0),
//     hevimetal_log(0),
//     auxMat_plug_log(0),
//     auxMat_layer_log(0)
// {
//     /*Set detail view angle (360 for full, 180 for half view)*/
//     //my_detector->detail_view_end_angle = 180.0*deg; //cross-section view
//     this->detail_view_end_angle = 360.0*deg; //fully enclosed

//     this->cut_clearance = 0.1*mm;

//     /*Set detector measurements for my_detector*/
//     //~   denotes estimated from blueprints
//     //*   denotes obtained/converted from blueprints
//     //?   denotes unknown/estimated
//     //OK  denotes confirmed measurements

//     //distance from origin to front face of Hevimetal
//     //  this->dist_from_origin = 9.906*cm; //OK
//     this->dist_from_origin = 0.0*cm; //OK

//     //Germanium crystal, core, dead layer & electrode
//     this->crystal_outer_radius = 2.50*cm; //OK    *** Changed from spec (2.585*cm) to give realistic efficiencies *** -Evan
//     this->crystal_inner_radius = 0.50*cm; //OK
//     this->hole_starting_depth = 2.0*cm; //OK
//     this->crystal_length = 5.0*cm; //OK     *** Changed from spec (5.620*cm) to give realistic efficiencies *** -Evan
//     this->dead_layer_thickness = 1.0*mm; //OK
//     this->electrode_radius = 1.0*mm; //OK

//     //StructureMat cage surrounding Ge crystal
//     this->inner_can_thickness = 0.5*mm; //OK
//     this->inner_can_extends_past_crystal = 2.0*cm; //~2.0cm
//     this->inner_can_lid_thickness = 1.78*mm; //*
//     this->inner_can_lid_separation = 4.0*mm; //OK

//     //Beryllium window
//     this->beryllium_dist_from_crystal = 2.0*mm; //~   //lower cage ring thickness omitted
//     this->beryllium_thickness = 0.5*mm; //OK

//     //StructureMat can surrounding Ge crystal & cage
//     this->outer_can_length = 11.5*cm; //OK
//     this->outer_can_thickness = 0.75*mm; //OK

//     //StructureMat & ElectrodeMat Cooling Rods & Cover
//     this->structureMat_cooling_rod_radius = 3.0*mm; //~

//     this->electrodeMat_cooling_rod_radius = 4.76*mm; //OK   //must be > structureMat rod radius
//     this->electrodeMat_cooling_rod_length = 36.75*cm; //OK    //length from where structureMat rod ENDS to LN2 container

//     //Inner BGO surrounding cooling rod
//     this->inner_BGO_annulus_length = 8.1*cm; //OK from blueprint
//     this->inner_BGO_clearance = 0.5*mm; //~ dist. b/w BGO & surrounding structureMat cover
//     this->inner_BGO_innerRadius = 9.9*mm; //OK    from blueprint
//     this->inner_BGO_outerRadius = 30.7*mm; //OK   from blueprint

//     //Outer BGO surrounding entire detector
//     this->outer_BGO_bottom_thickness = 1.35*cm; //OK
//     this->outer_BGO_taper_height = 6.2*cm; //*
//     this->outer_BGO_top_outer_radius = 6.55*cm;
//     this->outer_BGO_total_length = 19.0*cm;//*
//     this->outer_BGO_clearance = 0.5*mm;//~
//     this->outer_BGO_displacement = 3.25*cm; //OK (distance that detector is recessed from BGO) *** Changed from spec (1.0*cm) to give realistic efficiencies *** -Evan

//     //Liquid N2 container measurements
//     this->liquid_N2_length = 22.8*cm; //OK
//     this->liquid_N2_radius = 7.64*cm;  //OK

//     //Hevimetal collimator measurements
//     this->hevimetal_thickness = 2.54*cm; //OK
//     this->hevimetal_front_side_length = 4.369*cm; //OK
//     this->hevimetal_rear_side_length = 5.489*cm; //OK

//     //AuxMat
//     this->auxMat_plug_neg_radius = 1.64*cm; //OK
//     this->auxMat_plug_pos_radius = 2.06*cm; //OK
//     this->auxMat_layer_thickness = 9.6*mm; //OK
//     //this->auxMat_layer_thickness = 1.50*cm; //OK

//     //suppressed:
//     this->extraClearance = 0.5*mm;
//     this->electrodeMat = "Copper";
//     this->structureMat = "Aluminum";
//     this->auxMat = "Delrin";

//     /*****************************************************************************/
//     /*** definitions for commonly used parameters - DO NOT EDIT (unless you really have to) ***/
//     /**/  //distance to center of Ge crystal from world origin
//     /**/  crystal_dist_from_origin = crystal_length/2.0
//             /**/                 + beryllium_dist_from_crystal
//             /**/                 + beryllium_thickness
//             /**/                 + outer_BGO_displacement
//             /**/                 + hevimetal_thickness
//             /**/                 + dist_from_origin;
//     /**/  //distance from top of crystal to top of outer can
//     /**/  outer_can_extends_past_crystal =  outer_can_length
//             /**/                    - crystal_length
//             /**/                    - beryllium_dist_from_crystal
//             /**/                    - beryllium_thickness;
//     /**/  //inner radius of outer structureMat can
//     /**/  outer_can_innerRadius =   inner_BGO_outerRadius
//             /**/                + inner_BGO_clearance
//             /**/                - outer_can_thickness;
//     /**/  //auxMat front side length calculated as extension from hevimetal
//     /**/  auxMat_layer_front_side_length =  hevimetal_front_side_length
//             /**/                    - (auxMat_layer_thickness
//                                        /**/                    * (hevimetal_rear_side_length - hevimetal_front_side_length)
//                                        /**/                    / hevimetal_thickness);
//     /**************************************************************/
// }

// ///////////////////////////////////////////////////////////////////////
// // The ::Apparatus8piVacuumChamber constructor initiates all the Logical
// // and Physical Volumes used in the vacuum Chamber geometery, and the
// //  ::Apparatus8piVacuumChamber  destructor deletes them from the stack
// // when they go out of scope
// ///////////////////////////////////////////////////////////////////////
// Apparatus8piVacuumChamber::Apparatus8piVacuumChamber() :
//     // LogicalVolumes
//     vacuum_chamber_sphere_log(0)//, vacuum_chamber_sphere_vacuum_log(0)
// {
//     /////////////////////////////////////////////////////////////////////
//     // Apparatus8piVacuumChamber Physical Properties
//     /////////////////////////////////////////////////////////////////////

//     this->vacuum_material                                 = "Vacuum";

//     this->vacuum_chamber_sphere_material                  = "Delrin";

//     this->vacuum_chamber_inner_radius                     = 84.4*mm;
//     this->vacuum_chamber_outer_radius                     = 89.4*mm;

// }// end ::Apparatus8piVacuumChamber

// ///////////////////////////////////////////////////////////////////////
// // The ::Apparatus8piVacuumChamberAuxMatShell constructor initiates all the Logical
// // and Physical Volumes used in the vacuum Chamber geometery, and the
// //  ::Apparatus8piVacuumChamberAuxMatShell  destructor deletes them from the stack
// // when they go out of scope
// ///////////////////////////////////////////////////////////////////////
// Apparatus8piVacuumChamberAuxMatShell::Apparatus8piVacuumChamberAuxMatShell() :
//     // LogicalVolumes
//     vacuum_chamber_aux_sphere_log(0)
// {
//     /////////////////////////////////////////////////////////////////////
//     // Apparatus8piVacuumChamberAuxMatShell Physical Properties
//     /////////////////////////////////////////////////////////////////////

//     this->vacuum_chamber_sphere_material                  = "Delrin";

//     this->vacuum_chamber_inner_radius                     = 89.4*mm;
//     this->vacuum_chamber_outer_radius                     = 89.4*mm;

// }// end ::Apparatus8piVacuumChamberAuxMatShell



// void DetectorConstruction::AddDetectionSystem8pi(G4int ndet)
// {
//     // Describe Placement
//     G4double theta,phi,position;
//     G4ThreeVector move,direction;

//     G4double hexagonAlign[20][5] = {
//         {-0.982247, 0.000000,-0.187593,  100.812,  180.000},
//         {-0.303531, 0.934172,-0.187592,  100.812,  108.000},
//         { 0.794654, 0.577351,-0.187592,  100.812,   36.000},
//         { 0.794655,-0.577350,-0.187592,  100.812,  324.000},
//         {-0.303531,-0.934172,-0.187592,  100.812,  252.000},
//         { 0.982247, 0.000000, 0.187593,   79.188,    0.000},
//         { 0.303531,-0.934172, 0.187592,   79.188,  288.000},
//         {-0.794654,-0.577351, 0.187592,   79.188,  216.000},
//         {-0.794655, 0.577350, 0.187592,   79.188,  144.000},
//         { 0.303531, 0.934172, 0.187592,   79.188,   72.000},
//         {-0.607062, 0.000000,-0.794655,  142.623,  180.000},
//         {-0.187592, 0.577350,-0.794655,  142.623,  108.000},
//         { 0.491124, 0.356822,-0.794654,  142.623,   36.000},
//         { 0.491124,-0.356822,-0.794654,  142.623,  324.000},
//         {-0.187592,-0.577350,-0.794655,  142.623,  252.000},
//         { 0.607062, 0.000000, 0.794655,   37.377,    0.000},
//         { 0.187592,-0.577350, 0.794655,   37.377,  288.000},
//         {-0.491124,-0.356822, 0.794654,   37.377,  216.000},
//         {-0.491124, 0.356822, 0.794654,   37.377,  144.000},
//         { 0.187592, 0.577350, 0.794655,   37.377,   72.000}};

//     DetectionSystem8pi* pDetectionSystem8pi = new DetectionSystem8pi() ;
//     pDetectionSystem8pi->Build() ;

//     for(G4int detector_number = 0; detector_number < ndet; detector_number++)
//     {
//         theta = hexagonAlign[detector_number][3]*deg;
//         phi = hexagonAlign[detector_number][4]*deg;

//         G4RotationMatrix* rotate = new G4RotationMatrix; // rotation matrix corresponding to direction vector
//         rotate->rotateZ(30.0*deg);
//         rotate->rotateY(theta);
//         rotate->rotateZ(phi);

//         direction = G4ThreeVector(sin(theta)*cos(phi),sin(theta)*sin(phi),cos(theta));
//         position = 9.906*cm;
//         move = position * direction;

//         pDetectionSystem8pi->PlaceDetector(this->logicWorld, move, rotate, detector_number);
//     }

// }

// void DetectorConstruction::AddDetectionSystem8piDetector(G4int ndet)
// {
//     // Describe Placement
//     G4double theta,phi,position;
//     G4ThreeVector move,direction;

//     G4double hexagonAlign[20][5] = {
//         {-0.982247, 0.000000,-0.187593,  100.812,  180.000},
//         {-0.303531, 0.934172,-0.187592,  100.812,  108.000},
//         { 0.794654, 0.577351,-0.187592,  100.812,   36.000},
//         { 0.794655,-0.577350,-0.187592,  100.812,  324.000},
//         {-0.303531,-0.934172,-0.187592,  100.812,  252.000},
//         { 0.982247, 0.000000, 0.187593,   79.188,    0.000},
//         { 0.303531,-0.934172, 0.187592,   79.188,  288.000},
//         {-0.794654,-0.577351, 0.187592,   79.188,  216.000},
//         {-0.794655, 0.577350, 0.187592,   79.188,  144.000},
//         { 0.303531, 0.934172, 0.187592,   79.188,   72.000},
//         {-0.607062, 0.000000,-0.794655,  142.623,  180.000},
//         {-0.187592, 0.577350,-0.794655,  142.623,  108.000},
//         { 0.491124, 0.356822,-0.794654,  142.623,   36.000},
//         { 0.491124,-0.356822,-0.794654,  142.623,  324.000},
//         {-0.187592,-0.577350,-0.794655,  142.623,  252.000},
//         { 0.607062, 0.000000, 0.794655,   37.377,    0.000},
//         { 0.187592,-0.577350, 0.794655,   37.377,  288.000},
//         {-0.491124,-0.356822, 0.794654,   37.377,  216.000},
//         {-0.491124, 0.356822, 0.794654,   37.377,  144.000},
//         { 0.187592, 0.577350, 0.794655,   37.377,   72.000}};

//     DetectionSystem8pi* pDetectionSystem8pi = new DetectionSystem8pi() ;
//     pDetectionSystem8pi->Build() ;

//     theta = hexagonAlign[ndet-1][3]*deg;
//     phi = hexagonAlign[ndet-1][4]*deg;

//     G4RotationMatrix* rotate = new G4RotationMatrix; //rotation matrix corresponding to direction vector
//     rotate->rotateZ(30.0*deg);
//     rotate->rotateY(theta);
//     rotate->rotateZ(phi);

//     direction = G4ThreeVector(sin(theta)*cos(phi),sin(theta)*sin(phi),cos(theta));
//     position = 9.906*cm;
//     move = position * direction;

//     pDetectionSystem8pi->PlaceDetector(this->logicWorld, move, rotate, ndet-1);
// }
