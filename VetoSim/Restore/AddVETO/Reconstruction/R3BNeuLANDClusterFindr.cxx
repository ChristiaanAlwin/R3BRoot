// R3B NeuLAND Cluster Finder cxx-file. Written by C. A. Douma.

// inclusion of required C++ headers:

// Inclusion of the required ROOT headers:

// Inclusion of the required FAIR headers:

// Inclusion of the required R3B headers:

// Inclusion of the own header file:
#include "R3BNeuLANDClusterFindr.h"

// Using standard namespaces:
using namespace std;

// Auxilliary function for sorting digis in time 
// to be used with std::sort from <algorithm>
bool SortDigiByTime(const R3BNeulandDigi *digi1, const R3BNeulandDigi *digi2)
{
  if(digi1->GetT() < digi2->GetT()) {
    return true;
  } else {
    return false;
  }
}

// Default Constructor:
R3BNeuLANDClusterFindr::R3BNeuLANDClusterFindr()
  : FairTask("R3BNeuLANDClusterFindr")
{
  fArrayCluster = new TClonesArray("R3BNeuLANDClustr");
  fSpaceRadius = 7.5;
  fTimeRadius = 1.0;
  fTopBoundary = 5.0;
  fEmax_hist = 1500.0;
  fNplanes = 50;
  fNClmax_hist = 50;
}

// Destructor:
R3BNeuLANDClusterFindr::~R3BNeuLANDClusterFindr()
{
  if (fArrayCluster) 
  {
    fArrayCluster->Clear();
    delete fArrayCluster;
  }
}

// NOTE: FairTask initialization function:
InitStatus R3BNeuLANDClusterFindr::Init()
{
  // Each FairTask begins with defining the input/output manager:
  FairRootManager* ioman = FairRootManager::Instance();
  
  // Chech that it worked:
  if (NULL == ioman) 
  {
    cout << "### FATAL: No FairRootManager created!\n";
    return kFATAL;
  }

  // Connect to the LandDigis:
  fArrayDigi = (TClonesArray*) ioman->GetObject("LandDigi");
  if (fArrayDigi == nullptr) {fArrayDigi = (TClonesArray*) ioman->GetObject("NeulandDigis");}
  
  // Again: Check that it worked:
  if(NULL == fArrayDigi) 
  {
    cout << "### FATAL: No Digitizer output detected!\n";
    return kFATAL;
  }

  // Register the output array:
  ioman->Register("NeuLANDCluster","Clusters in NeuLAND",fArrayCluster,kTRUE);

  // Initialize control histograms
  CreateHistograms();
  
  // Done.
  return kSUCCESS;
}

