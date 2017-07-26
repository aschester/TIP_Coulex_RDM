#ifndef Materials_H
#define Materials_H 1

#include "globals.hh"
#include "G4Material.hh"
#include "G4NistManager.hh"

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
  G4Material* Au;
  G4Material* Pb;
  G4Material* Be;

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

