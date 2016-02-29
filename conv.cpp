#include "llvm/Pass.h"//
#include "llvm/IR/Function.h"//
#include "llvm/IR/Module.h"//
#include "llvm/Support/raw_ostream.h"//
#include "llvm/IR/Type.h"//
#include "llvm/IR/CallSite.h"//
#include "llvm/IR/Instructions.h"//
#include "llvm/IR/InstIterator.h"//
#include "llvm/IR/Constants.h"////
#include "search.hpp"
#include "conv.hpp"
using namespace llvm;
void convert(vector<BasicBlock*> &T1,vector<BasicBlock*> &T2,Type* T,vector<Instruction*> &inst1,vector<Instruction*> &inst2)
  {
  for(int i=0;i<T1.size();i++)
    {
    for(BasicBlock::iterator instruction = T1[i]->begin(), ee = T1[i]->end(); instruction != ee; ++instruction)
      {
      //**********************************delete all conversation from the new block**************************************
       for (int i=0;i<instruction->getNumOperands();i++)
            {
            int n=Searchinst(inst1,dyn_cast<Instruction>(instruction->getOperand(i)));
            if(n<inst1.size())
              {
              if(dyn_cast<FPExtInst>(inst1[n]))
                {
                instruction->setOperand(i,inst1[n]->getOperand(0));
                inst2[n]->eraseFromParent();
                }
              if(dyn_cast< FPTruncInst>(inst1[n]))
                {
                instruction->setOperand(i,inst1[n]->getOperand(0));
                inst2[n]->eraseFromParent();
                }  
              }
            }
      //*************************************write instruction **********************************************        
      if(StoreInst *st=dyn_cast<StoreInst>(instruction))
        {
        int n1=Searchinst(inst1,dyn_cast<Instruction>(st->getOperand(1)));
        //************************************Store argument********************************************
        if(Argument *a= dyn_cast<Argument>(st->getOperand(0)))
          {
          int ID= a->getType()->getTypeID();
          if(a->getType()->isFloatingPointTy () )
            {
            if(ID > T->getTypeID())
              {
              FPTruncInst *c= new FPTruncInst  (st->getOperand(0), T, "",st);
              st->setOperand(0,c);
              }
            else if (ID< T->getTypeID())
              {
              FPExtInst *ext = new FPExtInst(st->getOperand(0), T, "",st);
              st->setOperand(0,ext);
              }
            else
              {
              st->setOperand(0,st->getOperand(0));
              }  
            }    
          else
            {
            st->setOperand(0,st->getOperand(0));
            }  
          }
         //***********************************Store constant***************************** 
        else if (Constant *cons=dyn_cast<Constant>(st->getOperand(0)))
          {
          if(ConstantFP *cc=dyn_cast<ConstantFP>(cons))
            {
            double a= APFloat(cc->getValueAPF()).convertToDouble();
            Value* x1 = ConstantFP::get(T,a);
            st->setOperand(0,x1);
            }
          else
            {
            st->setOperand(0,cons);//constant not float 
            }  
          }
         //*****************************************store inst*********************   
        else
          {
          int n=Searchinst(inst1,dyn_cast<Instruction>(st->getOperand(0)));
          AllocaInst *al1,*al2;
          al1=dyn_cast<AllocaInst>(inst1[n1]);
          al2=dyn_cast<AllocaInst>(inst2[n1]);
          int a =inst2[n]->getType()->getTypeID();
          if (inst2[n]->getType()->isFloatingPointTy ())   
            {
            if (a>T->getTypeID())
              {
              FPTruncInst *fpt = new FPTruncInst(inst2[n], T, "", st);
              st->setOperand(0,fpt);
              }
            else if(a< T->getTypeID())
              {
              FPExtInst *ext = new FPExtInst(inst2[n], T, "",st);
              st->setOperand(0,ext);
              }
            else  
              {
              st->setOperand(0,inst2[n]);
              }    
            }
          else
            { 
            st->setOperand(0,inst2[n]);
            }  
          }
         st->setOperand(1,inst2[n1]); 
        }
  //**********************************input and output instruction ***********************************************
  
        if(instruction->mayWriteToMemory () && instruction->mayReadFromMemory ())
          {
         for (int i=0;i<instruction->getNumOperands();i++)
            {
            int n=Searchinst(inst1,dyn_cast<Instruction>(instruction->getOperand(i)));
            
            if(n<inst1.size())
              {
              int a =inst2[n]->getType()->getTypeID();
              if(inst2[n]->getType()->isFloatingPointTy () )
                {
                
                if (a<inst1[n]->getType()->getTypeID() ) 
                  {
                  FPExtInst *ext = new FPExtInst(inst2[n], inst1[n]->getType(), "");
                  ext->insertBefore(instruction);
                  instruction->setOperand(i,ext);
                  }
                else if(a>inst1[n]->getType()->getTypeID())
                  {
                  FPTruncInst *fpt = new FPTruncInst(inst2[n], inst1[n]->getType(), "", instruction);
                  instruction->setOperand(i,fpt);              
                  }
                else
                  {
                  instruction->setOperand(i,inst2[n]);
                  }   
                } 
              else
                {
                instruction->setOperand(i,inst2[n]);
                }
              }
            }
          }
  
  //**************************************************************************************************************************
  
        if(!(instruction->mayReadOrWriteMemory ()))
          {
          for (int i=0;i<instruction->getNumOperands();i++)
            {
            int n=Searchinst(inst1,dyn_cast<Instruction>(instruction->getOperand(i)));
            
            if(n<inst1.size())
              {
              int a =inst2[n]->getType()->getTypeID();
              if(inst2[n]->getType()->isFloatingPointTy())
                {
                if (a<inst1[n]->getType()->getTypeID() ) 
                  {
                  FPExtInst *ext = new FPExtInst(inst2[n], inst1[n]->getType(), "");
                  ext->insertBefore(instruction);
                  instruction->setOperand(i,ext);
                  }
                else if(a>inst1[n]->getType()->getTypeID())
                  {
                  FPTruncInst *fpt = new FPTruncInst(inst2[n], inst1[n]->getType(), "", instruction);
                  instruction->setOperand(i,fpt);              
                  }
                else
                  {
                  instruction->setOperand(i,inst2[n]);
                  }   
                } 
              else
                {
                instruction->setOperand(i,inst2[n]);
                }
              }
            }
          }
      }
    }
  }
