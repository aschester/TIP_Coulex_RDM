#include "CsI_array.hh"

//-----------------------------------------------------------------------------
CsI_array::CsI_array(G4LogicalVolume* experimentalHall_log,Materials* mat)
{
  // position is face position + 1/2 CsI thickness
  //zpos=5.7*mm + 0.5*GetCsIDetector()->GetCsIDetectorThickness();
  //zpos=28.7*mm + 0.5*GetCsIDetector()->GetCsIDetectorThickness();
  zpos=51.7*mm + 0.5*GetCsIDetector()->GetCsIDetectorThickness();
  materials=mat;
  expHall_log=experimentalHall_log;

  // Ring assignments by theta (ring : array positions)
  // 1: 1, 3, 5, 7
  // 2: 2, 4, 6, 8
  // 3: 9, 13, 17, 21
  // 4: 10, 12, 14, 16, 18, 20, 22, 24
  // 5: 11, 15, 19, 23

  // Birks constant kB in um/MeV

  // Theta-based ring assignment
  // kBavg = 1.4532 Savg 846.800
  id[ 0]= 1;pos[ 0].setX( 15.2*mm);pos[ 0].setY( 00.0*mm);pos[ 0].setZ(zpos);ring[ 0]=1;kB[ 0]=1.7077;S[ 0]=850.610;
  id[ 1]= 2;pos[ 1].setX( 15.2*mm);pos[ 1].setY( 16.7*mm);pos[ 1].setZ(zpos);ring[ 1]=2;kB[ 1]=1.5179;S[ 1]=808.249;
  id[ 2]= 3;pos[ 2].setX( 00.0*mm);pos[ 2].setY( 16.7*mm);pos[ 2].setZ(zpos);ring[ 2]=1;kB[ 2]=1.4791;S[ 2]=869.615;
  id[ 3]= 4;pos[ 3].setX(-15.2*mm);pos[ 3].setY( 16.7*mm);pos[ 3].setZ(zpos);ring[ 3]=2;kB[ 3]=1.4532;S[ 3]=846.800;//avg of others
  id[ 4]= 5;pos[ 4].setX(-15.2*mm);pos[ 4].setY( 00.0*mm);pos[ 4].setZ(zpos);ring[ 4]=1;kB[ 4]=1.4622;S[ 4]=862.841;
  id[ 5]= 6;pos[ 5].setX(-15.2*mm);pos[ 5].setY(-16.7*mm);pos[ 5].setZ(zpos);ring[ 5]=2;kB[ 5]=0.6393;S[ 5]=552.371;
  id[ 6]= 7;pos[ 6].setX( 00.0*mm);pos[ 6].setY(-16.7*mm);pos[ 6].setZ(zpos);ring[ 6]=1;kB[ 6]=1.4849;S[ 6]=809.215;
  id[ 7]= 8;pos[ 7].setX( 15.2*mm);pos[ 7].setY(-16.7*mm);pos[ 7].setZ(zpos);ring[ 7]=2;kB[ 7]=1.5438;S[ 7]=782.957;
  id[ 8]= 9;pos[ 8].setX( 30.4*mm);pos[ 8].setY( 00.0*mm);pos[ 8].setZ(zpos);ring[ 8]=3;kB[ 8]=1.4532;S[ 8]=846.800;//avg of others
  id[ 9]=10;pos[ 9].setX( 30.4*mm);pos[ 9].setY( 16.7*mm);pos[ 9].setZ(zpos);ring[ 9]=4;kB[ 9]=1.2883;S[ 9]=796.694;
  id[10]=11;pos[10].setX( 30.4*mm);pos[10].setY( 33.4*mm);pos[10].setZ(zpos);ring[10]=5;kB[10]=1.5874;S[10]=865.548;
  id[11]=12;pos[11].setX( 15.2*mm);pos[11].setY( 33.4*mm);pos[11].setZ(zpos);ring[11]=4;kB[11]=1.5140;S[11]=908.071;
  id[12]=13;pos[12].setX( 00.0*mm);pos[12].setY( 33.4*mm);pos[12].setZ(zpos);ring[12]=3;kB[12]=1.5102;S[12]=893.571;
  id[13]=14;pos[13].setX(-15.2*mm);pos[13].setY( 33.4*mm);pos[13].setZ(zpos);ring[13]=4;kB[13]=1.2183;S[13]=854.953;
  id[14]=15;pos[14].setX(-30.4*mm);pos[14].setY( 33.4*mm);pos[14].setZ(zpos);ring[14]=5;kB[14]=1.3108;S[14]=862.699;
  id[15]=16;pos[15].setX(-30.4*mm);pos[15].setY( 16.7*mm);pos[15].setZ(zpos);ring[15]=4;kB[15]=1.2720;S[15]=782.115;
  id[16]=17;pos[16].setX(-30.4*mm);pos[16].setY( 00.0*mm);pos[16].setZ(zpos);ring[16]=3;kB[16]=1.4769;S[16]=763.549;
  id[17]=18;pos[17].setX(-30.4*mm);pos[17].setY(-16.7*mm);pos[17].setZ(zpos);ring[17]=4;kB[17]=1.6481;S[17]=898.445;
  id[18]=19;pos[18].setX(-30.4*mm);pos[18].setY(-33.4*mm);pos[18].setZ(zpos);ring[18]=5;kB[18]=1.5573;S[18]=967.251;
  id[19]=20;pos[19].setX(-15.2*mm);pos[19].setY(-33.4*mm);pos[19].setZ(zpos);ring[19]=4;kB[19]=1.6031;S[19]=925.601;
  id[20]=21;pos[20].setX( 00.0*mm);pos[20].setY(-33.4*mm);pos[20].setZ(zpos);ring[20]=3;kB[20]=1.3669;S[20]=852.041;
  id[21]=22;pos[21].setX( 15.2*mm);pos[21].setY(-33.4*mm);pos[21].setZ(zpos);ring[21]=4;kB[21]=1.6856;S[21]=957.165;
  id[22]=23;pos[22].setX( 30.4*mm);pos[22].setY(-33.4*mm);pos[22].setZ(zpos);ring[22]=5;kB[22]=1.6814;S[22]=920.972;
  id[23]=24;pos[23].setX( 30.4*mm);pos[23].setY(-16.7*mm);pos[23].setZ(zpos);ring[23]=4;kB[23]=1.4148;S[23]=845.062;
 
  // Inner and outer rings separated
  // id[ 0]= 1;pos[ 0].setX( 15.2*mm);pos[ 0].setY( 00.0*mm);pos[ 0].setZ(zpos);ring[ 0]=1;kB[ 0]=1.7077;S[ 0]=850.610;
  // id[ 1]= 2;pos[ 1].setX( 15.2*mm);pos[ 1].setY( 16.7*mm);pos[ 1].setZ(zpos);ring[ 1]=1;kB[ 1]=1.5179;S[ 1]=808.249;
  // id[ 2]= 3;pos[ 2].setX( 00.0*mm);pos[ 2].setY( 16.7*mm);pos[ 2].setZ(zpos);ring[ 2]=1;kB[ 2]=1.4791;S[ 2]=869.615;
  // id[ 3]= 4;pos[ 3].setX(-15.2*mm);pos[ 3].setY( 16.7*mm);pos[ 3].setZ(zpos);ring[ 3]=1;kB[ 3]=1.4532;S[ 3]=846.800;//avg of others
  // id[ 4]= 5;pos[ 4].setX(-15.2*mm);pos[ 4].setY( 00.0*mm);pos[ 4].setZ(zpos);ring[ 4]=1;kB[ 4]=1.4622;S[ 4]=862.841;
  // id[ 5]= 6;pos[ 5].setX(-15.2*mm);pos[ 5].setY(-16.7*mm);pos[ 5].setZ(zpos);ring[ 5]=1;kB[ 5]=0.6393;S[ 5]=552.371;
  // id[ 6]= 7;pos[ 6].setX( 00.0*mm);pos[ 6].setY(-16.7*mm);pos[ 6].setZ(zpos);ring[ 6]=1;kB[ 6]=1.4849;S[ 6]=809.215;
  // id[ 7]= 8;pos[ 7].setX( 15.2*mm);pos[ 7].setY(-16.7*mm);pos[ 7].setZ(zpos);ring[ 7]=1;kB[ 7]=1.5438;S[ 7]=782.957;
  // id[ 8]= 9;pos[ 8].setX( 30.4*mm);pos[ 8].setY( 00.0*mm);pos[ 8].setZ(zpos);ring[ 8]=2;kB[ 8]=1.4532;S[ 8]=846.800;//avg of others
  // id[ 9]=10;pos[ 9].setX( 30.4*mm);pos[ 9].setY( 16.7*mm);pos[ 9].setZ(zpos);ring[ 9]=2;kB[ 9]=1.2883;S[ 9]=796.694;
  // id[10]=11;pos[10].setX( 30.4*mm);pos[10].setY( 33.4*mm);pos[10].setZ(zpos);ring[10]=2;kB[10]=1.5874;S[10]=865.548;
  // id[11]=12;pos[11].setX( 15.2*mm);pos[11].setY( 33.4*mm);pos[11].setZ(zpos);ring[11]=2;kB[11]=1.5140;S[11]=908.071;
  // id[12]=13;pos[12].setX( 00.0*mm);pos[12].setY( 33.4*mm);pos[12].setZ(zpos);ring[12]=2;kB[12]=1.5102;S[12]=893.571;
  // id[13]=14;pos[13].setX(-15.2*mm);pos[13].setY( 33.4*mm);pos[13].setZ(zpos);ring[13]=2;kB[13]=1.2183;S[13]=854.953;
  // id[14]=15;pos[14].setX(-30.4*mm);pos[14].setY( 33.4*mm);pos[14].setZ(zpos);ring[14]=2;kB[14]=1.3108;S[14]=862.699;
  // id[15]=16;pos[15].setX(-30.4*mm);pos[15].setY( 16.7*mm);pos[15].setZ(zpos);ring[15]=2;kB[15]=1.2720;S[15]=782.115;
  // id[16]=17;pos[16].setX(-30.4*mm);pos[16].setY( 00.0*mm);pos[16].setZ(zpos);ring[16]=2;kB[16]=1.4769;S[16]=763.549;
  // id[17]=18;pos[17].setX(-30.4*mm);pos[17].setY(-16.7*mm);pos[17].setZ(zpos);ring[17]=2;kB[17]=1.6481;S[17]=898.445;
  // id[18]=19;pos[18].setX(-30.4*mm);pos[18].setY(-33.4*mm);pos[18].setZ(zpos);ring[18]=2;kB[18]=1.5573;S[18]=967.251;
  // id[19]=20;pos[19].setX(-15.2*mm);pos[19].setY(-33.4*mm);pos[19].setZ(zpos);ring[19]=2;kB[19]=1.6031;S[19]=925.601;
  // id[20]=21;pos[20].setX( 00.0*mm);pos[20].setY(-33.4*mm);pos[20].setZ(zpos);ring[20]=2;kB[20]=1.3669;S[20]=852.041;
  // id[21]=22;pos[21].setX( 15.2*mm);pos[21].setY(-33.4*mm);pos[21].setZ(zpos);ring[21]=2;kB[21]=1.6856;S[21]=957.165;
  // id[22]=23;pos[22].setX( 30.4*mm);pos[22].setY(-33.4*mm);pos[22].setZ(zpos);ring[22]=2;kB[22]=1.6814;S[22]=920.972;
  // id[23]=24;pos[23].setX( 30.4*mm);pos[23].setY(-16.7*mm);pos[23].setZ(zpos);ring[23]=2;kB[23]=1.4148;S[23]=845.062;
}

