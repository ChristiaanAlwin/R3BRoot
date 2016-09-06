// R3B NeuLAND Cluster cxx-file. Written by C. A. Douma.

// inclusion of required C++ headers:

// Inclusion of the required ROOT headers:

// Inclusion of the required FAIR headers:

// Inclusion of the required R3B headers:

// Inclusion of the own header file:
#include "R3BNeuLANDCluster.h"

// Using standard namespaces:
using namespace std;

// Define the class:
ClassImp(R3BNeuLANDCluster)

// Default Constructor:
R3BNeuLANDCluster::R3BNeuLANDCluster()
{
  fStartX = 0.0;
  fStartY = 0.0;
  fStartZ = 0.0;
  fStartT = 0.0;
  fStopX = 0.0;
  fStopY = 0.0;
  fStopZ = 0.0;
  fStopT = 0.0;
  fEdgeT = 0.0;
  fEdgeX = 0.0;
  fEdgeY = 0.0;
  fEdgeZ = 0.0;
  fE = 0.0;
  fSize = 0;
  fTopology = 0;
  fMarked = kFALSE;
  fElMarker = kFALSE;
  fEliminated = kFALSE;
  fDigiBars.clear();
  if (fSize!=fDigiBars.size()) {cout << "fSize = " << fSize << " | fDigiBars.size() = " << fDigiBars.size() << " ### ERROR in size when creating 0!!!\n";}
}

// Constructor from 1 digi:
R3BNeuLANDCluster::R3BNeuLANDCluster(R3BLandDigi* digi)
{
    fStartT = digi->GetTdc();
    fStartX = digi->GetXX();
    fStartY = digi->GetYY();
    fStartZ = digi->GetZZ();
    fStopT = digi->GetTdc();
    fStopX = digi->GetXX();
    fStopY = digi->GetYY();
    fStopZ = digi->GetZZ();
    fEdgeT = 0.0;
    fEdgeX = 0.0;
    fEdgeY = 0.0;
    fEdgeZ = 0.0;
    fE = digi->GetQdc();
    fSize = 1;
    fTopology = 0;
    fMarked = kFALSE;
    fElMarker = kFALSE;
    fEliminated = kFALSE;
    fDigiBars.clear();
    fDigiBars.push_back(digi->GetPaddleNr() + 1); // digis start at zero!
    if (fSize!=fDigiBars.size()) {cout << "fSize = " << fSize << " | fDigiBars.size() = " << fDigiBars.size() << " ### ERROR in size when creating 1!!!\n";}
}

// Constructor from 2 digis:
R3BNeuLANDCluster::R3BNeuLANDCluster(R3BLandDigi* digi1, R3BLandDigi* digi2)
{
    // We first load the content depending on the assumption that digi1 has the smaller time:
    fStartT = digi1->GetTdc();
    fStartX = digi1->GetXX();
    fStartY = digi1->GetYY();
    fStartZ = digi1->GetZZ();
    fStopT = digi2->GetTdc();
    fStopX = digi2->GetXX();
    fStopY = digi2->GetYY();
    fStopZ = digi2->GetZZ();
    fEdgeT = 0.0;
    fEdgeX = 0.0;
    fEdgeY = 0.0;
    fEdgeZ = 0.0;
    fE = digi1->GetQdc() + digi2->GetQdc();
    fSize = 2;
    fTopology = 0;
    fMarked = kFALSE;
    fElMarker = kFALSE;
    fEliminated = kFALSE;
    fDigiBars.clear();
    fDigiBars.push_back(digi1->GetPaddleNr() + 1);
    fDigiBars.push_back(digi2->GetPaddleNr() + 1);
    
    // then change the required content if the time order was wrong:
    if (digi2->GetTdc()<digi1->GetTdc())
    {
        fStartT = digi2->GetTdc();
        fStartX = digi2->GetXX();
        fStartY = digi2->GetYY();
        fStartZ = digi2->GetZZ();
        fStopT = digi1->GetTdc();
        fStopX = digi1->GetXX();
        fStopY = digi1->GetYY();
        fStopZ = digi1->GetZZ();
    }
    
    // Done!
    if (fSize!=fDigiBars.size()) {cout << "fSize = " << fSize << " | fDigiBars.size() = " << fDigiBars.size() << " ### ERROR in size when creating 2!!!\n";}
}

