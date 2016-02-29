#ifndef temp2
#define temp2
using namespace std;
using namespace llvm;
namespace llvm {
	class Value;
	class Type;
	class Instruction;
	class BasicBlock;
}
extern void tempo(Module &M,Function &f,vector <BasicBlock*> &BBclonef,vector <BasicBlock*> &BBcloned,vector <BasicBlock*> &BBclonel);
#endif
