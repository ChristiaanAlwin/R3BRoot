// -----------------------------------------------------------------------------
// -----                   R3BNeuLandClusterFinder                         -----
// -----     Created 2fneuland_ClusterTimeRadius2-2012 by D.Kresan         -----
// -----         The original algorithm is written by M.Heil               -----
// -----               Merging is added by C. A. Douma                     -----
// -----------------------------------------------------------------------------

#include <iostream>
#include <algorithm>

#include "TClonesArray.h"
#include "TMath.h"
#include "TH1F.h"
#include "TH2F.h"

#include "FairRootManager.h"

#include "R3BLandDigi.h"
#include "R3BNeuLandCluster.h"
#include "R3BNeuLandClusterFinder.h"


using std::cout;
using std::endl;




// Auxilliary function for sorting digis in time 
// to be used with std::sort
bool SortDigiByTime(const R3BLandDigi *digi1, const R3BLandDigi *digi2);



// -----------------------------------------------------------------------------
R3BNeuLandClusterFinder::R3BNeuLandClusterFinder()
  : FairTask("R3BNeuLandClusterFinder")
{
  // Default constructor

  // Set default verbosity level
  fVerbose = 1;
  
  // set default cluster form:
  fneuland_UseSphericalRadius = kFALSE;
  fneuland_DeBug = kFALSE;
  fneuland_Nplanes = 50;

  // Create array for storing the clusters
  fArrayCluster = new TClonesArray("R3BNeuLandCluster");
}
// -----------------------------------------------------------------------------



// -----------------------------------------------------------------------------
R3BNeuLandClusterFinder::R3BNeuLandClusterFinder(Int_t verbose)
  : FairTask("R3BNeuLandClusterFinder", verbose)
{
  // Standard constructor

  // Set verbosity level
  fVerbose = verbose;
  
  // set default cluster form:
  fneuland_UseSphericalRadius = kFALSE;
  fneuland_DeBug = kFALSE;
  fneuland_Nplanes = 50;

  // Create array for storing the clusters
  fArrayCluster = new TClonesArray("R3BNeuLandCluster");
}
// -----------------------------------------------------------------------------



// -----------------------------------------------------------------------------
R3BNeuLandClusterFinder::~R3BNeuLandClusterFinder()
{
  // Destructor

  // Delete the array of clusters (first clear the array)
  if(fArrayCluster) {
    fArrayCluster->Clear();
    delete fArrayCluster;
  }
}
// -----------------------------------------------------------------------------



// -----------------------------------------------------------------------------
InitStatus R3BNeuLandClusterFinder::Init()
{
  // Task initialization

  // Get pointer to I/O Manager
  FairRootManager *rootMgr = FairRootManager::Instance();
  // Check pointer
  if(NULL == rootMgr) {
    cout << "-E- R3BNeuLandClusterFinder::Init : illegal pointer to root manager."
	 << endl;
    // Stop here
    return kFATAL;
  }

  // Get pointer to LAND digi array
  fArrayDigi = (TClonesArray*) rootMgr->GetObject("LandDigi");
  // Check pointer
  if(NULL == fArrayDigi) {
    cout << "-E- R3BNeuLandClusterFinder::Init : no LandDigi array." << endl;
    // Stop here
    return kFATAL;
  }

  // Register the output array (will be written to the output file)
  rootMgr->Register("NeuLandCluster", "Digi clusters in NeuLAND",
		    fArrayCluster, kTRUE);

  // Initialize control histograms
  CreateHistograms();
  
  return kSUCCESS;
}
// -----------------------------------------------------------------------------

// NOTE: 3000 --> Nbars. Array --> to new-dynamical & delete!!!
// declaration of doubles!

