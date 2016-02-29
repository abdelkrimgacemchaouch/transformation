#include "llvm/IR/Type.h"//
#include "llvm/IR/Instructions.h"//
#include "llvm/IR/InstIterator.h"//
#include "llvm/IR/Constants.h"//
#include "search.hpp"
#include "filling.hpp"
using namespace llvm;
void fill(BasicBlock *BB1,BasicBlock *BB2,Type* T,vector<Instruction*> &inst1,vector<Instruction*> &inst2)
  {
  for (BasicBlock::iterator instruction = BB1->begin(), ee = BB1->end(); instruction != ee; ++instruction)
    {
    if(AllocaInst *alloca=dyn_cast<AllocaInst>(instruction))
      {
      int a=alloca->getAllocatedType ()->getTypeID(); 
      if(alloca->getAllocatedType ()->isFloatingPointTy () )
        {
        if(a > T->getTypeID()) //if the type is smaller than the type wants
          {
          AllocaInst *new_alloca = new AllocaInst(T,0, instruction->getName());
          inst2.push_back(new_alloca);
          BB2->getInstList().push_back(new_alloca);
          }
        else if(a < T->getTypeID()) //if the type is bigger than the type want
          {
          AllocaInst *new_alloca = new AllocaInst(T,0, instruction->getName());
          inst2.push_back(new_alloca);
          BB2->getInstList().push_back(new_alloca);
          }  
        else //if the type is  the same
          {
          Instruction *I=instruction->clone();
          BB2->getInstList().push_back(I);
          inst2.push_back(I);
          }  
        }
      else //not floating type
        {
        Instruction *I=instruction->clone();
        BB2->getInstList().push_back(I);
        inst2.push_back(I);
        }
       inst1.push_back(instruction);           
      }
      //read instruction
    else if(LoadInst *la= dyn_cast<LoadInst>(instruction))
      {
      int n=Searchinst(inst1,dyn_cast<Instruction>(instruction->getOperand(0)));
      LoadInst *new_load=new LoadInst(inst2[n]," ",false);    
      inst2.push_back(new_load);
      inst1.push_back(la);
      BB2->getInstList().push_back(new_load);
      }
     //other instruction   
    else
      {
      Instruction *I=instruction->clone();
      BB2->getInstList().push_back(I);
      inst2.push_back(I);
      inst1.push_back(instruction);
      }  
    }
  }
void branch(vector<BasicBlock*> &T1,vector<BasicBlock*> &T2)
  {
  for(int i=0;i<T1.size();i++)
      {
      if (BranchInst *CI = dyn_cast<BranchInst>(T1[i]->getTerminator ()))
        {
        if(CI->isConditional())
          {
          Instruction *I;
          for(BasicBlock::iterator instruction = T1[i]->begin(), ee = T1[i]->end(); instruction != ee; ++instruction){
          if(ICmpInst *o=dyn_cast<ICmpInst>(instruction))
            {
            I=o;
            }
          }
          int index= Search(T2,CI->getSuccessor(0));
          int  index1= Search(T2,CI->getSuccessor(1)); 
          BranchInst::Create (T1[index],T1[index1] ,I,T1[i]);
          CI->eraseFromParent();
          }
        else
          {
          int index= Search(T2,CI->getSuccessor(0));
          CI->eraseFromParent();
          BranchInst::Create(T1[index],T1[i]);
          }
        }
      }
  }  
