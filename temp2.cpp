#include "llvm/Pass.h"//
#include "llvm/IR/Function.h"//
#include "llvm/IR/Module.h"//
#include "llvm/Support/raw_ostream.h"//
#include "llvm/IR/Type.h"//
#include "llvm/IR/CallSite.h"//
#include "llvm/Analysis/CFG.h"
#include "llvm/IR/Instructions.h"//
#include "llvm/IR/InstIterator.h"//
#include "llvm/IR/Constants.h"//
#include "temp2.hpp"
#include "cont_tra.hpp"
using namespace llvm;
using namespace std;
void tempo(Module &M,Function &f,vector <BasicBlock*> &BBclonef,vector <BasicBlock*> &BBcloned,vector <BasicBlock*> &BBclonel)
  {
  vector<BasicBlock*> contt,tran,rep;
  vector<Instruction*> copy,original;
  LLVMContext &context = M.getContext();
  Instruction*I,*br,*I1;
  BasicBlock *bt;
  int a=0; //
  //************************************************************************
  if(inst_begin(f)!=inst_end(f))
    {      
    for (Function::iterator blocdebase = f.begin(), e = f.end(); blocdebase != e; ++blocdebase)
      {
      if(pred_begin(blocdebase)==pred_end(blocdebase))
        {
        rep.push_back(blocdebase);// take all the blocks 
        }
      }
    int e= rep.size()-1; 
    bt = BasicBlock::Create(context, "first", &f,f.begin());//block that determines the way
    //*********************************create copy of the each value********************* 
    for(int i=0;i<e;i++)
      {
      for (BasicBlock::iterator instruction = rep[i]->begin(), ee = rep[i]->end(); instruction != ee; ++instruction)
        {
        if(AllocaInst *al=dyn_cast<AllocaInst>(instruction))
          {
          Instruction *neww=instruction->clone();
          bt->getInstList().push_back(neww);
          original.push_back(neww);
          AllocaInst *new_alloca = new AllocaInst(al->getAllocatedType(),0, al->getAlignment(), al->getName(),bt);
          copy.push_back(new_alloca);
          al->replaceAllUsesWith(new_alloca);
          }
        }  
      }
      //************************************************
    BranchInst::Create(rep[0],bt);        
    transform(M,f,BBclonef,BBcloned,original,copy);  
    transform(M,f,BBcloned,BBclonel,original,copy);    
    }
   }