// Destructor:
R3BNeuLANDCluster::~R3BNeuLANDCluster()
{
  fStartX = 0.0;
  fStartY = 0.0;
  fStartZ = 0.0;
  fStartT = 0.0;
  fStopX = 0.0;
  fStopY = 0.0;
  fStopZ = 0.0;
  fStopT = 0.0;
  fEdgeT = 0.0;
  fEdgeX = 0.0;
  fEdgeY = 0.0;
  fEdgeZ = 0.0;
  fE = 0.0;
  fSize = 0;
  fTopology = 0;
  fMarked = kFALSE;
  fElMarker = kFALSE;
  fEliminated = kFALSE;
  fDigiBars.clear();
  
  if (fSize!=fDigiBars.size()) {cout << "fSize = " << fSize << " | fDigiBars.size() = " << fDigiBars.size() << " ### ERROR in size when destroying!!!\n";}
}

// Define the Get-functions:
Double_t R3BNeuLANDCluster::GetStartT() {return fStartT;}
Double_t R3BNeuLANDCluster::GetStartX() {return fStartX;}
Double_t R3BNeuLANDCluster::GetStartY() {return fStartY;}
Double_t R3BNeuLANDCluster::GetStartZ() {return fStartZ;}
Double_t R3BNeuLANDCluster::GetStopT() {return fStopT;}
Double_t R3BNeuLANDCluster::GetStopX() {return fStopX;}
Double_t R3BNeuLANDCluster::GetStopY() {return fStopY;}
Double_t R3BNeuLANDCluster::GetStopZ() {return fStopZ;}
Double_t R3BNeuLANDCluster::GetEdgeT() {return fEdgeT;}
Double_t R3BNeuLANDCluster::GetEdgeX() {return fEdgeX;}
Double_t R3BNeuLANDCluster::GetEdgeY() {return fEdgeY;}
Double_t R3BNeuLANDCluster::GetEdgeZ() {return fEdgeZ;}
Double_t R3BNeuLANDCluster::GetE() {return fE;}
Int_t R3BNeuLANDCluster::GetSize() {return fSize;}
Int_t R3BNeuLANDCluster::GetTopology() {return fTopology;}
Int_t R3BNeuLANDCluster::GetNbars() {return fDigiBars.size();}
Int_t R3BNeuLANDCluster::GetBar(Int_t const index) {return fDigiBars.at(index);}
Bool_t R3BNeuLANDCluster::IsMarked() {return fMarked;}
Bool_t R3BNeuLANDCluster::IsElMarked() {return fElMarker;}
Bool_t R3BNeuLANDCluster::IsEliminated() {return fEliminated;}
void R3BNeuLANDCluster::StartPosition(TVector3 &pos) {pos.SetXYZ(fStartX, fStartY, fStartZ);}
void R3BNeuLANDCluster::StopPosition(TVector3 &pos) {pos.SetXYZ(fStopX, fStopY, fStopZ);}
void R3BNeuLANDCluster::EdgePosition(TVector3 &pos) {pos.SetXYZ(fEdgeX, fEdgeY, fEdgeZ);}

// Define the Set-functions:
void R3BNeuLANDCluster::Mark() {fMarked = kTRUE;}
void R3BNeuLANDCluster::UnMark() {fMarked = kFALSE;}
void R3BNeuLANDCluster::Eliminate() {fEliminated = kTRUE;}
void R3BNeuLANDCluster::Allow() {fEliminated = kFALSE;}
void R3BNeuLANDCluster::MarkEl() {fElMarker = kTRUE;}
void R3BNeuLANDCluster::UnMarkEl() {fElMarker = kFALSE;}

