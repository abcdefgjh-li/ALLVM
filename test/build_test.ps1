# VMP 递归测试编译脚本
$ndk_clang = "D:\cpp\obfuscator-ollvm-21.x\android-ndk-r30-beta1-windows\toolchains\llvm\prebuilt\windows-x86_64\bin\clang.exe"
$adb = "C:\Users\Administrator\AppData\Local\Android\Sdk\platform-tools\adb.exe"

Write-Host "=== 编译 VMP 递归测试 ===" -ForegroundColor Cyan

# 编译
& $ndk_clang -target aarch64-linux-android21 -fPIC -O2 -mllvm -irobf -mllvm -irobf-vmp -frtti -fno-exceptions -fvisibility=hidden -o test_recursion jni/test_recursion.c

if ($LASTEXITCODE -ne 0) {
    Write-Host "编译失败！" -ForegroundColor Red
    exit 1
}

Write-Host "编译成功！" -ForegroundColor Green
Write-Host "`n=== 推送到模拟器 ===" -ForegroundColor Cyan

& $adb push test_recursion /data/local/tmp/
& $adb shell chmod 755 /data/local/tmp/test_recursion

Write-Host "`n=== 在模拟器上运行 ===" -ForegroundColor Cyan
& $adb shell /data/local/tmp/test_recursion
