#include <QApplication>
#include <QIcon>
#include "MainWindow.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    app.setApplicationName("ALLVM 混淆编译配置工具");
    app.setApplicationVersion("1.6.0");
    app.setWindowIcon(QIcon(":/UI.png"));
    
    app.setStyleSheet(
        "QMainWindow { background: #1a1a2e; }"
        "QGroupBox { color: #e0e0e0; font-weight: bold; border: 1px solid #3a3a5c; "
        "  border-radius: 6px; margin-top: 12px; padding-top: 16px; }"
        "QGroupBox::title { subcontrol-origin: margin; left: 12px; padding: 0 6px; }"
        "QCheckBox { color: #c0c0d0; spacing: 8px; }"
        "QCheckBox::indicator { width: 16px; height: 16px; }"
        "QCheckBox::indicator:unchecked { border: 2px solid #4a4a6a; border-radius: 3px; background: #252540; }"
        "QCheckBox::indicator:checked { border: 2px solid #00d4aa; border-radius: 3px; background: #00d4aa; }"
        "QLineEdit { background: #252540; color: #e0e0e0; border: 1px solid #4a4a6a; "
        "  border-radius: 4px; padding: 6px 10px; font-size: 13px; }"
        "QLineEdit:focus { border-color: #00d4aa; }"
        "QTextEdit { background: #12122a; color: #00ff88; border: 1px solid #3a3a5c; "
        "  border-radius: 4px; font-family: 'Consolas', monospace; font-size: 12px; }"
        "QPushButton { background: #00d4aa; color: #1a1a2e; border: none; border-radius: 4px; "
        "  padding: 8px 20px; font-weight: bold; font-size: 13px; }"
        "QPushButton:hover { background: #00eebb; }"
        "QPushButton:pressed { background: #00c49a; }"
        "QPushButton#btnSelect { background: #3a3a5c; color: #e0e0e0; }"
        "QPushButton#btnSelect:hover { background: #4a4a6c; }"
        "QComboBox { background: #252540; color: #e0e0e0; border: 1px solid #4a4a6a; "
        "  border-radius: 4px; padding: 6px 10px; font-size: 13px; }"
        "QComboBox:hover { border-color: #00d4aa; }"
        "QComboBox QAbstractItemView { background: #252540; color: #e0e0e0; "
        "  selection-background-color: #00d4aa; selection-color: #1a1a2e; }"
        "QLabel { color: #a0a0b0; }"
        "QProgressBar { border: 1px solid #3a3a5c; border-radius: 4px; background: #252540; "
        "  text-align: center; color: #e0e0e0; height: 20px; }"
        "QProgressBar::chunk { background: #00d4aa; border-radius: 3px; }"
    );
    
    MainWindow window;
    window.resize(1000, 750);
    window.show();
    
    return app.exec();
}