// Define other functions:
void R3BNeuLANDCluster::Print()
{
    // Prints the content of a cluster:
    cout << " Cluster starting point: fStartT = " << fStartT << "\n"
         << "                         fStartX = " << fStartX << "\n"
         << "                         fStartY = " << fStartY << "\n"
         << "                         fStartZ = " << fStartZ << "\n";
    cout << " Cluster End point: fStopT = " << fStopT << "\n"
         << "                    fStopX = " << fStopX << "\n"
         << "                    fStopY = " << fStopY << "\n"
         << "                    fStopZ = " << fStopZ << "\n";
    cout << " Cluster Edge: fEdgeT = " << fEdgeT << "\n"
         << "               fEdgeX = " << fEdgeX << "\n"
         << "               fEdgeY = " << fEdgeY << "\n"
         << "               fEdgeZ = " << fEdgeZ << "\n";
    cout << " Cluster Total Energy: fE = " << fE << "\n";
    cout << " Cluster Total Size: fSize = " << fSize << "\n";
    cout << " Cluster Topology: fTopology = " << fTopology << "\n";
    cout << " Cluster Is Marked: " << fMarked << "\n";
    cout << " Cluster Is Eliminated: " << fEliminated << "\n";
    cout << " Digibars Vector Size = " << fDigiBars.size() << "\n";
    cout << " Digibars Content = ";
    
    for (Int_t k = 0; k<fDigiBars.size(); ++k)
    {
        cout << fDigiBars.at(k) << ", ";
    }
    
    cout << "\n\n";
}


// Function to add a digi to a cluster:
void R3BNeuLANDCluster::AddDigi(R3BLandDigi* digi)
{
    // First check if this digi is already in the cluster:
    Int_t nbars = fDigiBars.size();
    Bool_t ItIsThere = kFALSE;
    
    for (Int_t k = 0; k<nbars; ++k)
    {
        if (fDigiBars.at(k)==(digi->GetPaddleNr()+1)) {ItIsThere = kTRUE;}
    }
    
    // Then update the cluster data if it is not yet there:
    if (!ItIsThere)
    {
        fE = fE + digi->GetQdc();
        fSize = fSize + 1;
        fDigiBars.push_back(digi->GetPaddleNr() + 1);
    
        // Then update the startpoint if needed:
        if (digi->GetTdc()<fStartT)
        {
            fStartT = digi->GetTdc();
            fStartX = digi->GetXX();
            fStartY = digi->GetYY();
            fStartZ = digi->GetZZ();
        }
    
        // Then update the stop point if needed:    
        if (digi->GetTdc()>fStopT)
        {
            fStopT = digi->GetTdc();
            fStopX = digi->GetXX();
            fStopY = digi->GetYY();
            fStopZ = digi->GetZZ();
        }
    }    
    
    // Check the size of the cluster:
    if (fSize!=fDigiBars.size()) {cout << "fSize = " << fSize << " | fDigiBars.size() = " << fDigiBars.size() << " ### ERROR in size when adding!!!\n";}
    
    // Done!
}

