#include "MainWindow.h"
#include "ConfigManager.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QFileDialog>
#include <QMessageBox>
#include <QFileInfo>
#include <QDir>
#include <QDirIterator>
#include <QScrollBar>
#include <QCoreApplication>
#include <QRegularExpression>
#include <QCloseEvent>
#include <QDialog>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), m_process(nullptr) {
    setWindowTitle("OLLVM 混淆编译配置工具");
    setMinimumSize(600, 500);
    resize(640, 540);
    setupUI();
    loadConfig();
    m_mainTab->loadDefaultNdk();
    m_outputLog->clear();
    m_mainTab->loadMkContent();
}

MainWindow::~MainWindow() {
    saveConfig();
    if (m_process && m_process->state() != QProcess::NotRunning) {
        m_process->kill();
        m_process->waitForFinished(3000);
    }
}

void MainWindow::closeEvent(QCloseEvent *event) {
    saveConfig();
    event->accept();
}

void MainWindow::setupUI() {
    auto *centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    auto *mainLayout = new QVBoxLayout(centralWidget);
    mainLayout->setSpacing(2);
    mainLayout->setContentsMargins(2, 2, 2, 2);

    auto *titleLabel = new QLabel("<h2 style='color:#00d4aa;'>OLLVM 代码混淆编译器</h2>"
                                  "<p style='color:#808090;'>Android NDK 项目 Android.mk 一键注入 &amp; 编译</p>", this);
    mainLayout->addWidget(titleLabel);

    m_tabWidget = new QTabWidget(this);
    m_tabWidget->setStyleSheet(
        "QTabWidget::pane { border: 1px solid #3a3a5c; background: #1a1a2e; }"
        "QTabBar::tab { background: #252540; color: #808090; padding: 4px 14px; border: 1px solid #3a3a5c; }"
        "QTabBar::tab:selected { background: #1a1a2e; color: #00d4aa; border-bottom: 2px solid #00d4aa; }"
        "QTabBar::tab:hover { color: #c0c0d0; }");

    m_mainTab = new MainTab(this);
    connect(m_mainTab, &MainTab::logMessage, this, &MainWindow::onLogMessage);
    connect(m_mainTab, &MainTab::jniFolderChanged, this, [this]() {
        m_mainTab->loadMkContent();
    });
    m_tabWidget->addTab(m_mainTab, "主界面");

    auto *tabMk = new QWidget();
    auto *mkLayout = new QVBoxLayout(tabMk);
    mkLayout->setContentsMargins(2, 2, 2, 2);
    auto *mkTextEdit = new QTextEdit(tabMk);
    mkTextEdit->setTextInteractionFlags(Qt::TextEditorInteraction);
    mkTextEdit->setStyleSheet("QTextEdit{font-family:Consolas,monospace;font-size:12px;background-color:#000000;color:#ffffff;}");
    mkTextEdit->setTabStopDistance(24);
    mkTextEdit->setPlaceholderText("Android.mk 内容将显示在这里...");
    mkLayout->addWidget(mkTextEdit);
    m_tabWidget->addTab(tabMk, "MK 信息");
    m_mkInfoText = mkTextEdit;
    connect(m_mainTab, &MainTab::mkContentChanged, mkTextEdit, &QTextEdit::setPlainText);

    auto *tabOut = new QWidget();
    auto *outTabLayout = new QVBoxLayout(tabOut);
    outTabLayout->setContentsMargins(2, 2, 2, 2);
    m_progressBar = new QProgressBar(tabOut);
    m_progressBar->setRange(0, 0);
    m_progressBar->setVisible(false);
    outTabLayout->addWidget(m_progressBar);
    m_outputLog = new QTextEdit(tabOut);
    m_outputLog->setReadOnly(true);
    m_outputLog->setTextInteractionFlags(Qt::TextSelectableByMouse | Qt::TextSelectableByKeyboard);
    outTabLayout->addWidget(m_outputLog);
    m_tabWidget->addTab(tabOut, "编译输出");

    mainLayout->addWidget(m_tabWidget, 1);

    auto *actionLayout = new QHBoxLayout();
    actionLayout->setSpacing(6);
    actionLayout->setContentsMargins(4, 0, 4, 4);

    m_btnInject = new QPushButton("一键注入", this);
    m_btnInject->setFixedHeight(34);
    m_btnInject->setMinimumWidth(170);
    m_btnInject->setStyleSheet(
        "QPushButton{background:#e67e22;color:#1a1a2e;font-size:13px;font-weight:bold;padding:4px 16px;}"
        "QPushButton:hover{background:#f39c12;}"
        "QPushButton:disabled{background:#3a3a5c;color:#606070;}");
    connect(m_btnInject, &QPushButton::clicked, this, &MainWindow::onInjectFlags);
    actionLayout->addWidget(m_btnInject);

    m_btnBuild = new QPushButton("一键编译 (ndk-build)", this);
    m_btnBuild->setFixedHeight(34);
    m_btnBuild->setMinimumWidth(180);
    m_btnBuild->setStyleSheet(
        "QPushButton{background:#00d4aa;color:#1a1a2e;font-size:13px;font-weight:bold;padding:4px 16px;}"
        "QPushButton:hover{background:#00eebb;}"
        "QPushButton:disabled{background:#3a3a5c;color:#606070;}");
    connect(m_btnBuild, &QPushButton::clicked, this, &MainWindow::onBuild);
    actionLayout->addWidget(m_btnBuild);

    m_btnStopBuild = new QPushButton("⏹ 停止编译", this);
    m_btnStopBuild->setFixedHeight(34);
    m_btnStopBuild->setMinimumWidth(120);
    m_btnStopBuild->setVisible(false);
    m_btnStopBuild->setStyleSheet(
        "QPushButton{background:#e74c3c;color:#e0e0e0;font-size:13px;font-weight:bold;padding:4px 16px;}"
        "QPushButton:hover{background:#c0392b;}");
    connect(m_btnStopBuild, &QPushButton::clicked, this, [this]() {
        if (m_process && m_process->state() != QProcess::NotRunning) {
            m_process->kill();
            appendLog("", "#ff4444");
            appendLog("=== 编译已被用户强制终止 ===", "#ff4444");
        }
    });
    actionLayout->addWidget(m_btnStopBuild);

    m_btnCollect = new QPushButton("收集产物到输出", this);
    m_btnCollect->setFixedHeight(34);
    m_btnCollect->setMinimumWidth(160);
    m_btnCollect->setStyleSheet(
        "QPushButton{background:#3498db;color:#1a1a2e;font-size:13px;font-weight:bold;padding:4px 16px;}"
        "QPushButton:hover{background:#5dade2;}"
        "QPushButton:disabled{background:#3a3a5c;color:#606070;}");
    connect(m_btnCollect, &QPushButton::clicked, this, &MainWindow::onCollectOutput);
    actionLayout->addWidget(m_btnCollect);

    m_btnHelp = new QPushButton("帮助文档", this);
    m_btnHelp->setFixedHeight(34);
    m_btnHelp->setMinimumWidth(100);
    m_btnHelp->setStyleSheet(
        "QPushButton{background:#8e44ad;color:#1a1a2e;font-size:13px;font-weight:bold;padding:4px 16px;}"
        "QPushButton:hover{background:#a569bd;}"
        "QPushButton:disabled{background:#3a3a5c;color:#606070;}");
    connect(m_btnHelp, &QPushButton::clicked, this, &MainWindow::onShowHelp);
    actionLayout->addWidget(m_btnHelp);

    m_btnClean = new QPushButton("清理控制台", this);
    m_btnClean->setFixedHeight(34);
    m_btnClean->setMinimumWidth(100);
    m_btnClean->setStyleSheet(
        "QPushButton{background:#3a4a5a;color:#e0e0e0;font-size:12px;padding:4px 12px;}"
        "QPushButton:hover{background:#4a5a6a;}"
        "QPushButton:disabled{background:#2a2a3c;color:#606070;}");
    connect(m_btnClean, &QPushButton::clicked, this, &MainWindow::onCleanBuild);
    actionLayout->addWidget(m_btnClean);

    actionLayout->addStretch();
    mainLayout->addLayout(actionLayout);
}

