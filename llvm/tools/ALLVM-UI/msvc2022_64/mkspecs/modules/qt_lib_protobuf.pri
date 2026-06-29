QT.protobuf.VERSION = 6.10.2
QT.protobuf.name = QtProtobuf
QT.protobuf.module = Qt6Protobuf
QT.protobuf.libs = $$QT_MODULE_LIB_BASE
QT.protobuf.ldflags = 
QT.protobuf.includes = $$QT_MODULE_INCLUDE_BASE $$QT_MODULE_INCLUDE_BASE/QtProtobuf
QT.protobuf.frameworks = 
QT.protobuf.bins = $$QT_MODULE_BIN_BASE
QT.protobuf.depends =  core
QT.protobuf.uses = 
QT.protobuf.module_config = v2 staticlib
QT.protobuf.DEFINES = QT_PROTOBUF_LIB
QT.protobuf.enabled_features = protobufquick
QT.protobuf.disabled_features = protobuf-qtcoretypes protobuf-qtguitypes protobuf-wellknowntypes
QT_CONFIG += protobufquick
QT_MODULES += protobuf