// Function to merge two clusters:
void R3BNeuLANDCluster::Merge(R3BNeuLANDCluster* Second, TClonesArray* fDigis)
{
    // First update the cluster data:
    fE = fE + Second->GetE();
    fSize = fSize + Second->GetSize();
    
    // Now we must check the overlaps in the clusters:
    Int_t nbars1 = fDigiBars.size();
    Int_t nbars2 = Second->GetNbars();
    Int_t ndigis = fDigis->GetEntries();
    R3BLandDigi* digi;
    Bool_t* Overlap = new Bool_t[nbars2];
    for (Int_t k2 = 0; k2<nbars2; ++k2) {Overlap[k2] = kFALSE;}
    
    // Loop to check the overlaps:
    for (Int_t k1 = 0; k1<nbars1; ++k1)
    {
        for (Int_t k2 = 0; k2<nbars2; ++k2)
        {
            if (fDigiBars.at(k1)==Second->GetBar(k2))
            {
                // Then there is overlap. Then we should
                // subtract this digi from the size and
                // from the energy. Loop over the
                // digis to find it:
                for (Int_t kd = 0; kd<ndigis; ++kd)
                {
                    digi = (R3BLandDigi*) fDigis->At(kd);
                    if (fDigiBars.at(k1)==(digi->GetPaddleNr()+1))
                    {
                        // Now we found the digi. Subtract it:
                        fE = fE - digi->GetQdc();
                        fSize = fSize - 1;
                        // We already added all energy, so without this
                        // subtraction, it would be added twice!
                    }
                }
                
                // Since there is overlap, mark this:
                Overlap[k2] = kTRUE;
            }
        }
    }
    
    // Now add the bars that do not overlap to the vector:
    for (Int_t k2 = 0; k2<nbars2; ++k2)
    {
        if (!Overlap[k2])
        {
            fDigiBars.push_back(Second->GetBar(k2));
        }
    }
    
    // Then delete the new:
    delete Overlap;                
    
    // Then update the startpoint if needed:
    if (Second->GetStartT()<fStartT)
    {
        fStartT = Second->GetStartT();
        fStartX = Second->GetStartX();
        fStartY = Second->GetStartY();
        fStartZ = Second->GetStartZ();
    }
    
    // Then update the stop point if needed:    
    if (Second->GetStopT()>fStopT)
    {
        fStopT = Second->GetStopT();
        fStopX = Second->GetStopX();
        fStopY = Second->GetStopY();
        fStopZ = Second->GetStopZ();
    }
    
    // Check the size of the cluster:
    if (fSize!=fDigiBars.size()) {cout << "fSize = " << fSize << " | fDigiBars.size() = " << fDigiBars.size() << " ### ERROR in size when merging!!!\n";}
    
    // Done!
}

// Function to replace a cluster:
void R3BNeuLANDCluster::Replace(R3BNeuLANDCluster* Second)
{
  // Store the vector bars of the second cluster in between variables first:
  Int_t nbars = Second->GetNbars();
  std::vector<Int_t> Between;
  for (Int_t k = 0; k<nbars; ++k)
  {
      Between.push_back(Second->GetBar(k));
  }
  
  // Then do the replacing:
  fStartX = Second->GetStartX();
  fStartY = Second->GetStartY();
  fStartZ = Second->GetStartZ();
  fStartT = Second->GetStartT();
  fStopX = Second->GetStopX();
  fStopY = Second->GetStopX();
  fStopZ = Second->GetStopX();
  fStopT = Second->GetStopX();
  fEdgeT = Second->GetEdgeX();
  fEdgeX = Second->GetEdgeX();
  fEdgeY = Second->GetEdgeX();
  fEdgeZ = Second->GetEdgeX();
  fE = Second->GetE();
  fSize = Second->GetSize();
  fTopology = Second->GetTopology();
  fMarked = Second->IsMarked();
  fEliminated = Second->IsEliminated();
  
  // Finally, replace the digibars:
  fDigiBars.clear();
  for (Int_t k = 0; k<nbars; ++k)
  {
      fDigiBars.push_back(Between.at(k));
  }
  Between.clear();
  
  // Check the size of the cluster:
  if (fSize!=fDigiBars.size()) {cout << "fSize = " << fSize << " | fDigiBars.size() = " << fDigiBars.size() << " ### ERROR in size when replacing!!!\n";}
  
  // Done!
}