void MainWindow::onLogMessage(const QString &text, const QString &color) {
    appendLog(text, color);
}

void MainWindow::appendLog(const QString &text, const QString &color) {
    QString escaped = text.toHtmlEscaped();
    escaped.replace('\n', "<br>");
    m_outputLog->append("<span style='color:" + color + ";'>" + escaped + "</span>");
    QScrollBar *sb = m_outputLog->verticalScrollBar();
    sb->setValue(sb->maximum());
}

void MainWindow::onInjectFlags() {
    saveConfig();

    QString jniPath = m_mainTab->jniFolder();
    if (jniPath.isEmpty()) {
        QMessageBox::warning(this, "错误", "请先选择 jni 文件夹！");
        return;
    }

    QString mkFile = jniPath + "/Android.mk";
    if (!QFileInfo::exists(mkFile)) {
        QMessageBox::warning(this, "错误", "Android.mk 不存在！");
        return;
    }

    QStringList flags;
    auto passChecks = m_mainTab->passChecks();
    for (const auto &pc : passChecks) {
        if (pc.chk->isChecked()) {
            flags << "-mllvm" << "-" + pc.flag;
        }
        if (pc.levelCombo && pc.chk->isChecked()) {
            int lvl = pc.levelCombo->currentIndex() + 1;
            flags << "-mllvm" << "-level-" + pc.flag.mid(6) + "=" + QString::number(lvl);
        }
    }

    bool hasVmp = false;
    for (const auto &pc : passChecks) {
        if (pc.chk->isChecked() && pc.flag == "irobf-vmp") {
            hasVmp = true;
            break;
        }
    }
    if (hasVmp) {
        flags << "-frtti" << "-fno-exceptions";
    }

    if (flags.isEmpty()) {
        QMessageBox::warning(this, "错误", "请至少选择一个混淆功能！");
        return;
    }

    QStringList injectFlags;
    injectFlags << "-mllvm" << "-irobf" << flags;

    QString backupFile = mkFile + ".bak";
    if (!QFileInfo::exists(backupFile)) {
        QFile::copy(mkFile, backupFile);
        appendLog("[备份] 已创建备份: " + backupFile, "#f39c12");
    }

    QFile f(mkFile);
    if (!f.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::warning(this, "错误", "无法读取 Android.mk");
        return;
    }
    QString content = QString::fromUtf8(f.readAll());
    f.close();

    QStringList lines = content.split('\n');
    QStringList newLines;

    for (int i = 0; i < lines.size(); i++) {
        QString trimmed = lines[i].trimmed();

        if (trimmed.startsWith("include $(BUILD_EXECUTABLE)")) {
            if (!injectFlags.isEmpty()) {
                newLines.append("LOCAL_CFLAGS += " + injectFlags.join(' '));
                newLines.append("LOCAL_CPPFLAGS += " + injectFlags.join(' '));
            }
        }

        if (trimmed.startsWith("LOCAL_CFLAGS") && trimmed.contains("-mllvm") && trimmed.contains("-irobf")) {
            continue;
        }
        if (trimmed.startsWith("LOCAL_CPPFLAGS") && trimmed.contains("-mllvm") && trimmed.contains("-irobf")) {
            continue;
        }

        newLines.append(lines[i]);
    }

    if (!f.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate)) {
        QMessageBox::warning(this, "错误", "无法写入 Android.mk");
        return;
    }
    f.write(newLines.join('\n').toUtf8());
    f.close();

    appendLog("[注入] 已注入混淆标志", "#ffffff;background:#8B8000;padding:2px 4px;border-radius:2px");
    appendLog("  LOCAL_CFLAGS += " + injectFlags.join(' '), "#ffffff;background:#5a4a00;padding:2px 4px;border-radius:2px");
    appendLog("  LOCAL_CPPFLAGS += " + injectFlags.join(' '), "#ffffff;background:#5a4a00;padding:2px 4px;border-radius:2px");

    m_mainTab->loadMkContent();
    m_tabWidget->setCurrentIndex(1);
}

