#include "AddConst.h"
#include "llvm/Passes/PassBuilder.h"
#include "llvm/Passes/PassPlugin.h"
#include<iostream>
using namespace std;
using namespace llvm;

LoopWrapper &prepare(LoopInfo &LI, LLVMContext &Ctx)
{
	// initialize the loop wrapper
	auto LW = new LoopWrapper(LI);
	auto BoolTy = IntegerType::getInt1Ty(Ctx);
	auto False = ConstantInt::getFalse(BoolTy);
	//Traverse the loops using loopinfo
	for( auto L : LI.getLoopsInPreorder())
	{
		auto LH = L->getHeader(); //loopheader
		auto LL = L->getLatch(); // loop latch
		assert(L->isRotatedForm()) // loop rotate do while type with condition at the end
		LW->LLBlocks.insert(LL);
		SmallVector<BasicBlock *, 4>ExitingBlocks;
		L->getExitingBlocks(ExitingBlocks); // Exiting Blocks
		auto Before = LH->getFirstNonPHI(); //position in the loop header where we are going to insert these three functions
		// visit every exiting block except for the loop latch since once the loop conditional statement becomes true, the loop ends.
		for(auto LE : ExitingBlocks)
		{
			if (LE == LL)
				continue;
			auto Terminator = LE->getTerminator(); // We get the exiting block terminator which is a conditional statement. 
			auto LET = cast<BranchInst>(Terminator);
			auto C = LET->getCondition();
			auto Phi = PHINode::Create(BoolTy, pred_size(LH), "p", Before); //then we create the phi function. then we traverse the predecessor of the loop header to fill the phi function with correct incoming values.
			for(auto P : predecessors(LH))
			{
				Phi->addIncoming(P == LL ? C : False, P);
				LW->PredMap[c] = Phi; // we also save the fucntion in our loop wrapper structure.
			}
		}
		SmallVector<BasicBlock *, 4>ExitBlocks;
		L->getExitingBlocks(ExitBlocks); // then we get the exit blocks which are the blocks outside the loop.
		for(auto LE : ExitBlocks) LW->ExitBlocks.insert(LE);
	}
	return *LW;
}