// Function to check the topology of the clusters:
Bool_t R3BNeuLANDCluster::TopologyIsLine(TClonesArray* fDigis, Double_t const TopBoundary)
{
    // This function checks if the form of the cluster is
    // consistent with a line.
    
    // Begin by defining the position arrays of the digis
    // inside the cluster:
    Int_t ndigis = fDigis->GetEntries();
    R3BLandDigi* digi;
    Double_t* xdigi = new Double_t[fSize];
    Double_t* ydigi = new Double_t[fSize];
    Double_t* zdigi = new Double_t[fSize];
    Double_t* tdigi = new Double_t[fSize];
    
    for (Int_t k = 0; k<ndigis; ++k)
    {
        digi = (R3BLandDigi*) fDigis->At(k);
        for (Int_t kc = 0; kc<fSize; ++kc)
        {
            if ((digi->GetPaddleNr()+1)==fDigiBars.at(kc))
            {
                xdigi[kc] = digi->GetXX();
                ydigi[kc] = digi->GetYY();
                zdigi[kc] = digi->GetZZ();
                tdigi[kc] = digi->GetTdc();
            }
        }
    }
    
    // Now we simply draw a line between the startpoint and
    // endpoint of the line and compute the distance to the
    // line for each digi. We quadratically sum it and divide
    // it over the number of digis in the cluster. if this
    // number is below a certain value, we call the cluster a line.
    
    // Begin by shifting the origin to fStart. NOTE: timen is left unchanged!
    for (Int_t kc = 0; kc<fSize; ++kc)
    {
        xdigi[kc] = xdigi[kc] - fStartX;
        ydigi[kc] = ydigi[kc] - fStartY;
        zdigi[kc] = zdigi[kc] - fStartZ;
    }
    
    // Now rotate untill shifted fStop is along the z-axis:
    TVector3 Stop;
    TVector3 Point;
    Stop.SetX(fStopX - fStartX);
    Stop.SetY(fStopY - fStartY);
    Stop.SetZ(fStopZ - fStartZ);
    
    Double_t theta = Stop.Theta();
    Double_t phi = Stop.Phi();
    
    for (Int_t kc = 0; kc<fSize; ++kc)
    {
        Point.SetX(xdigi[kc]);
        Point.SetY(ydigi[kc]);
        Point.SetZ(zdigi[kc]);
        Point.RotateZ(-1.0*phi);
        Point.RotateY(-1.0*theta);
        xdigi[kc] = Point.X();
        ydigi[kc] = Point.Y();
        zdigi[kc] = Point.Z();
    }

    Stop.RotateZ(-1.0*phi);
    Stop.RotateY(-1.0*theta);
    
    // Then now compute the quadratic sum:
    Double_t Qsum = 0.0;
    
    for (Int_t kc = 0; kc<fSize; ++kc)
    {
        Qsum = Qsum + xdigi[kc]*xdigi[kc];
        Qsum = Qsum + ydigi[kc]*ydigi[kc];
    }
    
    Qsum = Qsum/((Int_t) fSize);
    
    // Now perform the check:
    Double_t dist = TMath::Sqrt(Qsum);
    Bool_t Line = (dist<TopBoundary);
    
    // return:
    delete xdigi;
    delete ydigi;
    delete zdigi;
    delete tdigi;
    
    return Line;

    // Done!
}

