#include "AddConst.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/PassManager.h"
#include<iostream>
using namespace std;

using namespace llvm;

namespace addconst
{
	void ReplaceAddInstWithConstant(BinaryOperator *BO)
	{
		assert(BO->getOpcode() == Instruction::BinaryOps::Add);
		auto Fst = cast<ConstantInt>(BO->getOperand(0));
		auto Snd = cast<ConstantInt>(BO->getOperand(1));
		auto Sum = ConstantExpr::getAdd(Fst, Snd);
		BO->replaceAllUsesWith(Sum);
		BO->eraseFromParent();
	}
	PreservedAnalyses AddConstPass::run(Function &F, FunctionAnalysisManager &FAM)
	{
		auto &AddInsts = FAM.getResult<AddConstAnalysis>(F);
		for(auto BO : AddInsts) ReplaceAddInstWithConstant(BO);
		auto PA = PreservedAnalyses::all();
		PA.abandon<AddConstAnalysis>();
		return PA;
	}
}