// -----------------------------------------------------------------------------
void R3BNeuLandClusterFinder::Exec(Option_t *option)
{
  // Debug output:
  Bool_t debug = fneuland_DeBug;  
  if (debug) {std::cout << "############ NOTE: Start of ::Exec()-function.\n";}
    
  // Reset the previous event
  Reset();  
  
  // Prepare digis for sorting (fill the stl vector)
  Int_t nDigis = fArrayDigi->GetEntries();
  R3BLandDigi *digi1;
  R3BLandDigi *digi2;

  //loop over entries:  
  for (Int_t i = 0; i < nDigis; i++) 
  {
    digi1 = (R3BLandDigi*) fArrayDigi->At(i);
    fVectorDigi.push_back(digi1);
  }

  // Sort digis in time
  std::sort(fVectorDigi.begin(), fVectorDigi.end(), SortDigiByTime);
  nDigis = fVectorDigi.size();
       
  // find clusters and mark the position of the cluster by 
  // time of first hit, position of first hit, and total energy 
  Double_t posx1, posy1, posz1, post1, posx2, posy2, posz2, post2, rad;
  Int_t nClusters = 0;
  fhHits->Fill(nDigis);
 
  // Map from digi index to cluster index
  Int_t Nbars = 50*fneuland_Nplanes;
  Int_t* belongsToCluster = new Int_t[Nbars];
  
  for(Int_t i = 0; i < Nbars; i++) 
  {
    belongsToCluster[i] = -1;
  }
  
  // Declare variables outside of loop:
  R3BNeuLandCluster *first_cluster;
  R3BNeuLandCluster *second_cluster;
  R3BNeuLandCluster *end_cluster;
  Int_t oldPaddle;
  Int_t first_clusNo;
  Int_t second_clusNo;
  Bool_t NoNeighbours;
  
  if (debug) {std::cout << "--------- start of event ----------\nNClusters = " << nClusters << "\n";}
  if (debug) {std::cout << "Spatial Search radius = " << fneuland_ClusterSpaceRadius << " [cm] | Time Search radius = " << fneuland_ClusterTimeRadius << "[ns]\n";}
  
  // Check number of digis
  if (nDigis > 0 && nDigis <= Nbars) 
  {  
    if (debug) {std::cout << "Event passed the checks.\n";}
    
    // Loop over all sorted digis
    for (Int_t i = 0; i < nDigis; i++) 
    {
      // Get pointer to 1-st digi
      digi1 = fVectorDigi.at(i);
      
      // Store position & time:
      posx1 = digi1->GetXX();
      posy1 = digi1->GetYY();
      posz1 = digi1->GetZZ();
      post1 = digi1->GetTdc();        
      
      // loop over all possible neighbours:
      for (Int_t k = 0; k < nDigis; k++) 
      {
	// Get pointer to 2-nd digi:
	digi2 = fVectorDigi.at(k);
        
        // store position & time:
        posx2 = digi2->GetXX();
        posy2 = digi2->GetYY();
        posz2 = digi2->GetZZ();
        post2 = digi2->GetTdc();  

	// Check if properly sorted:
	if ((post2<post1)&&(k>i))
        {
	  // 2-nd digi has earlier time. Stop here.
	  Fatal("Exec()", "Sorting of digis failed.");
	}
	
	// Compute spatial radius. Use cubic form:
	rad = TMath::Abs(posx1-posx2);
        if (TMath::Abs(posy1-posy2)>rad) {rad = TMath::Abs(posy1-posy2);}
        if (TMath::Abs(posz1-posz2)>rad) {rad = TMath::Abs(posz1-posz2);}
	
	// If we want to use spherical form:
	if (fneuland_UseSphericalRadius)
        {   
	  rad = TMath::Sqrt((posx1-posx2)*(posx1-posx2) + (posy1-posy2)*(posy1-posy2) + (posz1-posz2)*(posz1-posz2));
        }
        
	// Check if neighbour:
	if ((rad<fneuland_ClusterSpaceRadius)&&(TMath::Abs(post1-post2)<fneuland_ClusterTimeRadius)&&(i!=k)) // if i=k it is the same poit --> pointless!
        {
	  // This is a neighbour. Now there basically are 4 different situations:
          // 1: both belongsToCluster-values are equal to -1 ==> both are not assigned to a new cluster. ==> create a new cluster based on these two points.
          // 2: i belongs to a cluster while k does not. Add k to the cluster. Notice that time(i)<time(k) always!
          // 3: k belongs to a cluster while i does not. Add i to the cluster. Notice that time(i)<time(k) always!
          // 4: both i and k belong to a cluster.
          //    ==> check if it is the same cluster. if this is the case, do nothing. We are fine!
          //    ==> if they are neigbours & belong to different clusters ==> merge the clusters!
            
          if (debug) {std::cout << "We found a pair of neighbours! NClusters = " << nClusters << "\n";}  
            
          // So we first check for all situations:
          if ((belongsToCluster[i]==-1)&&(belongsToCluster[k]==-1))
          {
              // Create a new cluster:
              if (k<i)
              {
              // reverse of next case
              new ((*fArrayCluster)[nClusters]) R3BNeuLandCluster(digi1->GetXX(), digi1->GetYY(), digi1->GetZZ(), digi1->GetTdc(),
							          digi2->GetXX(), digi2->GetYY(), digi2->GetZZ(), digi2->GetTdc(),
							          digi1->GetQdc()+digi2->GetQdc(), 2);
              }
              else if (k>i)
              {
              // then post2<post1 hence digi2<digi1
              new ((*fArrayCluster)[nClusters]) R3BNeuLandCluster(digi2->GetXX(), digi2->GetYY(), digi2->GetZZ(), digi2->GetTdc(),
							          digi1->GetXX(), digi1->GetYY(), digi1->GetZZ(), digi1->GetTdc(),
							          digi1->GetQdc()+digi2->GetQdc(), 2);
              }
              else
              {
                  Fatal("Exec()", "Wrong if-logic in your code!!!.");
              }
              
              belongsToCluster[i] = nClusters;
              belongsToCluster[k] = nClusters;
              nClusters = nClusters + 1;
              
              if (debug) {std::cout << "We created a new cluster! NClusters = " << nClusters << "\n";}
          }
          else if ((belongsToCluster[i]>-1)&&(belongsToCluster[k]==-1))
          {
              // Then add k to the cluster of i:
              first_clusNo = belongsToCluster[i];
              belongsToCluster[k] = first_clusNo;
              
              // update the cluster:
              first_cluster = (R3BNeuLandCluster*) fArrayCluster->At(first_clusNo);
              first_cluster->SetE(first_cluster->GetE() + digi2->GetQdc());
              first_cluster->SetSize(first_cluster->GetSize() + 1);
              
              // if the time of the new point is smaller then the first time,
              // or bigger then the last time, we should do more updates:
              if (digi2->GetTdc() > first_cluster->GetStopT())
              {
                first_cluster->SetStopT(digi2->GetTdc());
                first_cluster->SetStopPosition(digi2->GetXX(), digi2->GetYY(), digi2->GetZZ());
              }
              
              if (digi2->GetTdc() < first_cluster->GetStartT())
              {
                first_cluster->SetStartT(digi2->GetTdc());
                first_cluster->SetStartPosition(digi2->GetXX(), digi2->GetYY(), digi2->GetZZ());
              }
              
              // Update the array:
              //fArrayCluster->AddAt(first_cluster,first_clusNo);
              
              if (debug) {std::cout << "We updated a cluster! NClusters = " << nClusters << "\n";}
              
              // update is now done.
          }
          else if ((belongsToCluster[i]==-1)&&(belongsToCluster[k]>-1))
          {
              // Then add i to the cluster of k:
              first_clusNo = belongsToCluster[k];
              belongsToCluster[i] = first_clusNo;
              
              // update the cluster:
              first_cluster = (R3BNeuLandCluster*) fArrayCluster->At(first_clusNo);
              first_cluster->SetE(first_cluster->GetE() + digi1->GetQdc());
              first_cluster->SetSize(first_cluster->GetSize() + 1);
              
              // if the time of the new point is smaller then the first time,
              // or bigger then the last time, we should do more updates:
              if (digi1->GetTdc() > first_cluster->GetStopT())
              {
                first_cluster->SetStopT(digi1->GetTdc());
                first_cluster->SetStopPosition(digi1->GetXX(), digi1->GetYY(), digi1->GetZZ());
              }
              
              if (digi1->GetTdc() < first_cluster->GetStartT())
              {
                first_cluster->SetStartT(digi1->GetTdc());
                first_cluster->SetStartPosition(digi1->GetXX(), digi1->GetYY(), digi1->GetZZ());
              }
              
              // Update the array:
              //fArrayCluster->AddAt(first_cluster,first_clusNo);
              
              if (debug) {std::cout << "We updated a cluster! NClusters = " << nClusters << "\n";}
              
              // update is now done.
          }
          else if ((belongsToCluster[i]>-1)&&(belongsToCluster[k]>-1)&&(belongsToCluster[i]!=belongsToCluster[k]))
          {
              
              // merge clusters:
              first_clusNo = belongsToCluster[i];
              second_clusNo = belongsToCluster[k];
              
              // we do not care which index is bigger. That is not important. We will
              // first add the second cluster to the first cluster. Then we will move
              // cluster number nClusters to the index of the second cluster, which is now gone.
              // then we will lower NClusters by one.
              
              // Do actual merging:
              first_cluster = (R3BNeuLandCluster*) fArrayCluster->At(first_clusNo);
              second_cluster = (R3BNeuLandCluster*) fArrayCluster->At(second_clusNo);
              first_cluster->SetE(first_cluster->GetE() + second_cluster->GetE());
              first_cluster->SetSize(first_cluster->GetSize() + second_cluster->GetSize());
              
              // Update start and stop points:
              if (second_cluster->GetStartT()<first_cluster->GetStartT())
              {
                  first_cluster->SetStartT(second_cluster->GetStartT());
                  first_cluster->SetStartPosition(second_cluster->GetStartX(), second_cluster->GetStartY(), second_cluster->GetStartZ());
              }
              
              if (second_cluster->GetStopT()>first_cluster->GetStopT())
              {
                  first_cluster->SetStopT(second_cluster->GetStopT());
                  first_cluster->SetStopPosition(second_cluster->GetStopX(), second_cluster->GetStopY(), second_cluster->GetStopZ());
              }
              
              // Now we need to move the cluster at NClusters to the second cluster index.              
              end_cluster = (R3BNeuLandCluster*) fArrayCluster->At(nClusters-1); // NOTE: Need n-1 since nClusters is not a C++ index!
              
              // replace second cluster with the end-cluster:
              second_cluster->SetE(end_cluster->GetE());
              second_cluster->SetSize(end_cluster->GetSize());
              second_cluster->SetStartT(end_cluster->GetStartT());
              second_cluster->SetStartPosition(end_cluster->GetStartX(), end_cluster->GetStartY(), end_cluster->GetStartZ());
              second_cluster->SetStopT(end_cluster->GetStopT());
              second_cluster->SetStopPosition(end_cluster->GetStopX(), end_cluster->GetStopY(), end_cluster->GetStopZ());
              
              // update fArrayCluster:
              //fArrayCluster->AddAt(first_cluster,first_clusNo);
              //fArrayCluster->AddAt(second_cluster,second_clusNo);
              fArrayCluster->ExpandCreate(nClusters);
              
              // Give all digis of the second cluster a new cluster number.
              // What we need is that all digis of the second cluster obtain the first cluster number
              // and that all digis of the end cluster obtain the index of the second cluster.
              
              // replace seconds by firsts:
              for (Int_t subi = 0; subi < nDigis; subi++) 
              {
                  if (belongsToCluster[subi]==second_clusNo) {belongsToCluster[subi] = first_clusNo;}
              }
              
              // then in a separate loop, replace ends by seconds:
              for (Int_t subi = 0; subi < nDigis; subi++) 
              {
                  if (belongsToCluster[subi]==nClusters) {belongsToCluster[subi] = second_clusNo;}
              }
                            
              // update the number of clusters:
              nClusters = nClusters - 1;
              
              // And that should do the merging!
              if (debug) {std::cout << "####### We merged two clusters! NClusters = " << nClusters << "\n";} 
          }
          // else: do nothing: the neighbours already belong to the same cluster.
          
          // Now we did everything we needed to do. So now close all blocks:
        }
      }
    }
    
    // NOTE: We compared all different digis in pares. But we only created a cluster after we found
    // a matching pair. Hence we cannot have a cluster of only a single digi. We will now create those
    // clusters. We will check that such a digi indeed has no neighbours.
    
    // Loop over all sorted digis
    for (Int_t i = 0; i < nDigis; i++) 
    {
      // Get pointer to 1-st digi
      digi1 = fVectorDigi.at(i);
      
      // Store position & time:
      posx1 = digi1->GetXX();
      posy1 = digi1->GetYY();
      posz1 = digi1->GetZZ();
      post1 = digi1->GetTdc();        
      
      // re-set boolian for the check:
      NoNeighbours = kTRUE;
      
      // loop over all possible neighbours:
      for (Int_t k = 0; k < nDigis; k++) 
      {
	// Get pointer to 2-nd digi:
	digi2 = fVectorDigi.at(k);
        
        // store position & time:
        posx2 = digi2->GetXX();
        posy2 = digi2->GetYY();
        posz2 = digi2->GetZZ();
        post2 = digi2->GetTdc();  
        
        // Check if properly sorted:
	if ((post2<post1)&&(k>i))
        {
	  // 2-nd digi has earlier time. Stop here.
	  Fatal("Exec()", "Sorting of digis failed.");
	}
	
	// Compute spatial radius. Use cubic form:
	rad = TMath::Abs(posx1-posx2);
        if (TMath::Abs(posy1-posy2)>rad) {rad = TMath::Abs(posy1-posy2);}
        if (TMath::Abs(posz1-posz2)>rad) {rad = TMath::Abs(posz1-posz2);}
	
	// If we want to use spherical form:
	if (fneuland_UseSphericalRadius)
        {   
	  rad = TMath::Sqrt((posx1-posx2)*(posx1-posx2) + (posy1-posy2)*(posy1-posy2) + (posz1-posz2)*(posz1-posz2));
        }
        
        // Check that digi1 has no neighbours:        
        if ((rad<fneuland_ClusterSpaceRadius)&&(TMath::Abs(post1-post2)<fneuland_ClusterTimeRadius)&&(i!=k))
        {
            NoNeighbours = kFALSE;
        }
      }
      
      if (NoNeighbours)
      {
          // Then create a new cluster:
          new ((*fArrayCluster)[nClusters]) R3BNeuLandCluster(digi1->GetXX(), digi1->GetYY(), digi1->GetZZ(), digi1->GetTdc(),
							      digi1->GetXX(), digi1->GetYY(), digi1->GetZZ(), digi1->GetTdc(),
							      digi1->GetQdc(), 1);
          belongsToCluster[i] = nClusters;
          nClusters = nClusters + 1;
              
          if (debug) {std::cout << "We created a SINGLE cluster! NClusters = " << nClusters << "\n";}
      }
    }
    
    // Now check that indeed all digis have been assigned to clusters:
    for (Int_t i = 0; i < nDigis; i++) 
    {
        if (belongsToCluster[i]==-1)
        {
            Fatal("Exec()", "Some digis cannot be assigned to clusters!");
        }
        
        digi1 = fVectorDigi.at(i);
        
        if ((nClusters>1)&&(debug))
        {
            std::cout << " ### Digi Time = " << digi1->GetTdc() << "\n";
            std::cout << " ### Digi Xpos = " << digi1->GetXX() << "\n";
            std::cout << " ### Digi Ypos = " << digi1->GetYY() << "\n";
            std::cout << " ### Digi Zpos = " << digi1->GetZZ() << "\n";
        }
    }   
        
    if (debug) {std::cout << "------ end of event --------\n";}
        
    if (debug) {std::cout << "############ NOTE: Final status of event: NClusters = " << nClusters << "\n";}
    
    // Fill control histograms
    Int_t totalSize = 0;
    
    for(Int_t i = 0; i < nClusters; i++) 
    {
      first_cluster = (R3BNeuLandCluster*) fArrayCluster->At(i);
      fhClusterSize->Fill(first_cluster->GetSize());
      fhClusterEnergy->Fill(first_cluster->GetE());
      totalSize += first_cluster->GetSize();
    }
    
    fhClusters->Fill(nClusters);
    fhClusterNo_vs_Size->Fill(totalSize, nClusters);
  }
  
  
  // Control output
  if(fVerbose) 
  {
    cout << "-I- R3BNeuLandClusterFinder : found " << nClusters << " clusters." << endl;
  }
  
  // delete everything:
  delete belongsToCluster;
  
  // Done!
  if (debug) {std::cout << "############ NOTE: End of ::Exec()-function.\n";}
}
            