// Function to check the topology of the clusters:
Bool_t R3BNeuLANDCluster::TopologyIsKink(TClonesArray* fDigis, Double_t const TopBoundary)
{
    // This function checks if the form of the cluster is
    // consistent with a line having a single kink.
    
    // Begin by defining the position arrays of the digis
    // inside the cluster:
    Int_t ndigis = fDigis->GetEntries();
    R3BLandDigi* digi;
    Double_t* xdigi = new Double_t[fSize];
    Double_t* ydigi = new Double_t[fSize];
    Double_t* zdigi = new Double_t[fSize];
    Double_t* tdigi = new Double_t[fSize];
    
    for (Int_t k = 0; k<ndigis; ++k)
    {
        digi = (R3BLandDigi*) fDigis->At(k);
        for (Int_t kc = 0; kc<fSize; ++kc)
        {
            if ((digi->GetPaddleNr()+1)==fDigiBars.at(kc))
            {
                xdigi[kc] = digi->GetXX();
                ydigi[kc] = digi->GetYY();
                zdigi[kc] = digi->GetZZ();
                tdigi[kc] = digi->GetTdc();
            }
        }
    }
    
    // Now the idea is that we draw two lines: one from the kink to
    // the startpoint and one from the kink to the stoppoint. Then we compute
    // the distance of each digi to one of these lines. We keep the smallest
    // of these two distances. Then we take a quadratic sum of all these
    // distances and divide it by the cluster size. If this number is
    // below the boundary, then we passed the test and we call teh topology
    // a kink. The tricky point is that any digi can basically be the kink.
    // so we simply need to try them all out...
    
    Double_t* Qsum = new Double_t[fSize];
    Double_t kink_index = -1;
    Double_t xkink = 0.0;
    Double_t ykink = 0.0;
    Double_t zkink = 0.0;
    Double_t tkink = 0.0;
    Double_t Qdist1 = 0.0;
    Double_t Qdist2 = 0.0;
    Double_t theta = 0.0;
    Double_t phi = 0.0;
    TVector3 Kink3;
    TVector3 Point;

    for (Int_t kink = 0; kink<fSize; ++kink)
    {
        Qsum[kink] = 0.0;
        xkink = xdigi[kink];
        ykink = ydigi[kink];
        zkink = zdigi[kink];
        
        // Now check the distances from all digis to one of the lines:
        for (Int_t k = 0; k<fSize; ++k)
        {
            // Fisrt line:
            Point.SetX(xdigi[k] - fStartX);
            Point.SetY(ydigi[k] - fStartY);
            Point.SetZ(zdigi[k] - fStartZ);
            
            Kink3.SetX(xkink - fStartX);
            Kink3.SetY(ykink - fStartY);
            Kink3.SetZ(zkink - fStartZ);
            
            theta = Kink3.Theta();
            phi = Kink3.Phi();
            
            Point.RotateZ(-1.0*phi);
            Point.RotateY(-1.0*theta);
            
            Qdist1 = Point.X()*Point.X() + Point.Y()*Point.Y();
            
            // Second line:
            Point.SetX(xdigi[k] - fStopX);
            Point.SetY(ydigi[k] - fStopY);
            Point.SetZ(zdigi[k] - fStopZ);
            
            Kink3.SetX(xkink - fStopX);
            Kink3.SetY(ykink - fStopY);
            Kink3.SetZ(zkink - fStopZ);
            
            theta = Kink3.Theta();
            phi = Kink3.Phi();
            
            Point.RotateZ(-1.0*phi);
            Point.RotateY(-1.0*theta);
            
            Qdist2 = Point.X()*Point.X() + Point.Y()*Point.Y();
            
            // Add to the quadratic sum:
            if (Qdist1<Qdist2) {Qsum[kink] = Qsum[kink] + Qdist1;}
            else {Qsum[kink] = Qsum[kink] + Qdist2;}
        }
    }
    
    // Now we need to find the minimum of all the Qsum values:
    Double_t MinQsum = 1e99;
    Int_t mink = -1;
    
    for (Int_t kink = 0; kink<fSize; ++kink)
    {
        if (Qsum[kink]<MinQsum)
        {
            MinQsum = Qsum[kink];
            mink = kink;
        }
    }
    
    // make decision:
    Bool_t Test = (MinQsum<TopBoundary);
    
    // Load the minimum into the edge:
    if (Test)
    {
      fEdgeX = xdigi[mink];
      fEdgeY = ydigi[mink];
      fEdgeZ = zdigi[mink];
      fEdgeT = tdigi[mink];
    }
    
    // Done:
    delete xdigi;
    delete ydigi;
    delete zdigi;
    delete tdigi;
    delete Qsum;
    
    return Test;
}
    
