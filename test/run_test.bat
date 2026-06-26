@echo off
adb -s emulator-5554 push libs\arm64-v8a\fla3_test /data/local/tmp/fla3_test
adb -s emulator-5554 shell chmod 755 /data/local/tmp/fla3_test
echo test_input | adb -s emulator-5554 shell /data/local/tmp/fla3_test
