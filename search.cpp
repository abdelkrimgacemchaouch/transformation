#include "llvm/Pass.h"//
#include "llvm/IR/Function.h"//
#include "llvm/IR/Module.h"//
#include "llvm/Support/raw_ostream.h"//
#include "llvm/IR/Type.h"//
#include "llvm/IR/Instructions.h"//
#include "search.hpp"
#include <iostream>
#include <fstream>
using namespace llvm;
using namespace std;
int Search(vector<BasicBlock*> F ,BasicBlock *D){
  int i,j;
  bool f=false;
  i=0; j=0;
  while(i<F.size() && f==false)
    {
    if (F[i] == D) 
      {
      f=true;
      }
    else { j++;}
    i++;   
    }
return j;
}
//*****************************************************************************************
int Searchinst(vector<Instruction*> I,Instruction *inst )
  {
  int i,j;
  bool f=false;
  i=0; j=0;
  while(i<I.size() && f==false)
    {
    if (I[i] == inst) 
      {
      f=true;
      }
    else { j++;}
    i++;   
    }
 if(f==true)
  {
  return j;
  }
 else 
  {
  return (j+1);
  }
  
  }
//********************************************************************************************
int Searcharg(vector<Argument*> I,Argument *inst )
  {
  int i,j;
  bool f=false;
  i=0; j=0;
  while(i<I.size() && f==false)
    {
    if (I[i] == inst) 
      {
      f=true;
      }
    else { j++;}
    i++;   
    }
 if(f==true)
  {
  return j;
  }
 else 
  {
  return (j+1);
  } 
  }

