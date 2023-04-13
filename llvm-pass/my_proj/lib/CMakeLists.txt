add_library(AddConst MODULE
AddConstAnalysis.cpp
AddConstPass.cpp
AddConstPlugin.cpp)

target_include_directories(AddConst PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/../include")
