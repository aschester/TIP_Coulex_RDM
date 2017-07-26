#ifndef Results_h
#define Results_h 1

#include "globals.hh"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "TrackerIonSD.hh"
#include "TrackerCsIHit.hh"
#include "Projectile.hh"
#include "Recoil.hh"
#include "Flags.hh"
#include "DetectorConstruction.hh"
#include "Materials.hh"
#include "Reaction.hh"
#include "Common.hh"

// ROOT stuff
#include "TROOT.h"
#include "TTree.h"
#include "TBranch.h"
#include "TStyle.h"
#include "TCanvas.h"
#include "TH1.h"
#include "TH2.h"
#include "TGraph.h"
#include "TApplication.h"
#include "TFile.h"

typedef struct{
  Int_t evNb;
  Int_t Ap;
  Int_t Zp;
  Int_t Ar;
  Int_t Zr;
 } IonStat;

typedef struct{
    Double_t x;
    Double_t y;
    Double_t z;
    Double_t px;
    Double_t py;
    Double_t pz;
    Double_t E;
    Double_t b;
    Double_t w;
    Double_t path;  // path length in material in mg/cm^2
    Double_t Eloss; // energy loss on path
    Double_t df;    // doppler shift factor sqrt(1-b*b)/[1-b*e_g.e_p]
    Double_t t;     // time
 } IonInf;

typedef struct{
    Double_t x;
    Double_t y;
    Double_t z;
    Double_t px;
    Double_t py;
    Double_t pz;
    Double_t E;
    Double_t b;
    Double_t w;
    Double_t path; // path length in material in mg/cm^2
    Double_t LY;   // Birks Law light yield. LY = sum over dL
    Int_t    Id;
    Int_t    ring;
    Int_t    fold;
  } CsIInf;

typedef struct{
    Int_t    Gfold;
    Int_t    GId[GN*GS];
    Int_t    GSeg[GN*GS];
    Double_t Gx[GN*GS];
    Double_t Gy[GN*GS];
    Double_t Gz[GN*GS];
    Int_t    GRing[GN*GS];
    Double_t GE[GN*GS];
    Double_t GW[GN*GS];
    Int_t    GfoldAB;
    Int_t    GIdAB[GN*GS];
    Int_t    GSegAB[GN*GS];
    Int_t    GRingAB[GN*GS];
    Double_t GEAB[GN*GS];
    Double_t GWAB[GN*GS];
 } GInf;

typedef struct{
  Double_t tpL;  // theta proj lab
  Double_t tpCM; // theta proj CM
  Double_t trL;  // theta recoil lab
  Double_t trCM; // theta recoil CM
  Double_t dc;   // distance of closest approach
} ReactInf;

class Results
{
public:
  Results(Projectile*,Recoil*,DetectorConstruction*);
  ~Results();
 
public:
  
  void SetupRun();
  void FillTree(G4int,TrackerIonHitsCollection*,TrackerCsIHitsCollection*,G4double[16][4], G4double[16][4],G4ThreeVector[16][4]);
  void TreeView();
  void TreeSave(G4String);
  void TreeRead(G4String);
  void TreeAdd(G4String);
  void TreeClear();
  void TreeCreate();
  void TreeReport();
  void ProjLabScattAngle();
  void RecLabScattAngle();
  void CrossSectionZProfile();
  void TargetFaceF_CM();
  void PlotDistCloseApproach();
  void SetTargetFaceCrossSection(G4double xs){TargetFaceCrossSection=xs;};
  void GammaRingSpectrum(G4int);
  void GammaABRingSpectrum(G4int);
  void CsIRingEnergySpectrum(G4int);
  void CsIDetectorEnergySpectrum(G4int);
  void CsIRingLYSpectrum(G4int);
  void CsIDetectorLYSpectrum(G4int);
  void CsIFold();
  void DetCryCsIGammaSpectrum(G4int,G4int,G4int);
  void DetRingCsIRingGammaSpectrum(G4int,G4int,G4double);
  G4int RingMap(G4int,G4int);
  void ReportCrystalPositions();
  void ReportCsIPositions();
  void CalculateCrystalPositions();
  void CalculateCsIPositions();
  void GroupCosDist();  
  void SetFWHM_response(G4double,G4double,G4double);
  G4double FWHM_response(G4double);
  G4double CalculatePath(G4ThreeVector,G4ThreeVector);
  void     SetCsILYResponse(G4int,G4double,G4double);
  G4double CalculateBirksLawStep(G4int,G4double,G4double);
  G4double CalculateTheta(G4ThreeVector,G4ThreeVector);
  G4double CalculateDistCloseApproach(G4double,G4double);
  G4double CalculateDopplerFactor(IonInf,G4double,G4double,G4double);
  void     SaveDopplerFactorData();

private:
  // ROOT Tree stuff
  TTree* tree;
  G4int  IonFill;
  
  Projectile*           theProjectile;
  Recoil*               theRecoil;
  DetectorConstruction* theDetector;
  
  IonStat       stat;
  IonInf        gun,pBIn,pTIn,pRIn,pROut,pTOut,pDIn,pDOut,rROut,rTOut,rDIn,rDOut,pDec,rDec,rDTrack;
  CsIInf        pHit,rHit;
  ReactInf      react;
  G4int         Ap,Zp,Ar,Zr;
  G4double      ACM;          // conversion factor for CM velocity transformation
  GInf          GHit;         // gamma hit
  G4double      maxGe[GN];    // maximum single gamma energy deposit in a given detector (used to get position of hits when using addback)
  G4ThreeVector rDiPos,rDPos; // recoil degrader position vectors for calculating distance
  G4double      dDensity;     // degrader density for calculating path in mg/cm^2
  G4double      CsIDensity;   // CsI density for calculating path in mg/cm^2

  G4double kB[NCsI];  // Birks constant in um/MeV
  G4double kBm[NCsI]; // Birks constant in (mg/cm^2)/MeV
  G4double S[NCsI];   // scaling factor to convert LY to channels

  G4ThreeVector p1,p2,pCM; // momentum vectors for calculating CM angles

  size_t soh,soi,sos,sogh,sor;

  TH1D     *h,*g;
  TCanvas  *c,*c1;
  TH2D     *correlation,*d;
  G4double TargetFaceCrossSection;
 
  G4ThreeVector CP[GN][GS];
  G4ThreeVector PP[NCsI];

  G4double A,B,C,F,G,H;

  G4Material* a;
};

#endif


           
