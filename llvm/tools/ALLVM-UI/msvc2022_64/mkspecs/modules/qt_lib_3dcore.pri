QT.3dcore.VERSION = 6.10.2
QT.3dcore.name = Qt3DCore
QT.3dcore.module = Qt63DCore
QT.3dcore.libs = $$QT_MODULE_LIB_BASE
QT.3dcore.ldflags = 
QT.3dcore.includes = $$QT_MODULE_INCLUDE_BASE $$QT_MODULE_INCLUDE_BASE/Qt3DCore
QT.3dcore.frameworks = 
QT.3dcore.bins = $$QT_MODULE_BIN_BASE
QT.3dcore.depends =  concurrent core gui network
QT.3dcore.run_depends = concurrent
QT.3dcore.uses = 
QT.3dcore.module_config = v2 staticlib
QT.3dcore.DEFINES = QT_3DCORE_LIB
QT.3dcore.enabled_features = qt3d-assimp qt3d-render qt3d-input qt3d-logic qt3d-extras qt3d-animation
QT.3dcore.disabled_features = 
QT_CONFIG += qt3d-assimp qt3d-render qt3d-input qt3d-logic qt3d-extras qt3d-animation
QT_MODULES += 3dcore

