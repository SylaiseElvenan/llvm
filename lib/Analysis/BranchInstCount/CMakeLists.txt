add_llvm_library( LLVMBranchInstCount MODULE BUILDTREE_ONLY
  BranchInstCount.cpp

  DEPENDS
  intrinsics_gen
  PLUGIN_TOOL
  opt
  )
  
