QT.graphs.VERSION = 6.10.2
QT.graphs.name = QtGraphs
QT.graphs.module = Qt6Graphs
QT.graphs.libs = $$QT_MODULE_LIB_BASE
QT.graphs.ldflags = 
QT.graphs.includes = $$QT_MODULE_INCLUDE_BASE $$QT_MODULE_INCLUDE_BASE/QtGraphs
QT.graphs.frameworks = 
QT.graphs.bins = $$QT_MODULE_BIN_BASE
QT.graphs.depends =  core gui quick quick3d
QT.graphs.run_depends = quickshapes_private quick3druntimerender qml quick3druntimerender_private
QT.graphs.uses = 
QT.graphs.module_config = v2 staticlib
QT.graphs.DEFINES = QT_GRAPHS_LIB
QT.graphs.enabled_features = graphs-2d graphs-2d-area graphs-2d-bar graphs-2d-donut-pie graphs-2d-line graphs-2d-scatter graphs-2d-spline graphs-3d graphs-3d-bars3d graphs-3d-scatter3d graphs-3d-surface3d
QT.graphs.disabled_features = 
QT_CONFIG += graphs-2d graphs-2d-area graphs-2d-bar graphs-2d-donut-pie graphs-2d-line graphs-2d-scatter graphs-2d-spline graphs-3d graphs-3d-bars3d graphs-3d-scatter3d graphs-3d-surface3d
QT_MODULES += graphs

