#include <iostream>

#include "Inputs_Load.h"
#include "Leaf_Connect.h"
#include "Tree_Definition.h"

using namespace std;

void Test()
{
  Int_t counter = 1;

  Leaf_Structure TheLeafs;
  Leaf_Structure* p_TheLeafs = &TheLeafs;
  Bool_t connect = Leaf_Connect(p_TheLeafs,counter);
  
  Inputs_Structure Inputs;
  Inputs_Structure* p_Inputs = &Inputs;
  Bool_t Load = Inputs_Load(p_Inputs,counter);
  
  Tree_Structure AnaTree;
  Tree_Structure* p_AnaTree = &AnaTree;
  Bool_t Define = Tree_Definition(p_AnaTree,counter);
  
  cout << "Connection boolian = " << connect << "\n";
  cout << "Loading boolian = " << Load << "\n";
  cout << "Definition boolian = " << Define << "\n";
  
}