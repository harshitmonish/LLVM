#include "AddConst.h"
#include "llvm/Passes/PassBuilder.h"
#include "llvm/Passes/PassPlugin.h"
#include<iostream>
using namespace std;

using namespace llvm;
	
// Takes a functionAnalysisManager \p FAM and uses it to register all the analyses created, so any pass can request their result.
void registerAnalyses(FunctionAnalysisManager &FAM)
{
	FAM.registerPass([] { return addconst::AddConstAnalysis(); });

}

/* Takes the \p Name of a transformation pass and check if it is the name of any of the passes implemented. If so, add it to the FunctionPassManager \p FPM.
   returns true if \p Name corresponds to any of the passes implemented; otherwise return false */
bool registerPipeline(StringRef Name, FunctionPassManager &FPM, ArrayRef<PassBuilder::PipelineElement>)
{
	if(Name == "print<add-const>")
	{
		FPM.addPass(addconst::AddConstPrinterPass(errs()));
		return true;
	}
	if(Name == "add-const")
	{
		FPM.addPass(addconst::AddConstPass());
		return true;
	}
	return false;
}

PassPluginLibraryInfo getAddConstPluginInfo() 
{
	//cout << getenv("LLVM_PLUGIN_API_VERSION")<< " ====================================\n"; 
	return {LLVM_PLUGIN_API_VERSION, "AddConst", LLVM_VERSION_STRING, [](PassBuilder &PB) {	PB.registerAnalysisRegistrationCallback(registerAnalyses); PB.registerPipelineParsingCallback(registerPipeline); }};
	/*
		1. register the AddConstAnalysis as an analysis pass so that it can be requested by other passes as following:
		FPM>getResult<AddConstAnalysis>(F), where FPM is the FunctionAnalysisManager and F is the Function that shall be analyzed.
	*/
	/*
		2. register the AddConstPrinterPass as "print<add-const>" so that it can be used when specifying pass pipelines with "-passes=". Also register 
		AddConstPass as "add-const".
	*/
}

// public entry point for a pass plugin:
extern "C" LLVM_ATTRIBUTE_WEAK PassPluginLibraryInfo llvmGetPassPluginInfo() {
	return getAddConstPluginInfo();
}
