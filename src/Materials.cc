#include "Materials.hh"

 Materials:: Materials()
{
  // G4double density,density_Al,density_Cu;

  G4NistManager* man = G4NistManager::Instance();
  man->SetVerbose(1);

  // Elements
  elH  = man->FindOrBuildElement(1);
  elC  = man->FindOrBuildElement(6);
  elO  = man->FindOrBuildElement(8);
  elSi = man->FindOrBuildElement(14);
 
  // Elemental materials
  Al = man->FindOrBuildMaterial("G4_Al");
  Ge = man->FindOrBuildMaterial("G4_Ge"); 
  Cu = man->FindOrBuildMaterial("G4_Cu");
  C  = man->FindOrBuildMaterial("G4_C");
  Au = man->FindOrBuildMaterial("G4_Au");
  Pb = man->FindOrBuildMaterial("G4_Pb");
  Be = man->FindOrBuildMaterial("G4_Be");

  // Compounds
  Vacuum = man->FindOrBuildMaterial("G4_Galactic");
  BGO    = man->FindOrBuildMaterial("G4_BGO");
  CsI    = man->FindOrBuildMaterial("G4_CESIUM_IODIDE");
  Mylar  = man->FindOrBuildMaterial("G4_MYLAR");
  Polyethylene  = man->FindOrBuildMaterial("G4_POLYETHYLENE");
  Air    = man->FindOrBuildMaterial("G4_AIR");

  // G10 from extended EM example TestEM3/src/DetectorConstruction.cc
  G4Material* G10 = new G4Material("NemaG10",1.700*g/cm3,4);
  G10->AddElement(elSi, 1);
  G10->AddElement(elO , 2);
  G10->AddElement(elC , 3);
  G10->AddElement(elH , 3);

  G4Material* amorC = new G4Material("amorphousC",2.000*g/cm3,1);
  amorC->AddElement(elC , 1);
}

Materials::~ Materials()
{;}
//-----------------------------------------------------------------------------
G4Material*  Materials::FindMaterial(G4String materialName)
{
  // search the material by its name 
  G4Material* pttoMaterial = G4Material::GetMaterial(materialName);  
  return pttoMaterial;
}
