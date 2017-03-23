#include <iostream>

#include "TFile.h"
#include "TTree.h"
#include "TBranch.h"
#include "TLeaf.h"
#include "TString.h"
#include "TObjDouble.h"
#include "TObjInt.h"
#include "TClonesArray.h"

using namespace std;

void TreeTest()
{
  // This macro creates and fills a tree. Some different leafs are tested out.
  
  // Written by C. A. Douma.
  
  // ===========================================================================
  
  // Creation of a file and a tree inside:
  TFile* f = new TFile("f.root","recreate");
  TTree* t = new TTree("Tree","Tree");
  
  // ==================================== Creating a simple leaf with 1 value per event:
  Int_t SingleValue = 0;
  t->Branch("SingleValue",&SingleValue,"SingleValue/I");
  // NOTE: Here is the trick. You first declare an integer/double/boolian, etc.
  // then you create a Branch in the tree with t->Branch(). The first argument
  // will be the name of the branch. If the branch is not given any additional
  // substructure, then this branch will become a leaf in the end.
  // the second argument is a reference to the integer that you declared previously. 
  // Now, if you assign a value to the integer and then give the command t->Fill();
  // then the value from the integer is duplicated into the tree. ROOT
  // knows how to do this due to the reference you gave. The third argument
  // is again a string. It MUST begin with the name of the branch/leaf. If it
  // is then followed by an "/I", ROOT knows that it is a leaf of 1 value per
  // event and this value is of type Int_t. I=Int_1, D=Double_t, B=Bool_t, etc.
  // then each time you give t->Fill(), a new Event is created and filled.
  // This is all done automatically.
  
  // ===================================== Creating a leaf with an array of values:
  Int_t Sizes = 0;
  Int_t Array [10];
  t->Branch("Sizes",&Sizes,"Sizes/I");
  t->Branch("Array",&Array,"Array[Sizes]/I");
  // NOTE: This requires the creation of two leafs: one that holds the size
  // of the arrays and one that holds the values. The leaf that holds
  // the sizes MUST be a leaf of 1 value per event of type Int_t. Now to fill
  // the leafs, one must declare an integer to hold the sizes and an array of
  // fixed size that holds the values. then the size leaf gets a reference to
  // the size integer (just like before) and the Array leaf gets a reference
  // to the declared array. Now for the array leaf, the third argument is a 
  // string that holds first the name of the Array leaf, then a "[", then the
  // name of the Size-leaf, then a "]" and then followed by "/I" for an array of
  // integers. Replace I by D, B, etc if you like. due to this combination
  // of two leafs in the string, ROOT knows that there should be an array
  // of variable size in the leaf. Of course, the declared C++ array has fixed
  // size. But if you give a value to the size-integer that is smaller or equal 
  // to this, the leaf will get this size. Hence the C++ array should be the
  // maximum size of all the leaf-sizes. ROOT will do all of this automatically
  // so all you need to do is to give t->Fill();
  
  // ====================================== Leaf with std::vector
  std::vector<Int_t> vec;
  t->Branch("vec",&vec);
  // NOTE: this is just done in the same way as for a leaf of 1 element
  // per event. Only now this 1 element is a vector not an integer.
  // Since this is a complex object, the thrid argument in t->Branch()
  // should be dropped. Since this is 1 complex object and not an array,
  // ROOT will not know the size of the leaf. You always should first
  // retrieve the entire vector and then obtain numbers from it.
  
  // ====================================== Leaf with dynamic array:
  Int_t Dyn_Sizes = 0;
  Int_t* p_Array = 0; // This will become your array:
  t->Branch("Dyn_Sizes",&Dyn_Sizes,"Dyn_Sizes/I");
  t->Branch("Dyn_Array",p_Array,"Dyn_Array[Dyn_Sizes]/I");
  // NOTE: This will introduce some extra difficulties, but the advantage
  // of this method is that one does not need to know the maximum
  // of the leaf size in the beginning.
  // the extra difficulty is that due to the required delete, the link
  // from the dynamic array to the tree is lost every time, hence this
  // link has to be re-initialized every time with SetBranchAddress.  
  
  // ===============================================================================  
  // Filling the tree: NOTE: we put in 10 events here.
  
  for (Int_t k = 0; k<10; ++k)
  {
    // Preparation:
    vec.clear(); // clean your std::vector so it can hold a new event.
    p_Array = new Int_t[k+1]; // create your dynamic array here!
    
    // Filling the variables:    
    SingleValue = k+1; // loads the event number k into the SingleValue leaf.
    
    // load the event number in all elements of the arrays:
    Sizes = k+1; // Define the leaf size to be equal to the event number.
    Dyn_Sizes = k+1; // Same: define the size...
    for (Int_t i=0; i<(k+1); ++i)
    {
      vec.push_back(k+1); // Automatically adds the value after the last-nonzero 
      Array[i] = k+1; // Add the values to the array.
      p_Array[i] = k+1; // Add value to the dynamic array.
    }    
    
    // Now we have the dynamic array, but we need to to re-link it to the branch every time:
    t->SetBranchAddress("Dyn_Array",p_Array);
    
    // Now all variables are properly set, fill the tree:
    t->Fill();
    
    // In the end, delete the elements of the dynamic array (but not the pointer!)
    delete[] p_Array; // NOTE: this has to be at the end of the for-loop!
    
  }
  
  // Then finally, write the tree to the file:
  t->Write();
  
  // ==============================================================================
  // Reading out the tree:
  cout << "The Tree is succesfully filled and saved!\n\n";
  
  // Getting the events that we need:
  Long64_t nEvents = t->GetEntries();
  Long64_t nBytes = 0;
  
  // Connect to the leafs:
  TLeaf* l_SingleValue = t->FindLeaf("SingleValue");
  //TLeaf* l_Array = t->FindLeaf("Array");
  TLeaf* l_Array = t->FindLeaf("Dyn_Array");
  
  // declare the variables that we need:
  Int_t val_SingleValue = 0;
  Int_t val_Size = 0;
  Int_t val_ArrayValue = 0;
  
  // loop over the tree:
  for (Int_t k = 0; k<nEvents; ++k)
  {
    // Get the current event:
    nBytes = t->GetEntry(k);
    
    // Single value leaf:
    val_SingleValue = l_SingleValue->GetValue(0);
    cout << "SingleValue = " << val_SingleValue << " | ";
    
    // Array of values:
    val_Size = l_Array->GetNdata();
    cout << "Size = " << val_Size << " | ";
    for (Int_t i = 0; i<val_Size; ++i)
    {
      val_ArrayValue = l_Array->GetValue(i);
      cout << val_ArrayValue << " ";
    }
    
    cout << "\n";
  }
    
  // Done!

}

// Add the main:
#ifndef __CINT__
#include <TApplication.h>
int main(int argc, char** argv)
{
  TApplication* TheApp = new TApplication("TreeTest",&argc,argv);
  TreeTest();
  // TheApp->Run(); ==> This macro does not contain graphics.
  return 0;
}
#endif