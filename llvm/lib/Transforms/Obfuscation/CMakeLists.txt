set(INTERPRETER_BC "${CMAKE_CURRENT_SOURCE_DIR}/../../../aVMPInterpreter/aVMPInterpreter.bc")

add_llvm_library(LLVMObfuscation
  CryptoUtils.cpp
  Utils.cpp
  ObfuscationPassManager.cpp
  ObfuscationOptions.cpp
  IndirectBranch.cpp
  IndirectCall.cpp
  IndirectGlobalVariable.cpp
  Flattening.cpp
  StringEncryption.cpp
  LegacyLowerSwitch.cpp
  ConstantIntEncryption.cpp
  ConstantFPEncryption.cpp
  MicrosoftRTTIEraser.cpp
  aVMP.cpp
  AProtect.cpp
  LdPreloadProtect.cpp
  VmProtectDetect.cpp
  UsbProtect.cpp
  IdaDetect.cpp
  VpnDetect.cpp
  ProxyDetect.cpp
  TimeDetect.cpp
  HostsDetect.cpp
  HideMaps.cpp
  FakeMaps.cpp
  RootDetect.cpp
  NoRootDetect.cpp
  SyscallProtect.cpp
  BanDump.cpp
  DetectUtils.cpp

  ADDITIONAL_HEADER_DIRS
  ${LLVM_MAIN_INCLUDE_DIR}/llvm/Transforms/Obfuscation

  DEPENDS
  LLVMLinker
  )

add_dependencies(LLVMObfuscation intrinsics_gen LLVMLinker)

if(EXISTS "${INTERPRETER_BC}")
  message(STATUS "Embedding aVMPInterpreter.bc into LLVMObfuscation")
  set_source_files_properties(aVMP.cpp PROPERTIES OBJECT_DEPENDS "${INTERPRETER_BC}")
endif()
