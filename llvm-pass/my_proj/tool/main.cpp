#include "AddConst.h"
#include "llvm/IRReader/IRReader.h"
#include "llvm/Passes/PassBuilder.h"
#include "llvm/Support/CommandLine.h"


using namespace llvm;

//a category for the options specified for this tool
static cl::OptionCategory AddConstCategory("addconst pass options");

//Required argument that specifies the module that will be transformed
static cl::opt<std::string> InputModule(cl::Positional,
					cl::desc("<Input module>"),
					cl::value_desc("bitcode filename"),
					cl::init(""), cl::Required,
					cl::cat(AddConstCategory));
// Optional option
static cl::opt<std::string> OutputModule("o", cl::Positional,
					cl::desc("<Output module>"),
					cl::value_desc("bitcode filename"),
					cl::init("out.ll"),
					cl::cat(AddConstCategory));

int main(int Argc, char **Argv)
{
	cl::HideUnrelatedOptions(AddConstCategory);
	cl::ParseCommandLineOptions(Argc, Argv, "evaluate constant add instructions at compile time. \n");
	llvm_shutdown_obj SDO;

	SMDiagnostic Err;
	LLVMContext Ctx;
	std::unique_ptr<Module> M = parseIRFile(InputModule.getValue(), Err, Ctx);

	if(!M)
	{
		errs() <<"Error reading bitcode file: "<< InputModule <<"\n";
		Err.print(Argv[0], errs());
		return -1;
	}
	addconst::AddConstPass Pass;
	FunctionPassManager FPM;
	FPM.addPass(Pass);

	FunctionAnalysisManager FAM;
	FAM.registerPass([&] {return addconst::AddConstAnalysis(); });

	PassBuilder PB;
	PB.registerFunctionAnalyses(FAM);
	
	for(Function &F : *M) FPM.run(F, FAM);
	std::error_code EC;
	raw_fd_ostream OS(OutputModule.getValue(), EC);

	if(EC)
	{
		errs() <<"Couldn't open " <<OutputModule.getValue() <<":" <<EC.message() <<"\n";
		return -1;
	}
	M->print(OS, nullptr);
	OS.close();

	
	return 0;
}