void MainWindow::onBuild() {
    QString jniPath = m_mainTab->jniFolder();
    if (jniPath.isEmpty()) {
        QMessageBox::warning(this, "错误", "请先选择 jni 文件夹！");
        return;
    }

    QString ndkPath = m_mainTab->ndkPath();
    if (ndkPath.isEmpty()) {
        QMessageBox::warning(this, "错误", "请先设置 NDK 路径！");
        return;
    }

    QString ndkBuild = QDir::toNativeSeparators(ndkPath + "/ndk-build.cmd");
    if (!QFileInfo::exists(ndkBuild)) {
        QMessageBox::warning(this, "错误", "ndk-build.cmd 不存在于: " + ndkBuild);
        return;
    }

    QDir d(jniPath);
    d.cdUp();
    QString projectDir = d.absolutePath();

    m_tabWidget->setCurrentIndex(2);

    auto removeDir = [&](const QString &dir) {
        QDir rd(dir);
        if (rd.exists()) {
            rd.removeRecursively();
            appendLog("[清理] " + QDir::toNativeSeparators(dir), "#e74c3c");
        }
    };
    removeDir(projectDir + "/libs");
    removeDir(projectDir + "/obj");

    QString cmd = ndkBuild;

    if (m_process) {
        delete m_process;
    }
    m_process = new QProcess(this);
    connect(m_process, &QProcess::readyReadStandardOutput, this, &MainWindow::onReadProcessOutput);
    connect(m_process, &QProcess::readyReadStandardError, this, &MainWindow::onReadProcessOutput);
    connect(m_process, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
            this, &MainWindow::onProcessFinished);

    appendLog("=====================================", "#00d4aa");
    appendLog("正在编译项目: " + projectDir, "#e0e0e0");
    appendLog("NDK 命令: " + cmd, "#a0a0b0");
    appendLog("=====================================", "#00d4aa");

    m_progressBar->setVisible(true);
    m_btnBuild->setEnabled(false);
    m_btnInject->setEnabled(false);
    m_btnCollect->setEnabled(false);
    m_btnStopBuild->setVisible(true);

    m_process->setWorkingDirectory(projectDir);
    m_process->start(cmd, QStringList());
}

