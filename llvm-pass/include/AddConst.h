#ifndef ADD_CONST_H
#define ADD_CONST_H

#include "llvm/IR/InstrTypes.h"
#include "llvm/IR/PassManager.h"
#include<iostream>
using namespace std;

namespace addconst
{	
	//an analysis that collects all the add instructions with constant operands across all functions
	struct AddConstAnalysis : public llvm::AnalysisInfoMixin<AddConstAnalysis>
	{
		public:
		//result of the analysis, a list of instructions of type BinaryOperator, opcode, add and constant operands.
		using Result = llvm::SmallVector<llvm::BinaryOperator *, 0>;
		
		//Traverse the function \p F, collecting all add instructions and return a list of all collected add instructions
		Result run(llvm::Function &F, llvm::FunctionAnalysisManager &FAM);
		
		// A special type used by analysis passes to provide an address that identifies that particular analysis pass type.
		static llvm::AnalysisKey Key;
	};
	
	// A wrapper around AddAnalysis to be used with "opt -passes=" to print the collected instructions
	struct AddConstPrinterPass : public llvm::PassInfoMixin<AddConstPrinterPass> 
	{
		public:
		// Takes a ostream OS and contructs an instance if the AddPrinterClass
		explicit AddConstPrinterPass(llvm::raw_ostream &OS) : OS(OS) {}
		
		// Run the AddAnalysis on function \p F and prints its result. Returns llvm::PreservedAnalyses::all() since nothing changed.
		llvm::PreservedAnalyses run(llvm::Function &F, llvm::FunctionAnalysisManager &FAM);
	
		private:
		llvm::raw_ostream &OS;
	};
	
	// A transformation pass that evaluates all constant-only add instructions and replace their uses by the computed constant.
	struct AddConstPass: public llvm::PassInfoMixin<AddConstPass>
	{
		public:
		/* Gets the result of AddConstAnalysis for the function \p F and uses it to replace the uses of the collected add instructions by their
		   final value returns set of preserved analyses (all of them, except for AddConstAnalysis). */
		llvm::PreservedAnalyses run(llvm::Function &F, llvm::FunctionAnalysisManager &FAM);
	};
}
#endif