//-----------------------------------------------------------------------------
CsI_array::~CsI_array()
{;}

//-----------------------------------------------------------------------------
void CsI_array::Construct()
{
  G4int i;

  vector<CsI_detector*>::iterator itPos = aCsI_array.begin();
  // clear all elements from the array
  for(; itPos < aCsI_array.end(); itPos++)
    delete *itPos;    // free the element from memory
   // finally, clear all elements from the array
  aCsI_array.clear();

  for(i=0;i<NCsI;i++) 
    aCsI_array.push_back(new CsI_detector(expHall_log,materials));   

  itPos = aCsI_array.begin();
  for(i=0;i<NCsI;i++) 
    {
      (*itPos)->Construct();
      pos[i].setZ(zpos);
      (*itPos)->Place(pos[i]);
      (*itPos)->setId(id[i]);
      (*itPos)->setRingId(ring[i]);
      (*itPos)->setName();     
      itPos++;
    }
}

//-----------------------------------------------------------------------------
void CsI_array::Report()
{
  for(int i=0;i<NCsI;i++){
    G4cout<<"id["<<i<<"] "<<id[i]
	  <<" (x,y,z)="<<"("<<pos[i].getX()/mm<<","<<pos[i].getY()/mm<<","<<pos[i].getZ()/mm<<") mm "
	  <<"ring "<<ring[i]<<" "
	  <<"kB "<<kB[i]<< " S "<<S[i]
	  <<G4endl;
  }  
}

//-----------------------------------------------------------------------------
void CsI_array::MakeSensitive(TrackerCsISD* TrackerCsI)
{
   vector<CsI_detector*>::iterator itPos = aCsI_array.begin();
  
  for(; itPos < aCsI_array.end(); itPos++)
    (*itPos)->GetLog()->SetSensitiveDetector(TrackerCsI);
    
 }

//-----------------------------------------------------------------------------
void CsI_array::SetPosZ(G4double z)
{
  vector<CsI_detector*>::iterator itPos = aCsI_array.begin();
  G4int i;
  for(i=0;i<NCsI;i++) 
    {
      pos[i].setZ(z);
      (*itPos)->Place(pos[i]);
      itPos++;
    }
}
