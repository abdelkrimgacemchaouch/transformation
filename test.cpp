#include "llvm/Pass.h"//
#include "llvm/IR/Function.h"//
#include "llvm/IR/Module.h"//
#include "llvm/Support/raw_ostream.h"//
#include "llvm/IR/Type.h"//
#include "llvm/IR/CallSite.h"//
#include "llvm/IR/Instructions.h"//
#include "llvm/IR/InstIterator.h"//
#include "llvm/IR/Constants.h"//
#include "search.hpp"
#include "conv.hpp"
#include "filling.hpp"
#include "temp2.hpp"
#include <iostream>
#include <fstream>
using namespace llvm;
using namespace std;
namespace 
  {
  struct test : public ModulePass 
    {
    static char ID;
    test() : ModulePass(ID) {}
    virtual  bool runOnModule(Module &M) 
      {
      vector<BasicBlock*> BBclonef,BBcloned,BBclonel,BBorg;
      std::vector<Instruction*> inst1,instf,instd,instl;
      LLVMContext &context = M.getContext();
      Type* type = Type::getFloatTy(context);
      Type* doublety= Type::getDoubleTy(context);
      Type * longty=Type::getFP128Ty(context);
      for(Module::iterator f = M.begin(), fe = M.end(); f != fe; f++) 
        {
        for (Function::iterator blocdebase = f->begin(), e = f->end(); blocdebase != e; ++blocdebase)
          {
          BBorg.push_back(blocdebase); 
          }
        }
      new_way(BBorg,BBclonef,inst1,instf,type);// way for float
      new_way(BBorg,BBcloned,inst1,instd,doublety);//way for double
      new_way(BBorg,BBclonel,inst1,instl,longty);// way for long
 //******************************************************************************************   

      for(Module::iterator f = M.begin(), fe = M.end(); f != fe; f++) 
        {
        tempo(M,*f,BBclonef,BBcloned,BBclonel);
/*for (Function::iterator blocdebase = f->begin(), e = f->end(); blocdebase != e; ++blocdebase){

errs()<<*blocdebase<<"\n";
}*/
        }
      return true;
      }
    void new_way(vector<BasicBlock*> &BBorg,vector<BasicBlock*> &BB,vector<Instruction*> &inst1,vector<Instruction*> &inst2,Type *T)
      {
      for(int i=0; i<BBorg.size();i++)
        {
        LLVMContext &context = BBorg[i]->getContext();
        BasicBlock *B = BasicBlock::Create(context,"new", BBorg[i]->getParent(),BBorg[i]);
        fill(BBorg[i],B,T,inst1,inst2);
        BB.push_back(B);
        }
      branch(BB,BBorg);
      convert(BB,BBorg,T,inst1,inst2);
      }
    };
  }
char test::ID = 0;
static RegisterPass<test> X("ttt", "Hello World Pass", false, false);