// Function to check the topology of the clusters:
Bool_t R3BNeuLANDCluster::TopologyIsPitchFork(TClonesArray* fDigis, Double_t const SpaceRadius, Double_t const TimeRadius)
{
    // This function checks if the form of the cluster is
    // consistent with a PitchFork. (one line going to two)
    
    // Begin by defining the position arrays of the digis
    // inside the cluster:
    Int_t ndigis = fDigis->GetEntries();
    R3BLandDigi* digi;
    Double_t* xdigi = new Double_t[fSize];
    Double_t* ydigi = new Double_t[fSize];
    Double_t* zdigi = new Double_t[fSize];
    Double_t* tdigi = new Double_t[fSize];
    
    for (Int_t k = 0; k<ndigis; ++k)
    {
        digi = (R3BLandDigi*) fDigis->At(k);
        for (Int_t kc = 0; kc<fSize; ++kc)
        {
            if ((digi->GetPaddleNr()+1)==fDigiBars.at(kc))
            {
                xdigi[kc] = digi->GetXX();
                ydigi[kc] = digi->GetYY();
                zdigi[kc] = digi->GetZZ();
                tdigi[kc] = digi->GetTdc();
            }
        }
    }
    
    // Now the idea is that we first check the structure of the cluster.
    // for a pitchfork there should be exactly one digi having 3 neighbours
    // and the others should all have 1 or 2.

    Int_t* Neighbours = new Int_t[fSize];
    Int_t Next = 0;
    Bool_t xcheck = kFALSE;
    Bool_t ycheck = kFALSE;
    Bool_t zcheck = kFALSE;
    Bool_t tcheck = kFALSE;
    
    for (Int_t k1 = 0; k1<fSize; ++k1)
    {
        Next = 0;
        
        // Check neighbours:
        for (Int_t k2 = 0; k2<fSize; ++k2)
        {
            if (k2!=k1)
            {
                // check if k2 is neighbour from k1:
                xcheck = (TMath::Abs(xdigi[k1] - xdigi[k2])<SpaceRadius);
                ycheck = (TMath::Abs(ydigi[k1] - ydigi[k2])<SpaceRadius);
                zcheck = (TMath::Abs(zdigi[k1] - zdigi[k2])<SpaceRadius);
                tcheck = (TMath::Abs(tdigi[k1] - tdigi[k2])<TimeRadius);
                
                if (xcheck&&ycheck&&zcheck&&tcheck) {Next = Next + 1;}
            }
        }
        
        // Fill the array:
        Neighbours[k1] = Next;
    }
    
    // Now find the number of points with 3 neighbours:
    Int_t n3 = 0;
    Bool_t pitchTest = kTRUE;
    Int_t index = -1;
    
    for (Int_t k1 = 0; k1<fSize; ++k1)
    {
        if (Neighbours[k1]>3) {pitchTest = kFALSE;}
        if (Neighbours[k1]==3) {n3 = n3 + 1; index = k1;}
    }
    if (n3>1) {pitchTest = kFALSE;}
    if (n3<1) {pitchTest = kFALSE;}
    
    // Now in principle we would also have to continue with our line test,
    // but we will not do that. Instead, we will simply return results now:
    
    if (pitchTest)
    {
        fEdgeT = tdigi[index];
        fEdgeX = xdigi[index];
        fEdgeY = ydigi[index];
        fEdgeZ = zdigi[index];
    }
    
    delete tdigi;
    delete xdigi;
    delete ydigi;
    delete zdigi;
    delete Neighbours;
    
    return pitchTest;
    
    // Done!
}
        