// NOTE: FairTask Execute-function:
void R3BNeuLANDClusterFindr::Exec(Option_t *option)
{
  // First reset the previous event:
  fArrayCluster->Clear();
  fVectorDigi.clear();
  
  // Re-fill the digi-vector:
  Int_t nDigis = fArrayDigi->GetEntries();
  R3BNeulandDigi *digi;
  
  for (Int_t k = 0; k<nDigis; k++) 
  {
    digi = (R3BNeulandDigi*) fArrayDigi->At(k);
    fVectorDigi.push_back(digi);
  }

  // Then sort the digis in time:
  sort(fVectorDigi.begin(), fVectorDigi.end(), SortDigiByTime);
  nDigis = fVectorDigi.size();
       
  // Now we will have to actually find the clusters.
  // In order to do that we will give every NeuLAND bar an index to which
  // cluster they belong:
  Int_t Nbars = 50*fNplanes;
  Int_t* belongsToCluster = new Int_t[Nbars];
  
  for(Int_t k = 0; k<Nbars; k++) 
  {
    belongsToCluster[k] = -1; // means: no cluster!
  }
  Int_t nClusters = 0;
  
  // Now we will prepare to loop over the digis and fill the clusters.
  // Declare what we need:
  R3BNeulandDigi* digi1;
  R3BNeulandDigi* digi2;
  R3BNeuLANDClustr* Cluster;
  TVector3 pos;
  Double_t posx1 = 0.0;
  Double_t posy1 = 0.0;
  Double_t posz1 = 0.0;
  Double_t post1 = 0.0;
  Double_t posx2 = 0.0;
  Double_t posy2 = 0.0;
  Double_t posz2 = 0.0;
  Double_t post2 = 0.0;
  Double_t rad = 0.0;
  Int_t clusNo1 = 0;
  Int_t clusNo2 = 0;
  
  if ((nDigis>0)&&(nDigis<Nbars))
  {  
    for (Int_t k1 = 0; k1<nDigis; ++k1)
    {
      for (Int_t k2 = 0; k2<nDigis; ++k2)
      {
          // load the digis:
          digi1 = (R3BNeulandDigi*) fVectorDigi.at(k1);
          digi2 = (R3BNeulandDigi*) fVectorDigi.at(k2);
          
          // request poistion & time of the digis:
          pos = digi1->GetPosition();
          posx1 = pos.X();
          posy1 = pos.Y();
          posz1 = pos.Z();
          post1 = digi1->GetT();  
        
          pos = digi2->GetPosition();
          posx2 = pos.X();
          posy2 = pos.Y();
          posz2 = pos.Z();
          post2 = digi2->GetT();  
          
          // Compute spatial radius:
          rad = TMath::Abs(posx1-posx2);
          if (TMath::Abs(posy1-posy2)>rad) {rad = TMath::Abs(posy1-posy2);}
          if (TMath::Abs(posz1-posz2)>rad) {rad = TMath::Abs(posz1-posz2);}
          
          // Check if they are neighbours:
	  if ((rad<fSpaceRadius)&&(TMath::Abs(post1-post2)<fTimeRadius)&&(k1!=k2)) // if k1==k2 it is the same poit --> pointless!
          {
              // This is a neighbour. Now there basically are 4 different situations:
              // 1: both belongsToCluster-values are equal to -1 ==> both are not assigned to a new cluster. ==> create a new cluster based on these two points.
              // 2: i belongs to a cluster while k does not. Add k to the cluster. Notice that time(i)<time(k) always!
              // 3: k belongs to a cluster while i does not. Add i to the cluster. Notice that time(i)<time(k) always!
              // 4: both i and k belong to a cluster.
              //    ==> check if it is the same cluster. if this is the case, do nothing. We are fine!
              //    ==> if they are neigbours & belong to different clusters ==> merge the clusters!
            
              // So we first check for all situations:
              if ((belongsToCluster[k1]==-1)&&(belongsToCluster[k2]==-1))
              {
                  // Then we must create a new cluster:
                  new ((*fArrayCluster)[nClusters]) R3BNeuLANDClustr(digi1,digi2);
                  
                  // And write this to the digis:
                  belongsToCluster[k1] = nClusters;
                  belongsToCluster[k2] = nClusters;
                  nClusters = nClusters + 1;
              }
              else if ((belongsToCluster[k1]>-1)&&(belongsToCluster[k2]==-1))
              {
                  // Then we must add k2 to k1:
                  clusNo1 = belongsToCluster[k1];
                  belongsToCluster[k2] = clusNo1;
              
                  // update the cluster:
                  Cluster = (R3BNeuLANDClustr*) fArrayCluster->At(clusNo1);
                  Cluster->AddDigi(digi2);
                  
                  // No need to write the cluster back to teh array.
              }
              else if ((belongsToCluster[k1]==-1)&&(belongsToCluster[k2]>-1))
              {
                  // Then we must add k1 to k2:
                  clusNo2 = belongsToCluster[k2];
                  belongsToCluster[k1] = clusNo2;
              
                  // update the cluster:
                  Cluster = (R3BNeuLANDClustr*) fArrayCluster->At(clusNo2);
                  Cluster->AddDigi(digi1);
                  
                  // No need to write the cluster back to the array.
              }
              else if ((belongsToCluster[k1]>-1)&&(belongsToCluster[k2]>-1)&&(belongsToCluster[k1]!=belongsToCluster[k2]))
              {
                // Then we must merge the clusters.
                clusNo1 = belongsToCluster[k1];
                clusNo2 = belongsToCluster[k2];
  
                Cluster = (R3BNeuLANDClustr*) fArrayCluster->At(clusNo1);
                Cluster->Merge((R3BNeuLANDClustr*) fArrayCluster->At(clusNo2),fArrayDigi);
                
                // And now we must eliminate the second cluster, by replacing
                // it with the end cluster in the array:
                Cluster = (R3BNeuLANDClustr*) fArrayCluster->At(clusNo2);
                Cluster->Replace((R3BNeuLANDClustr*) fArrayCluster->At(nClusters-1)); // NOTE: Need n-1 since nClusters is not a C++ index!
                
                // Then delete the cluster at the end:
                fArrayCluster->ExpandCreate(nClusters);
                
                // Now update our cluster indices for th merging:
                for (Int_t subi = 0; subi<nDigis; subi++) 
                {
                  if (belongsToCluster[subi]==clusNo2) {belongsToCluster[subi] = clusNo1;}
                }
                
                // Seperate loop not to mix things up:
                for (Int_t subi = 0; subi<nDigis; subi++) 
                {
                  if (belongsToCluster[subi]==(nClusters-1)) {belongsToCluster[subi] =clusNo2;}
                }
                            
                // update the number of clusters:
                nClusters = nClusters - 1;
                
                // Done with merging:
              }
          }
          
          for (Int_t k = 0; k<nClusters; ++k)
          {
              Cluster = (R3BNeuLANDClustr*) fArrayCluster->At(clusNo1);
              if (Cluster->GetNbars()==0) {Cluster->Print();}
              //cout << "-------------------------- End of Event ----------------------------\n\n\n\n";
          }
              
          
      }
    }
  }
                
  // Now we are almost done. We should now create clusters for the digis
  // that have no neighbours at all. Loop over all sorted digis:
  Bool_t NoNeighbours = kFALSE;
  
  if ((nDigis>0)&&(nDigis<Nbars))
  {
    for (Int_t i = 0; i<nDigis; i++) 
    {
      // Get pointer to 1-st digi
      digi1 = (R3BNeulandDigi*) fVectorDigi.at(i);
      
      // Store position & time:
      pos = digi1->GetPosition();
      posx1 = pos.X();
      posy1 = pos.Y();
      posz1 = pos.Z();
      post1 = digi1->GetT();        
      
      // re-set boolian for the check:
      NoNeighbours = kTRUE;
      
      // loop over all possible neighbours:
      for (Int_t k = 0; k < nDigis; k++) 
      {
	// Get pointer to 2-nd digi:
	digi2 = fVectorDigi.at(k);
        
        // store position & time:
        pos = digi2->GetPosition();
        posx2 = pos.X();
        posy2 = pos.Y();
        posz2 = pos.Z();
        post2 = digi2->GetT();  
        
        // Check if properly sorted:
	if ((post2<post1)&&(k>i))
        {
	  // 2-nd digi has earlier time. Stop here.
	  Fatal("Exec()", "### ERROR: Sorting of digis failed.");
	}
	
	// Compute spatial radius.
	rad = TMath::Abs(posx1-posx2);
        if (TMath::Abs(posy1-posy2)>rad) {rad = TMath::Abs(posy1-posy2);}
        if (TMath::Abs(posz1-posz2)>rad) {rad = TMath::Abs(posz1-posz2);}
        
        // Check that digi1 has no neighbours:        
        if ((rad<fSpaceRadius)&&(TMath::Abs(post1-post2)<fTimeRadius)&&(i!=k)) {NoNeighbours = kFALSE;}
      }
      
      // Then check if this digi indeed has no neighbours:
      if (NoNeighbours)
      {
          // Then create a new cluster:
          new ((*fArrayCluster)[nClusters]) R3BNeuLANDClustr(digi1);
          belongsToCluster[i] = nClusters;
          nClusters = nClusters + 1;
      }
    }
    
    // Now check that indeed all digis have been assigned to clusters:
    for (Int_t i = 0; i<nDigis; i++) 
    {
        if (belongsToCluster[i]==-1)
        {
            Fatal("Exec()", "### ERROR: Some digis were not assigned to clusters!");
        }
    }
  }
        
  // Now in principle we are done now. However, we will now determine the topology of all
  // the clusters:
  Double_t Etot = 0.0;
    
  if (nClusters>0)
  {
      for (Int_t k = 0; k<nClusters; ++k)
      {
          Cluster = (R3BNeuLANDClustr*) fArrayCluster->At(k);
          Cluster->IdentifyTopology(fArrayDigi,fSpaceRadius,fTimeRadius,fTopBoundary);
            
          // Fill histograms:
          h_Energy->Fill(Cluster->GetE());
          h_Size->Fill(Cluster->GetSize());
          h_Topology->Fill(Cluster->GetTopology());
            
          // sum up the energy:
          Etot = Etot + Cluster->GetE();
      }
  }

  // Last few histograms:
  h_NClusters->Fill(nClusters);
  h2_Calibration->Fill(nClusters,Etot);
        
  // Done! registration of output will be done automatically!
}

// Create our histograms:
void R3BNeuLANDClusterFindr::CreateHistograms()
{
  h_NClusters = new TH1I("Number of Clusters","Number of Clusters",fNClmax_hist,0,fNClmax_hist);
  h_Energy = new TH1D("Energy in a cluster","Energy in a cluster",(Int_t) fEmax_hist,0.0,fEmax_hist);
  h_Size = new TH1I("Size of a cluster","Size of a cluster",50,0,50);
  h_Topology = new TH1I("Topology Type","Topology Type",8,0,7);
  h2_Calibration = new TH2D("Calibration","Calibration",(Int_t) fEmax_hist,0.0,fEmax_hist,(Int_t) fNClmax_hist,0.0,(Int_t) fNClmax_hist);
}

// Finishing of the task: Writing our histograms to the file:
void R3BNeuLANDClusterFindr::Finish()
{
  h_NClusters->Write("Number of Clusters",2);
  h_Energy->Write("Energy in a cluster",2);
  h_Size->Write("Size of a cluster",2);
  h_Topology->Write("Topology Type",2);
  h2_Calibration->Write("h_ncl_etot",2);
}