void MainWindow::onReadProcessOutput() {
    auto *proc = qobject_cast<QProcess *>(sender());
    if (!proc) return;

    QString stdOut = QString::fromUtf8(proc->readAllStandardOutput());
    if (!stdOut.isEmpty()) {
        appendLog(stdOut, "#c0c0c0");
    }

    QString stdErr = QString::fromUtf8(proc->readAllStandardError());
    if (!stdErr.isEmpty()) {
        appendLog(stdErr, "#ff6644");
    }
}

void MainWindow::onProcessFinished(int exitCode, QProcess::ExitStatus exitStatus) {
    m_progressBar->setVisible(false);
    m_btnBuild->setEnabled(true);
    m_btnInject->setEnabled(true);
    m_btnCollect->setEnabled(true);
    m_btnStopBuild->setVisible(false);

    if (exitStatus == QProcess::NormalExit && exitCode == 0) {
        appendLog("", "#00d4aa");
        appendLog("=== 编译成功！===", "#00ff88");
        appendLog("产物位于项目目录的 libs 文件夹中", "#a0a0b0");
    } else {
        appendLog("", "#ff4444");
        appendLog("=== 编译失败（退出码: " + QString::number(exitCode) + "）===", "#ff4444");
    }
}

void MainWindow::onCollectOutput() {
    QString jniPath = m_mainTab->jniFolder();
    if (jniPath.isEmpty()) {
        QMessageBox::warning(this, "错误", "请先选择 jni 文件夹！");
        return;
    }

    QString outputFolder = m_mainTab->outputFolder();
    if (outputFolder.isEmpty()) {
        QMessageBox::warning(this, "错误", "请先设置输出文件夹！");
        return;
    }

    QDir jniDir(jniPath);
    jniDir.cdUp();
    QString projectDir = jniDir.absolutePath();
    QString libsDir = projectDir + "/libs";

    if (!QDir(libsDir).exists()) {
        QMessageBox::warning(this, "错误", "libs 目录不存在，请先编译！\n路径: " + libsDir);
        return;
    }

    int totalCopied = 0;
    QDir libsQDir(libsDir);
    QStringList archDirs = libsQDir.entryList(QDir::Dirs | QDir::NoDotAndDotDot);

    QDir outDir(outputFolder);
    if (!outDir.exists()) {
        outDir.mkpath(".");
    }

    for (const auto &arch : archDirs) {
        QString archPath = libsDir + "/" + arch;
        QDir archQDir(archPath);
        QStringList files = archQDir.entryList(QDir::Files | QDir::NoDotAndDotDot);

        QString archOutPath = outputFolder + "/" + arch;
        QDir archOutDir(archOutPath);
        if (!archOutDir.exists()) {
            archOutDir.mkpath(".");
        }

        for (const auto &file : files) {
            QString src = archPath + "/" + file;
            QString dst = archOutPath + "/" + file;

            if (QFile::exists(dst)) {
                QFile::remove(dst);
            }

            if (QFile::copy(src, dst)) {
                totalCopied++;
                appendLog(QString("[收集] %1/%2 -> %3/%2").arg(arch, file, QDir::toNativeSeparators(archOutPath)), "#00ff88");
            } else {
                appendLog(QString("[失败] %1/%2").arg(arch, file), "#ff4444");
            }
        }
    }

    if (totalCopied > 0) {
        appendLog(QString("\n[完成] 共收集 %1 个文件 -> %2").arg(totalCopied).arg(QDir::toNativeSeparators(outputFolder)), "#00ff88");
        QMessageBox::information(this, "收集完成",
            QString("已从 libs 收集 %1 个文件到:\n%2").arg(totalCopied).arg(QDir::toNativeSeparators(outputFolder)));
    } else {
        appendLog("[警告] libs 目录下未找到文件", "#cc6600");
        QMessageBox::warning(this, "警告", "libs 目录下未找到任何编译产物！\n请确保已执行编译。");
    }
}