// Function to check the topology of the clusters:
Bool_t R3BNeuLANDCluster::TopologyIsCross(TClonesArray* fDigis, Double_t const SpaceRadius, Double_t const TimeRadius)
{
    // This function checks if the form of the cluster is
    // consistent with a Cross: two lines intersecting.
    
    // Begin by defining the position arrays of the digis
    // inside the cluster:
    Int_t ndigis = fDigis->GetEntries();
    R3BLandDigi* digi;
    Double_t* xdigi = new Double_t[fSize];
    Double_t* ydigi = new Double_t[fSize];
    Double_t* zdigi = new Double_t[fSize];
    Double_t* tdigi = new Double_t[fSize];
    
    for (Int_t k = 0; k<ndigis; ++k)
    {
        digi = (R3BLandDigi*) fDigis->At(k);
        for (Int_t kc = 0; kc<fSize; ++kc)
        {
            if ((digi->GetPaddleNr()+1)==fDigiBars.at(kc))
            {
                xdigi[kc] = digi->GetXX();
                ydigi[kc] = digi->GetYY();
                zdigi[kc] = digi->GetZZ();
                tdigi[kc] = digi->GetTdc();
            }
        }
    }
    
    // Now the idea is that we first check the structure of the cluster.
    // for a pitchfork there should be exactly one digi having 4 neighbours
    // and the others should all have 1 or 2.

    Int_t* Neighbours = new Int_t[fSize];
    Int_t Next = 0;
    Bool_t xcheck = kFALSE;
    Bool_t ycheck = kFALSE;
    Bool_t zcheck = kFALSE;
    Bool_t tcheck = kFALSE;
    
    for (Int_t k1 = 0; k1<fSize; ++k1)
    {
        Next = 0;
        
        // Check neighbours:
        for (Int_t k2 = 0; k2<fSize; ++k2)
        {
            if (k2!=k1)
            {
                // check if k2 is neighbour from k1:
                xcheck = (TMath::Abs(xdigi[k1] - xdigi[k2])<SpaceRadius);
                ycheck = (TMath::Abs(ydigi[k1] - ydigi[k2])<SpaceRadius);
                zcheck = (TMath::Abs(zdigi[k1] - zdigi[k2])<SpaceRadius);
                tcheck = (TMath::Abs(tdigi[k1] - tdigi[k2])<TimeRadius);
                
                if (xcheck&&ycheck&&zcheck&&tcheck) {Next = Next + 1;}
            }
        }
        
        // Fill the array:
        Neighbours[k1] = Next;
    }
    
    // Now find the number of points with 4 neighbours:
    Int_t n4 = 0;
    Bool_t CrossTest = kTRUE;
    Int_t index = -1;
    
    for (Int_t k1 = 0; k1<fSize; ++k1)
    {
        if (Neighbours[k1]>4) {CrossTest = kFALSE;}
        if (Neighbours[k1]==3) {CrossTest = kFALSE;}
        if (Neighbours[k1]==4) {n4 = n4 + 1; index = k1;}
    }
    if (n4>1) {CrossTest = kFALSE;}
    if (n4<1) {CrossTest = kFALSE;}
    
    // Now in principle we would also have to continue with our line test,
    // but we will not do that. Instead, we will simply return results now:
    
    if (CrossTest)
    {
        fEdgeT = tdigi[index];
        fEdgeX = xdigi[index];
        fEdgeY = ydigi[index];
        fEdgeZ = zdigi[index];
    }
    
    delete tdigi;
    delete xdigi;
    delete ydigi;
    delete zdigi;
    delete Neighbours;
    
    return CrossTest;
    
    // Done!
}
      
// Check the Topology of the cluster:
void R3BNeuLANDCluster::IdentifyTopology(TClonesArray* fDigis, Double_t const SpaceRadius, Double_t const TimeRadius, Double_t const TopBoundary)
{
    // This function performs all the topology checks on the cluster:
    
    // We check the topologies one-by-one. This is because our tests do not
    // exclude other possibilities.
    Int_t Topology = 0;
    
    // First the singleton test:
    if (fSize==0)
    {
        Topology = 7;
    }
    else if (fSize==1)
    {
        Topology = 6;
    }
    else
    {
        // Now check for the cross:
        Bool_t Cross = R3BNeuLANDCluster::TopologyIsCross(fDigis,SpaceRadius,TimeRadius);
        
        if (Cross)
        {
            Topology = 4;
        }
        else
        {
            // Check for the pitchfork:
            Bool_t Pitch = R3BNeuLANDCluster::TopologyIsPitchFork(fDigis,SpaceRadius,TimeRadius);
            
            if (Pitch)
            {
                Topology = 3;
            }
            else
            {
                // Check for the straight line:
                Bool_t Line = R3BNeuLANDCluster::TopologyIsLine(fDigis,TopBoundary);
                
                if (Line)
                {
                    Topology = 1;
                }
                else
                {
                    // Check for the kink:
                    Bool_t Kink = R3BNeuLANDCluster::TopologyIsKink(fDigis,TopBoundary);
                    
                    if (Kink)
                    {
                        Topology = 2;
                    }
                    else
                    {
                        // No more options:
                        Topology = 5;
                    }
                }
            }
        }
    }
    
    // Done!
    fTopology = Topology;
}