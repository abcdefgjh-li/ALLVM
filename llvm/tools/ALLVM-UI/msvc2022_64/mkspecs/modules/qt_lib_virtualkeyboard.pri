QT.virtualkeyboard.VERSION = 6.10.2
QT.virtualkeyboard.name = QtVirtualKeyboard
QT.virtualkeyboard.module = Qt6VirtualKeyboard
QT.virtualkeyboard.libs = $$QT_MODULE_LIB_BASE
QT.virtualkeyboard.ldflags = 
QT.virtualkeyboard.includes = $$QT_MODULE_INCLUDE_BASE $$QT_MODULE_INCLUDE_BASE/QtVirtualKeyboard
QT.virtualkeyboard.frameworks = 
QT.virtualkeyboard.bins = $$QT_MODULE_BIN_BASE
QT.virtualkeyboard.depends =  core gui qml quick
QT.virtualkeyboard.uses = 
QT.virtualkeyboard.module_config = v2 staticlib
QT.virtualkeyboard.DEFINES = QT_VIRTUALKEYBOARD_LIB
QT.virtualkeyboard.enabled_features = vkb-desktop vkb-layouts vkb-default-style
QT.virtualkeyboard.disabled_features = vkb-record-trace-input vkb-arrow-keynavigation vkb-no-builtin-style vkb-retro-style vkb-bundle-cerence vkb-bundle-cerence-hwr vkb-bundle-cerence-xt9 vkb-cerence-xt9-debug vkb-cerence-xt9-9key-layouts vkb-cerence-static
QT_CONFIG += vkb-desktop vkb-layouts vkb-default-style
QT_MODULES += virtualkeyboard

