#ifndef REGIONS_EX_H
#define REGIONS_EX_H

#include "llvm/Pass.h"

namespace {
	class RegionsEx : public llvm::FunctionPass {
		public:
		static char ID; 
		RegionsEx() : llvm::FunctionPass(ID) {}
		bool runOnFunction(llvm::Function &F); 
		void getAnalysisUsage(llvm::AnalysisUsage &analyses) const;
	};
}
char RegionsEx::ID = 0;
static llvm::RegisterPass<RegionsEx> X("regionsEx", "Print region data");
#endif

