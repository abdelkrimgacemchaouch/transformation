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
#include "cont_tra.hpp"
#include "search.hpp"
using namespace llvm;
using namespace std;
void transform(Module &M,Function &f,vector <BasicBlock*> &BB1,vector <BasicBlock*> &BB2,vector<Instruction*>I1,vector<Instruction*>I2)
  {
  LLVMContext &context = M.getContext();
  Instruction *br,*I;
  Type* Int1= Type::getInt8PtrTy(context);
  Type *typpe=Type::getInt32Ty(context);
  Type* voidd= Type::getVoidTy(context);
  vector <BasicBlock*> BB3,BB4,contt,tran;
  vector<Instruction*>inst1,inst2;
  BasicBlock *bt;
  Instruction *in;
  unsigned i=0;
  
  Function *under_over = cast<Function>(M.getOrInsertFunction ("llvm.x86.sse.stmxcsr",voidd,Int1,nullptr));//function how test condition(overflow,underflow...)
  //*******************************take the basic block and his copy  of the highest type********************
    for (Function::iterator blocdebase = f.begin(), e = f.end(); blocdebase != e; ++blocdebase)
  {
  unsigned n=Search(BB1,blocdebase);
  if(n<BB1.size())
    {
    BB3.push_back(BB1[n]);
    BB4.push_back(BB2[n]); 
    }
  }
  
  //********************************take the instruction of each block *******************************
  bt=f.begin();   
  for(unsigned i=0;i<BB3.size();i++)
    {
    for(BasicBlock::iterator instruction = BB3[i]->begin(), ee = BB3[i]->end(); instruction != ee; ++instruction)
      {
      if (dyn_cast<LoadInst>(instruction))
        {
        //from read instruction 
        unsigned n=Searchinst(I2,dyn_cast<Instruction>(instruction->getOperand(0)));
        unsigned n1=Searchinst(inst2,dyn_cast<Instruction>(instruction->getOperand(0)));
        if(n<I2.size()&&n1>inst2.size())
          {
          inst2.push_back(I2[n]);//copy instruction
          inst1.push_back(I1[n]);//orginal instruction
          }
        }
      if (dyn_cast<StoreInst>(instruction))
        {
        //from write instruction
        unsigned n=Searchinst(I2,dyn_cast<Instruction>(instruction->getOperand(0)));
        unsigned n1=Searchinst(inst2,dyn_cast<Instruction>(instruction->getOperand(0)));
        unsigned n2=Searchinst(I2,dyn_cast<Instruction>(instruction->getOperand(1)));
        unsigned n3=Searchinst(inst2,dyn_cast<Instruction>(instruction->getOperand(1)));
        if(n<I2.size()&&n1>inst2.size()){inst2.push_back(I2[n]);inst1.push_back(I1[n]);}
        else if(n2<I2.size()&&n3>inst2.size()){inst2.push_back(I2[n2]);inst1.push_back(I1[n2]);}
        }        
      }
    }
 
  while(i<BB3.size())
    {
    BasicBlock *cont = BasicBlock::Create(context, "continue", BB3[i]->getParent());//block that allows to continue in the same way
    BasicBlock *transforms = BasicBlock::Create(context, "transforms", BB3[i]->getParent());//block that do the transformation  
    I= BB3[i]->getTerminator();
    br=I->clone();
    I->eraseFromParent();
    //*********************************test to choose what to do*************************
    AllocaInst *test = new AllocaInst(typpe,0, "test",BB3[i]);
    BitCastInst *newb= new BitCastInst(test,Int1," ",BB3[i]);
    CallInst::Create(under_over, newb, "",BB3[i]);
    LoadInst *Cov=new LoadInst(test,"",false,BB3[i]);
    Value *one = ConstantInt::get(Type::getInt32Ty(context), 63);
    Value *two = ConstantInt::get(Type::getInt32Ty(context), 16);
    Value *three = ConstantInt::get(Type::getInt32Ty(context), 8);
    BinaryOperator *te1 = BinaryOperator::Create(Instruction::And,Cov,one,"",BB3[i]);
    BinaryOperator *te2 = BinaryOperator::Create(Instruction::And,te1,two,"",BB3[i]);
    BinaryOperator *te3 = BinaryOperator::Create(Instruction::And,Cov,three,"",BB3[i]);
    BinaryOperator *te4 = BinaryOperator::Create(Instruction::Or,te3,te2,"",BB3[i]);
    Value *z = ConstantInt::get(Type::getInt32Ty(context), 0);
    Value *CondI = new ICmpInst(*BB3[i], ICmpInst::ICMP_NE, te4,z , "cond");
    //************************************************************************      
    BranchInst::Create (transforms,cont ,CondI,BB3[i]);// branch to the block of continue 
    BranchInst::Create(BB4[i],transforms);// branch to the block copy
    cont->getInstList().push_back(br);// branch to the next block
    contt.push_back(cont);//take all the continents blocks
    tran.push_back(transforms);//take all the transformations blocks
    i++;
    }
  BB3.clear();
  BB4.clear();   
  i=0;
  while(i<contt.size())
    {
    I= contt[i]->getTerminator();
    int size= I1.size();
    for(unsigned a=0;a<inst1.size();a++)
      { 
      LoadInst *new_load = new LoadInst(inst2[a],"Nl",false,I);//load the copy value
      new StoreInst(new_load,inst1[a] , false,I);//store the copy value in original value
      //********************************************************************** 
      in= tran[i]->getTerminator();  
      LoadInst *new_load1 = new LoadInst(inst1[a],"e",false,in);//load the original value of original block
      int id=Searchinst(I1,inst1[a]);
      AllocaInst *t=dyn_cast<AllocaInst>(I1[(id+(size/3))]);
      Type *T=t->getAllocatedType ();
      if(inst1[a]->getType()==I1[(id+(size/3))]->getType())//same type of original value of copy block
        {
        new StoreInst(new_load1,I1[(id+(size/3))] , false,4,in);
        }
      else//different type
        {
        FPExtInst *c= new FPExtInst  (new_load1,T , "",in);
        new StoreInst(c,I1[(id+(size/3))] , false,in);
        }    
      }   
    i++;
    }
  inst1.clear();
  contt.clear();
  }
