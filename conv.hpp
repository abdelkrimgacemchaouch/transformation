#ifndef conv
#define conv
using namespace std;
using namespace llvm;
namespace llvm {
	class Value;
	class Instruction;
	class BasicBlock;
}


extern void convert(vector<BasicBlock*> &T1,vector<BasicBlock*> &T2,Type* old,vector<Instruction*> &inst1,vector<Instruction*> &inst2);
#endif
