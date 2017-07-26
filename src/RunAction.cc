#include "RunAction.hh"

 
RunAction::RunAction(PhysicsList* PL,Results* Res,DetectorConstruction* Det): thePhysicsList(PL), theResults(Res), theDetector(Det)
{
  
}


RunAction::~RunAction()
{

}

void RunAction::BeginOfRunAction(const G4Run*)
{

  // check to see if its grabbing custom stopping data
  // thePhysicsList->getStoppingData()->DumpMap();
  // G4cout << "For NIST materials" << G4endl;
  // G4int Zp = 13;
  // G4double E = 1.66666666667;
  // G4String mat = "G4_Al";
  // G4cout << Zp << " in " << mat << " stopping at " << E << " MeV/A: " 
  // 	 << (thePhysicsList->getStoppingData()->GetDEDX(1.66666666667,Zp,mat)) / (MeV*cm2/mg) 
  // 	 << " MeV*cm2/mg" << G4endl;
  // mat = "G4_Cu";
  // G4cout << Zp << " in " << mat << " stopping at " << E << " MeV/A: " 
  // 	 << (thePhysicsList->getStoppingData()->GetDEDX(1.66666666667,Zp,mat)) / (MeV*cm2/mg) 
  // 	 << " MeV*cm2/mg" << G4endl;
  // mat = "G4_MYLAR";
  // G4cout << Zp << " in " << mat << " stopping at " << E << " MeV/A: " 
  // 	 << (thePhysicsList->getStoppingData()->GetDEDX(1.66666666667,Zp,mat)) / (MeV*cm2/mg) 
  // 	 << " MeV*cm2/mg" << G4endl;
  // mat = "G4_CESIUM_IODIDE";
  // G4cout << Zp << " in " << mat << " stopping at " << E << " MeV/A: " 
  // 	 << (thePhysicsList->getStoppingData()->GetDEDX(1.66666666667,Zp,mat)) / (MeV*cm2/mg) 
  // 	 << " MeV*cm2/mg" << G4endl;
  // Zp = 36;
  // mat = "G4_Al";
  // G4cout << Zp << " in " << mat << " stopping at " << E << " MeV/A: " 
  // 	 << (thePhysicsList->getStoppingData()->GetDEDX(1.66666666667,Zp,mat)) / (MeV*cm2/mg) 
  // 	 << " MeV*cm2/mg" << G4endl;
  // mat = "G4_Cu";
  // G4cout << Zp << " in " << mat << " stopping at " << E << " MeV/A: " 
  // 	 << (thePhysicsList->getStoppingData()->GetDEDX(1.66666666667,Zp,mat)) / (MeV*cm2/mg) 
  // 	 << " MeV*cm2/mg" << G4endl;
  // mat = "G4_MYLAR";
  // G4cout << Zp << " in " << mat << " stopping at " << E << " MeV/A: " 
  // 	 << (thePhysicsList->getStoppingData()->GetDEDX(1.66666666667,Zp,mat)) / (MeV*cm2/mg) 
  // 	 << " MeV*cm2/mg" << G4endl;
  // mat = "G4_CESIUM_IODIDE";
  // G4cout << Zp << " in " << mat << " stopping at " << E << " MeV/A: " 
  // 	 << (thePhysicsList->getStoppingData()->GetDEDX(1.66666666667,Zp,mat)) / (MeV*cm2/mg) 
  // 	 << " MeV*cm2/mg" << G4endl;  

  G4cout<<" Beginning of run "<<G4endl;
  thePhysicsList->getReaction()->TargetFaceCrossSection();
  recoilA=thePhysicsList->getReaction()->getTargetRecoilA();
  recoilZ=thePhysicsList->getReaction()->getTargetRecoilZ();
  theResults->SetupRun();
  gettimeofday(&ts,NULL);
}


 
void RunAction::EndOfRunAction(const G4Run*)
{
  G4double p,ddx;
  G4cout<<G4endl;
  theResults->SetTargetFaceCrossSection(thePhysicsList->getReaction()->GetTargetFaceCrossSection());
  dx=theDetector->GetPlunger()->GetTargetThickness();   // in cm
  ddx=dx*theDetector->GetPlunger()->GetTargetDensity(); // in g/cm^2
  Nv=theDetector->GetPlunger()->GetTargetNV(recoilZ);   // in atoms/cm3
  p=Nv*dx*thePhysicsList->getReaction()->GetThickTargetCrossSection()*1E-18;
 
  G4cout<<setprecision(3);

  G4cout<<" End of run "<<G4endl;
  G4cout<<" Thin target cross section is calculated assuming all reactions with the energy of the particle gun"<<G4endl;
  G4cout<<" Thick target correction accounts for the projectile energy change while crossing the backing and the target"<<G4endl;
  G4cout<<" Thin Target Cross Section is              : "<<thePhysicsList->getReaction()->GetTargetFaceCrossSection()<<" [b]"<<G4endl;
  G4cout<<" Thick Target correction is                : "<<thePhysicsList->getReaction()->GetThickTargetCorrection()<<G4endl;
  G4cout<<" Thick Target Cross Section is             : "<<thePhysicsList->getReaction()->GetThickTargetCrossSection()<<" [b]"<<G4endl;
  // G4cout<<" Target thickness in um  is                : "<<dx*10000.<<G4endl;
  G4cout<<" Target material density in g/cm3 is       : "<<theDetector->GetPlunger()->GetTargetDensity()<<G4endl;
  G4cout<<" Target thickness in mg/cm2 is             : "<<ddx*1000.<<G4endl;
  // G4cout<<" Number of atoms per unit volume           : "<<Nv<<" atoms/cm3"<<G4endl;
  // G4cout<<" Avogadro's number                         : "<<Avogadro<<" atoms/mol"<<G4endl;
  G4cout<<" Number density of recoils in the target is: "<<Nv/Avogadro*1000.<<" [milli-mole/cm3]"<<G4endl;
  G4cout<<" Number of excitation is                   : "<<p<<" per million beam particles"<<G4endl;
  G4cout<<" Results are based on simulation of : "<<thePhysicsList->getReaction()->GetNumberOfSimulatedReactions()<<" reactions"<<G4endl;
}