void MainWindow::onCleanBuild() {
    m_outputLog->clear();
    appendLog("[控制台] 已清屏", "#a0a0b0");
}

void MainWindow::onShowHelp() {
    QString md = R"MD(
# OLLVM 混淆编译配置工具

基于 LLVM 21.x 的 Android NDK 代码混淆一键配置工具，通过修改 `Android.mk` 自动注入编译标志实现混淆保护。

## 快速上手

### 第一步：选择项目目录

点击 **jni 文件夹** 右侧的 `选择` 按钮，选择你的 NDK 项目目录。

### 第二步：选择混淆功能

在 **混淆功能** 区域勾选需要的保护。

### 第三步：注入 Android.mk

点击 **一键注入** 按钮，自动注入混淆标志。

### 第四步：一键编译

确认 **NDK 路径** 正确后，点击 **一键编译 (ndk-build)**。

### 第五步：收集产物

点击 **收集产物到输出** 将编译产物复制到输出目录。
)MD";

    auto *dialog = new QDialog(this);
    dialog->setWindowTitle("帮助文档 - OLLVM 混淆编译配置工具");
    dialog->resize(800, 600);

    auto *layout = new QVBoxLayout(dialog);
    auto *textEdit = new QTextEdit(dialog);
    textEdit->setReadOnly(true);
    textEdit->setStyleSheet(
        "QTextEdit { background: #1a1a2e; color: #c0c0d0; border: none; }");
    textEdit->setHtml(mdToHtml(md));
    layout->addWidget(textEdit);

    auto *btnClose = new QPushButton("关闭", dialog);
    btnClose->setStyleSheet(
        "QPushButton { background: #3a3a5c; color: #e0e0e0; padding: 8px 30px; border-radius: 4px; }"
        "QPushButton:hover { background: #4a4a6c; }");
    connect(btnClose, &QPushButton::clicked, dialog, &QDialog::accept);
    layout->addWidget(btnClose, 0, Qt::AlignCenter);

    dialog->exec();
    dialog->deleteLater();
}

QString MainWindow::mdToHtml(const QString &md) {
    QStringList lines = md.split('\n');
    QString html;
    html += "<style>"
            "body { font-family: 'Microsoft YaHei', sans-serif; font-size: 14px; line-height: 1.8; }"
            "h1 { color: #00d4aa; font-size: 22px; border-bottom: 2px solid #3a3a5c; padding-bottom: 6px; }"
            "h2 { color: #3498db; font-size: 18px; border-bottom: 1px solid #3a3a5c; padding-bottom: 4px; }"
            "h3 { color: #e67e22; font-size: 15px; }"
            "code { background: #252540; color: #00ff88; padding: 2px 6px; border-radius: 3px; font-family: Consolas, monospace; }"
            "pre { background: #12122a; border: 1px solid #3a3a5c; border-radius: 6px; padding: 12px; overflow-x: auto; }"
            "p { color: #c0c0d0; }"
            "</style>";

    for (const auto &line : lines) {
        QString processed = line.toHtmlEscaped();
        
        if (processed.startsWith("### ")) {
            html += "<h3>" + processed.mid(4) + "</h3>\n";
        } else if (processed.startsWith("## ")) {
            html += "<h2>" + processed.mid(3) + "</h2>\n";
        } else if (processed.startsWith("# ")) {
            html += "<h1>" + processed.mid(2) + "</h1>\n";
        } else if (processed.startsWith("- ")) {
            html += "<li>" + processed.mid(2) + "</li>\n";
        } else if (!processed.isEmpty()) {
            html += "<p>" + processed + "</p>\n";
        }
    }

    return html;
}

void MainWindow::saveConfig() {
    ConfigManager::instance().saveConfig(
        m_mainTab->jniFolder(), m_mainTab->ndkPath(), m_mainTab->outputFolder(),
        m_mainTab->optLevel(), m_mainTab->passChecks()
    );
}

void MainWindow::loadConfig() {
    QString jniFolder, ndkPath, outputFolder;
    int optLevel;
    QList<PassCheckBox> passChecks = m_mainTab->passChecks();
    
    ConfigManager::instance().loadConfig(
        jniFolder, ndkPath, outputFolder, optLevel, passChecks
    );
    
    m_mainTab->setJniFolder(jniFolder);
    m_mainTab->setNdkPath(ndkPath);
    m_mainTab->setOutputFolder(outputFolder);
    m_mainTab->setOptLevel(optLevel);
    m_mainTab->updatePassChecks(passChecks);
}

void MainWindow::keyPressEvent(QKeyEvent *event) {
    QMainWindow::keyPressEvent(event);
}
