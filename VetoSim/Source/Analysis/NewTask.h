void NewTask(Leaf_Structure* const TheLeafs, Inputs_Structure* const Inputs, Tree_Structure* AnaTree, Int_t const Event, Int_t const Counter, TRandom3* generator)
{
  // This function does...
  
  // ... within a single event and puts the results
  // into the analysis tree. Therefore no outputs are required. The inputs are the pointers
  // to the 3 structures that hold all our information and the number of the current event.
  // We also pass the TRandom3 generator along as non-=const so that seeds are updated every time. Do not delete it!
  
  // NOTE: AnaTree should NOT be const, otherwise we cannot modify the structure.
  
  // Template is Written by C. A. Douma, for the R3B collaboration
    
  // =============================================================================================
  // Before starting your calculations, the values of the variables holding
  // information that will be put into the tree should be reset. This should be
  // done for single valued leafs and for static arrays:
  
  // NOTE: AnaTree->NewTask_SingleLeaf = 0;
  // NOTE: for (Int_t k = 0; k<(AnaTree->StaticSize); ++k)
  // NOTE: {
  // NOTE:    AnaTree->NewTask_ArrayLeaf[k] = 0;
  // NOTE: }
  
  // =============================================================================================
  // NOTE: Do your calculations here. 
  
  // Data from the Simulation output can be accessed with commands like:
  // Int_t Npoints = TheLeafs->LandPoint_fLightYield->GetNdata() ===> Get the size of this leaf.
  // Double_t light = TheLeafs->LandPoint_fLightYield->GetValue(3) ==> Gets the 4th value from the leaf.
  // Check the Leaf_Structure.h for all available leafs.
  
  // Input parameters can be accessed with expressions like Inputs->nEvents
  
  // ...
  
  
  // =============================================================================================
  // Now that the calculations are done, the results have to be put into the Analysis Tree.
  // since some of your leafs might be dynamical arrays, one first has to
  // re-allocate the arrays. Then, a SetBranchAddress should be performed, 
  // because the delete-operations destroys the link to the Analysis Tree. Only after all this,
  // the results of your calculations can be put into the Dynamical arrays. Then this
  // will be filled into the tree automatically. The delete commands that avoid memory leaks
  // should be put into the DeleteEverything function.
  
  // So first define the length of our arrays:
  // NOTE: Int_t ArrayLength = 1;
  
  // Then put this into the leaf that holds the size of these dynamical arrays:
  // NOTE: AnaTree->NewTask_Size = ArrayLength;
  
  // Then re-allocate the arrays:
  // NOTE: AnaTree->NewTask_Array = new Int_t[ArrayLength];
  
  // Then re-set the Branch Addresses:
  // NOTE: AnaTree->TheTree->SetBranchAddress("NewTask_Array",(void*) AnaTree->NewTask_Array);
  
  // Now put the results of your calculations into the arrays you just
  // declared with new:
  
  // NOTE: The delete commands should be put in the DeleteEverything-function.
  
  // =============================================================================================
  // And now we are done. All variables that we declared here without new are lost
  // after this function is complete, and that is how we like it (memory leaks)! The results are
  // NOT lost, since we put them into the arrays that are part of the AnaTree-structure.
  // here we only work with a pointer to this structure, so the results are put
  // in our structure outside this function. Then outside this function, the
  // command tree->Fill() is given and results are store. You are done now.
}