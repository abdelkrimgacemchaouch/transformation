#ifndef filling
#define filling
using namespace std;
using namespace llvm;
namespace llvm {
	class Value;
	class Instruction;
	class BasicBlock;
}
extern void fill(BasicBlock *BB1,BasicBlock *BB2,Type* old,vector<Instruction*> &inst1,vector<Instruction*> &inst2);
extern void branch(vector<BasicBlock*> &T1,vector<BasicBlock*> &T2);
#endif
