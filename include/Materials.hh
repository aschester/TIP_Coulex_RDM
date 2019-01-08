#ifndef Materials_H
#define Materials_H 1

#include "globals.hh"
#include "G4Material.hh"
#include "G4NistManager.hh"

using namespace CLHEP;

class Materials 
{
  public:

  Materials();
  ~Materials();
  
  G4Material* FindMaterial(G4String );
    
    private:

  // Elements
  G4Element* elH;
  G4Element* elC;
  G4Element* elO;
  G4Element* elSi;

  // Elemental materials
  G4Material* Al;
  G4Material* Ge;
  G4Material* Cu;
  G4Material* C;
  G4Material* Mg;
  G4Material* In;
  G4Material* Hf;
  G4Material* Au;
  G4Material* Ta;
  G4Material* Pb;
  G4Material* Be;
  G4Material* Ni;
  G4Material* Ti;
  G4Material* Ag;
  G4Material* Pt;

  // Compounds
  G4Material* Vacuum;
  G4Material* BGO;
  G4Material* CsI;
  G4Material* Mylar;
  G4Material* Polyethylene;
  G4Material* Air;
  G4Material* G10;
  G4Material* amorC;
};

#endif

