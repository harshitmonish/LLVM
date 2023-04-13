#include "AddConst.h"
#include "llvm/IR/Constants.h"
#include "llvm/Support/Casting.h"
#include<iostream>
using namespace std;

using namespace llvm;

namespace addconst {
	// Takes an instruction and checks whether its operands are constant or not.
	// returns true of all operands are constant; otherwise returns false
	bool isConstantIntOnly(Instruction &I)
	{
		for(Use &Op : I.operands())
		{
			//check if operand is a constant integer:
			if(!isa<ConstantInt>(Op))
				return false;
		}
		return true;
	}
	
	//Initialize the analysis key
	AnalysisKey AddConstAnalysis::Key;
	
	AddConstAnalysis::Result AddConstAnalysis::run(Function &F, FunctionAnalysisManager &FAM)
	{
		SmallVector<BinaryOperator *, 0> AddInsts;
		for(BasicBlock &BB : F)
		{
			for(Instruction &I : BB)
			{
				// First, check if I is an instance of BinaryOperator:
				if(!I.isBinaryOp())
					continue;
				// If it is, check if its opcode is Add
				if(!(I.getOpcode() == Instruction::BinaryOps::Add))
					continue;
				//check if all operands are constant integer
				if(!isConstantIntOnly(I))
					continue;
				// IF it is ADD, append it to the result vector
				AddInsts.push_back(&cast<BinaryOperator>(I));
				
			}
		}
		return AddInsts;
	}
	PreservedAnalyses AddConstPrinterPass::run(Function &F, FunctionAnalysisManager &FAM)
	{
		auto &AddInsts = FAM.getResult<AddConstAnalysis>(F);
		OS << "====================================\n"; 
		OS << " || " << F.getName() << "|| \n ";
		OS << "====================================\n"; 
		for (auto &Add : AddInsts) OS << *Add << "\n";
		OS << "====================================\n"; 
		return PreservedAnalyses::all();
	}
}
