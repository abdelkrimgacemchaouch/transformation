#ifndef cont_tra
#define cont_tra
using namespace std;
using namespace llvm;
namespace llvm {
	class Value;
	class Instruction;
	class BasicBlock;
}


extern void transform(Module &M,Function &f,vector <BasicBlock*> &BB1,vector <BasicBlock*> &BB2,vector<Instruction*>I1,vector<Instruction*>I2) ;
#endif
