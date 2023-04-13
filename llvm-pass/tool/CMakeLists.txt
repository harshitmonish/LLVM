add_executable(addconst	main.cpp ../lib/AddConstAnalysis.cpp ../lib/AddConstPass.cpp)
target_link_libraries(addconst LLVMCore LLVMIRReader LLVMPasses LLVMSupport)
target_include_directories(addconst PRIVATE "../include")
