#include "regionAnalyses.h"
#include "llvm/Passes/PassBuilder.h"
#include "llvm/Analysis/RegionInfo.h"
#include<iostream>
using namespace std;
using namespace llvm;

void RegionsEx::getAnalysisUsage(llvm::AnalysisUsage &analyses) const 
{
	analyses.addRequired<llvm::RegionInfoPass>(); //this tells llvm that we will be using region analysis, this analysis is made available to us by the class region info pass
	analyses.setPreservesAll(); // This tells that we are preserving all the information collected by llvm thus far
}
// For this analysis all we want to is to print out information about each region that constitutes the function.
bool RegionEx::runOnFunction(llvm::Function &F)
{
	llvm::RegionInfo &region_info = getAnalysis<llvm::RegionInfoPass>().getRegionInfo(); // firstly we get a reference to the region info class
	llvm::Region &region = *region_info.getTopLevelRegion(); // here we are getting a pointer to the tree of regions, we do this by capturing a reference to the root of the tree of regions.
	visit(region); // then we visit them recursively like we traverse a tree
	return false;
}
void visit(const llvm::Region &region)
{
	print_log(region); 
	for(auto &it : region) // we traverse the children regions
	{
		const llvm::region* sub_region = it.get();
		visit(*sub_region);
	}
}
void print_log(const llvm::Region &region)
{
	llvm.outs() << region.getNameStr() << '\n';
	llvm.outs() << "Top level? " <<region.isTopLevelRegion() << '\n';
	llvm.outs() << "Depth: "<<region.getDepth() << '\n';
	llvm.outs() << "Entry: "<<region.getEntry()->getName() << '\n';
	for(const llvm::BasicBlock *bb : region.blocks())
	{
		llvm::outs() << " - " << bb->getName() <<'\n'; 
	}
	const llvm::BasicBlock* exit_block = region.getExit();
	if(exit_block)
	{
		llvm::outs() <<"Exit: " << exit_block->getName() <<'\n';
	}
	else
	{
		llvm::outs() << "Exit: <Function Return>\n";
	}
}

