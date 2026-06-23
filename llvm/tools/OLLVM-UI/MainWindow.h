#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTextEdit>
#include <QProgressBar>
#include <QProcess>
#include <QTabWidget>
#include <QKeyEvent>
#include <QToolButton>
#include "MainTab.h"
#include "CodeEditor.h"

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    void closeEvent(QCloseEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;

private slots:
    void onInjectFlags();
    void onBuild();
    void onReadProcessOutput();
    void onProcessFinished(int exitCode, QProcess::ExitStatus exitStatus);
    void onCollectOutput();
    void onShowHelp();
    void onCleanBuild();
    void onLogMessage(const QString &text, const QString &color);
    void onToggleTheme();

private:
    void applyTheme();
    void updateThemeButton();
    void setupUI();
    void appendLog(const QString &text, const QString &color = "#e0e0e0");
    void saveConfig();
    void loadConfig();
    
    QTabWidget *m_tabWidget;
    MainTab *m_mainTab;
    
    CodeEditor *m_mkInfoText;
    QTextEdit *m_outputLog;
    QProgressBar *m_progressBar;
    
    QPushButton *m_btnInject;
    QPushButton *m_btnBuild;
    QPushButton *m_btnStopBuild;
    QPushButton *m_btnCollect;
    QPushButton *m_btnHelp;
    QPushButton *m_btnClean;
    QToolButton *m_btnTheme;
    
    QProcess *m_process;
    bool m_darkMode = false;
};

#endif
