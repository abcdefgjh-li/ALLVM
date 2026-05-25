add_llvm_library(LLVMObfuscation
  LicenseManager.cpp
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
  MemDetect.cpp
  PtraceDetect.cpp
  InlineHookDetect.cpp
  PltHookDetect.cpp
  HideMaps.cpp
  FakeMaps.cpp
  MemProtect.cpp
  RootDetect.cpp
  NoRootDetect.cpp
  SyscallProtect.cpp

  ADDITIONAL_HEADER_DIRS
  ${LLVM_MAIN_INCLUDE_DIR}/llvm/Transforms/Obfuscation

  DEPENDS
  LLVMLinker
  )

add_dependencies(LLVMObfuscation intrinsics_gen LLVMLinker)
