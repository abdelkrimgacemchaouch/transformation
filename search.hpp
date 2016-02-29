#ifndef search
#define search
using namespace std;
using namespace llvm;
namespace llvm {
	class Value;
	class Instruction;
	class BasicBlock;
}
extern int Search(vector<BasicBlock*> F  ,BasicBlock *D);
extern int Searchinst(vector<Instruction*> I,Instruction *inst );
extern int Searcharg(vector<Argument*> I,Argument *inst );
#endif