// -----------------------------------------------------------------------------



// -----------------------------------------------------------------------------
void R3BNeuLandClusterFinder::CreateHistograms()
{
  // Initialize control histograms

  fhClusterSize = new TH1F("hClusterSize", "number of paddles in a cluster", 500, -0.5, 499.5);
  fhClusterEnergy = new TH1F("hClusterEnergy", "Cluster energy", 5000, 0., 500.);
  fhHits = new TH1F("hHits", "Number of hits in one event", 500, -0.5, 499.5);
  fhClusters = new TH1F("hClusters", "Number of clusters in one event", 100, -0.5, 99.5);
  fhClusterNo_vs_Size = new TH2F("hClusterNo_vs_Size", "Number of Clusters vs. Total Size", 100, -0.5, 99.5, 100, -0.5, 99.5);
}
// -----------------------------------------------------------------------------



// -----------------------------------------------------------------------------
void R3BNeuLandClusterFinder::Finish()
{
  // Finish task execution

  // Write histograms to the output file
  fhClusterSize->Write();
  fhClusterEnergy->Write();
  fhHits->Write();
  fhClusters->Write();
  fhClusterNo_vs_Size->Write();
}
// -----------------------------------------------------------------------------



// -----------------------------------------------------------------------------
void R3BNeuLandClusterFinder::Reset()
{
  // Reset an event

  // Clear sorted vector
  fVectorDigi.clear();

  // Clear output array
  if(fArrayCluster) {
    fArrayCluster->Clear();
  }
}
// -----------------------------------------------------------------------------



// -----------------------------------------------------------------------------
bool SortDigiByTime(const R3BLandDigi *digi1, const R3BLandDigi *digi2)
{
  // Auxilliary function for sorting digis in time.
  // To be used with std::sort() from <algorithm>

  if(digi1->GetTdc() < digi2->GetTdc()) {
    return true;
  } else {
    return false;
  }
}
// -----------------------------------------------------------------------------




ClassImp(R3BNeuLandClusterFinder);


