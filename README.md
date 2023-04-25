# LLVM Passes
In this project I have implemented Analysis and Transformation passes using llvm opt tool. In Analysis loop I have implemented a pass to collect all the add instructions of constant operands and returned them. In transformation analysis I have implemented constant propagation and compile time evaluation optimizations.

In order to write an analysis pass following steps have been implemented:
* First we need to define the pass, for this you have to define a class which should inherit from the llvm::Analysis pass class and override the run() method which is the entrypoint of the analysis
* Next we need to register our pass with LLVM's pass manager. You can do this by defining a static llvm::PassRegistration<InstructionCountPass> object which will register the pass when it is loaded.
* Finally we need to run the pass, to do this we use the LLVM's FunctionAnalysisManager class which manages the analysis passes for a function.

  
In order to write an analysis pass following steps have been implemented:
* Define the pass, for this you have to define a class which should inherit from the llvm::pass class and override the runOnFunction() method which is the entry point of the transformation
* Next we need to register the pass, same as above we need to register the pass with LLVM's pass manager and can do this by defining a static llvm::RegisterPass<classname> object which will register the pass when it is loaded.
* Finally we need to run the pass, to do this we use LLVM's PassManagerBuilder class which provides a convinient way to configure the pass manager for your program.
  
  
## How to Run
Following are the steps involved:

1. Build the project
 1.1 goto project dir "cd <path-to-proj-dir>"
 1.2 set the llvm install dir path "LLVM_INSTALL_DIR=<path to llvm install dir>"
 1.3 compile the project using cmake "cmake -DLLVM_INSTALL_DIR=$LLVM_INSTALL_DIR -G “Ninja” -B build/ . 
 1.4 goto build dir "cd build"
 1.5 compile the project " cmake --build . "
2. Running the project

 2.1 set the opt tool path "LLVM_OPT=$LLVM_INSTALL_DIR/bin/opt"
 
 2.2 execute the opt tool "$LLVM_OPT -load-pass-plugin build/lib/libAddConst.so -passes="print<add-const>" -disable-output examples/"


Steps to run transformation loop pass
1. Build the project as above steps (skip if already build in previous analysis)
2. Running the project.

  2.1 set the opt tool path "LLVM_OPT=$LLVM_INSTALL_DIR/bin/opt"
  
  2.2 executet the opt tool "$LLVM_OPT -load-pass-plugin build/lib/libAddConst.so -passes=”add-const” -S examples/foo.ll -o examples/foo2.ll"
  
  
## OUTPUT
### Analysis Pass
![Analysis pass output](https://github.com/harshitmonish/LLVM/blob/main/llvm-analysis-pass.png)  

### Transformation Pass
![Transformation pass output](https://github.com/harshitmonish/LLVM/blob/main/llvm-transformation-pass.png)
  
