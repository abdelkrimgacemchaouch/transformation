we have implemented a tool that makes an automatic choice for the for precision of floating point.
this tool is implemented by llvm and c/c++, in this tool we take source code (c for the moment) and translate this source code to bite code.
the structure of bite code is module, function, basic block, instruction. 
our tool interested to function, where in each function we created three other way (float,double, long) and for each way we have just one type for floating point (float, double or long ) and we have done some conditions(overflow/underflow) that determine if we continue in the way or go to other way 


