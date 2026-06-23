#include "MainWindow.h"
#include "ConfigManager.h"
#include "HelpDocument.h"
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
#include <QSettings>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), m_process(nullptr) {
    setWindowTitle("ALLVM 混淆编译配置工具");
    setMinimumSize(700, 550);
    resize(720, 580);
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
    centralWidget->setObjectName("centralWidget");

    auto *mainLayout = new QVBoxLayout(centralWidget);
    mainLayout->setSpacing(12);
    mainLayout->setContentsMargins(12, 0, 12, 12);

    m_tabWidget = new QTabWidget(this);
    m_btnTheme = new QToolButton(m_tabWidget);
    m_btnTheme->setFixedSize(38, 38);
    m_btnTheme->setCursor(Qt::PointingHandCursor);
    connect(m_btnTheme, &QToolButton::clicked, this, &MainWindow::onToggleTheme);
    m_tabWidget->setCornerWidget(m_btnTheme, Qt::TopRightCorner);

    m_mainTab = new MainTab(this);
    connect(m_mainTab, &MainTab::logMessage, this, &MainWindow::onLogMessage);
    connect(m_mainTab, &MainTab::jniFolderChanged, this, [this]() {
        m_mainTab->loadMkContent();
    });
    m_tabWidget->addTab(m_mainTab, "主界面");

    auto *tabMk = new QWidget();
    auto *mkLayout = new QVBoxLayout(tabMk);
    mkLayout->setContentsMargins(0, 0, 0, 0);
    m_mkInfoText = new CodeEditor(tabMk);
    m_mkInfoText->setPlaceholderText("; Android.mk 内容将显示在这里...\n; 支持语法高亮和行号显示");
    mkLayout->addWidget(m_mkInfoText);
    m_tabWidget->addTab(tabMk, "MK 编辑器");
    connect(m_mainTab, &MainTab::mkContentChanged, m_mkInfoText, &CodeEditor::setPlainText);

    auto *tabOut = new QWidget();
    auto *outTabLayout = new QVBoxLayout(tabOut);
    outTabLayout->setContentsMargins(12, 12, 12, 12);
    m_progressBar = new QProgressBar(tabOut);
    m_progressBar->setRange(0, 0);
    m_progressBar->setVisible(false);
    outTabLayout->addWidget(m_progressBar);
    m_outputLog = new QTextEdit(tabOut);
    m_outputLog->setReadOnly(true);
    m_outputLog->setTextInteractionFlags(Qt::TextSelectableByMouse | Qt::TextSelectableByKeyboard);
    outTabLayout->addWidget(m_outputLog);
    m_tabWidget->addTab(tabOut, "控制台");

    mainLayout->addWidget(m_tabWidget, 1);

    auto *actionLayout = new QHBoxLayout();
    actionLayout->setSpacing(10);
    actionLayout->setContentsMargins(4, 0, 4, 4);

    m_btnInject = new QPushButton("一键注入", this);
    m_btnInject->setFixedHeight(34);
    m_btnInject->setMinimumWidth(170);
    m_btnInject->setStyleSheet(
        "QPushButton{background:#f97316;color:#ffffff;font-size:13px;font-weight:600;padding:4px 16px;border:none;border-radius:10px;}"
        "QPushButton:hover{background:#ea580c;}"
        "QPushButton:disabled{background:#cbd5e1;color:#f8fafc;}");
    connect(m_btnInject, &QPushButton::clicked, this, &MainWindow::onInjectFlags);
    actionLayout->addWidget(m_btnInject);

    m_btnBuild = new QPushButton("一键编译 (ndk-build)", this);
    m_btnBuild->setFixedHeight(34);
    m_btnBuild->setMinimumWidth(180);
    m_btnBuild->setStyleSheet(
        "QPushButton{background:#2563eb;color:#ffffff;font-size:13px;font-weight:600;padding:4px 16px;border:none;border-radius:10px;}"
        "QPushButton:hover{background:#1d4ed8;}"
        "QPushButton:disabled{background:#cbd5e1;color:#f8fafc;}");
    connect(m_btnBuild, &QPushButton::clicked, this, &MainWindow::onBuild);
    actionLayout->addWidget(m_btnBuild);

    m_btnStopBuild = new QPushButton("⏹ 停止编译", this);
    m_btnStopBuild->setFixedHeight(34);
    m_btnStopBuild->setMinimumWidth(120);
    m_btnStopBuild->setVisible(false);
    m_btnStopBuild->setStyleSheet(
        "QPushButton{background:#ef4444;color:#ffffff;font-size:13px;font-weight:600;padding:4px 16px;border:none;border-radius:10px;}"
        "QPushButton:hover{background:#dc2626;}");
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
        "QPushButton{background:#0ea5e9;color:#ffffff;font-size:13px;font-weight:600;padding:4px 16px;border:none;border-radius:10px;}"
        "QPushButton:hover{background:#0284c7;}"
        "QPushButton:disabled{background:#cbd5e1;color:#f8fafc;}");
    connect(m_btnCollect, &QPushButton::clicked, this, &MainWindow::onCollectOutput);
    actionLayout->addWidget(m_btnCollect);

    m_btnHelp = new QPushButton("帮助文档", this);
    m_btnHelp->setFixedHeight(34);
    m_btnHelp->setMinimumWidth(100);
    m_btnHelp->setStyleSheet(
        "QPushButton{background:#8b5cf6;color:#ffffff;font-size:13px;font-weight:600;padding:4px 16px;border:none;border-radius:10px;}"
        "QPushButton:hover{background:#7c3aed;}"
        "QPushButton:disabled{background:#cbd5e1;color:#f8fafc;}");
    connect(m_btnHelp, &QPushButton::clicked, this, &MainWindow::onShowHelp);
    actionLayout->addWidget(m_btnHelp);

    m_btnClean = new QPushButton("清理控制台", this);
    m_btnClean->setFixedHeight(34);
    m_btnClean->setMinimumWidth(100);
    m_btnClean->setStyleSheet(
        "QPushButton{background:#ffffff;color:#475569;font-size:12px;padding:4px 12px;border:1px solid #d7deea;border-radius:10px;}"
        "QPushButton:hover{background:#f8fafc;}"
        "QPushButton:disabled{background:#eef2f7;color:#94a3b8;}");
    connect(m_btnClean, &QPushButton::clicked, this, &MainWindow::onCleanBuild);
    actionLayout->addWidget(m_btnClean);

    actionLayout->addStretch();
    mainLayout->addLayout(actionLayout);
    applyTheme();
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
    QStringList ldflags;  // linker flags (通过 LOCAL_LDFLAGS 传递)
    QStringList cflagsForWrapper;  // 壳子选项（需要在编译阶段传递以启用检测）
    bool hasLinker = false;
    bool hasGz = false;
    
    auto passChecks = m_mainTab->passChecks();
    for (const auto &pc : passChecks) {
        if (pc.chk->isChecked()) {
            // linker 和 gz 选项通过 LOCAL_LDFLAGS 传递
            if (pc.flag == "firobf-linker") {
                ldflags << "-" + pc.flag;
                cflagsForWrapper << "-" + pc.flag;
                hasLinker = true;
            } else if (pc.flag == "firobf-gz") {
                ldflags << "-" + pc.flag;
                cflagsForWrapper << "-" + pc.flag;
                hasGz = true;
            } else {
                flags << "-mllvm" << "-" + pc.flag;
            }
        }
        if (pc.levelCombo && pc.chk->isChecked()) {
            int lvl = pc.levelCombo->currentIndex() + 1;
            flags << "-mllvm" << "-level-" + pc.flag.mid(6) + "=" + QString::number(lvl);
        }
    }

    // 启用壳子时自动注入对应的环境变量检测
    if (hasLinker) {
        flags << "-mllvm" << "-irobf-envcheck";
        appendLog("[自动注入] linker 壳启用，自动添加环境变量检测 (-irobf-envcheck)", "#00d4aa");
    }
    if (hasGz) {
        flags << "-mllvm" << "-irobf-gzcheck";
        appendLog("[自动注入] gz 壳启用，自动添加环境变量检测 (-irobf-gzcheck)", "#00d4aa");
    }

    bool hasVmp = false;
    for (const auto &pc : passChecks) {
        if (pc.chk->isChecked() && pc.flag == "irobf-vmp") {
            hasVmp = true;
            break;
        }
    }

    if (flags.isEmpty() && ldflags.isEmpty()) {
        QMessageBox::warning(this, "错误", "请至少选择一个混淆功能！");
        return;
    }

    QStringList injectFlags;
    if (!flags.isEmpty()) {
        injectFlags << "-mllvm" << "-irobf" << flags;
    }
    
    // VMP需要禁用异常
    if (hasVmp) {
        injectFlags << "-fno-exceptions" << "-frtti";
    }

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
            // 壳子选项也需要在编译阶段传递（用于自动启用检测）
            if (!cflagsForWrapper.isEmpty()) {
                newLines.append("LOCAL_CFLAGS += " + cflagsForWrapper.join(' '));
            }
            if (!ldflags.isEmpty()) {
                newLines.append("LOCAL_LDFLAGS += " + ldflags.join(' '));
            }
        }

        if (trimmed.startsWith("LOCAL_CFLAGS") && trimmed.contains("-mllvm") && trimmed.contains("-irobf")) {
            continue;
        }
        if (trimmed.startsWith("LOCAL_CPPFLAGS") && trimmed.contains("-mllvm") && trimmed.contains("-irobf")) {
            continue;
        }
        if (trimmed.startsWith("LOCAL_LDFLAGS") && (trimmed.contains("-firobf-linker") || trimmed.contains("-firobf-gz"))) {
            continue;
        }
        // 跳过之前注入的壳子编译选项
        if (trimmed.startsWith("LOCAL_CFLAGS") && (trimmed.contains("-firobf-linker") || trimmed.contains("-firobf-gz"))) {
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
    if (!injectFlags.isEmpty()) {
        appendLog("  LOCAL_CFLAGS += " + injectFlags.join(' '), "#ffffff;background:#5a4a00;padding:2px 4px;border-radius:2px");
        appendLog("  LOCAL_CPPFLAGS += " + injectFlags.join(' '), "#ffffff;background:#5a4a00;padding:2px 4px;border-radius:2px");
    }
    if (!cflagsForWrapper.isEmpty()) {
        appendLog("  LOCAL_CFLAGS += " + cflagsForWrapper.join(' ') + "  (壳子选项，用于启用检测)", "#ffffff;background:#5a4a00;padding:2px 4px;border-radius:2px");
    }
    if (!ldflags.isEmpty()) {
        appendLog("  LOCAL_LDFLAGS += " + ldflags.join(' '), "#ffffff;background:#5a4a00;padding:2px 4px;border-radius:2px");
    }

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
    appendLog("NDK 命令: " + cmd + " -j32", "#a0a0b0");
    appendLog("=====================================", "#00d4aa");

    m_progressBar->setVisible(true);
    m_btnBuild->setEnabled(false);
    m_btnInject->setEnabled(false);
    m_btnCollect->setEnabled(false);
    m_btnStopBuild->setVisible(true);

    m_process->setWorkingDirectory(projectDir);
    m_process->start(cmd, QStringList() << "-j32");
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
    m_tabWidget->setCurrentIndex(2);
    m_outputLog->clear();
    appendLog("[控制台] 已清屏", "#a0a0b0");
}

void MainWindow::onShowHelp() {
    HelpDocument *dialog = new HelpDocument(this, m_darkMode);
    dialog->exec();
    dialog->deleteLater();
}

void MainWindow::saveConfig() {
    ConfigManager::instance().saveConfig(
        m_mainTab->jniFolder(), m_mainTab->ndkPath(), m_mainTab->outputFolder(),
        m_mainTab->optLevel(), m_mainTab->passChecks()
    );
    QSettings settings(ConfigManager::instance().configPath(), QSettings::IniFormat);
    settings.setValue("ui/darkMode", m_darkMode);
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

    QSettings settings(ConfigManager::instance().configPath(), QSettings::IniFormat);
    m_darkMode = settings.value("ui/darkMode", false).toBool();
    applyTheme();
}

void MainWindow::keyPressEvent(QKeyEvent *event) {
    QMainWindow::keyPressEvent(event);
}

void MainWindow::updateThemeButton() {
    m_btnTheme->setText(m_darkMode ? QString::fromUtf8("☀") : QString::fromUtf8("☾"));
    m_btnTheme->setToolTip(m_darkMode ? "切换到亮色模式" : "切换到暗色模式");
    m_btnTheme->setStyleSheet(m_darkMode
        ? "QToolButton{background:#1e293b;color:#f8fafc;border:1px solid #334155;border-radius:21px;font-size:18px;font-weight:700;}QToolButton:hover{background:#334155;}"
        : "QToolButton{background:#ffffff;color:#1f2937;border:1px solid #d7deea;border-radius:21px;font-size:18px;font-weight:700;}QToolButton:hover{background:#f8fafc;}");
}

void MainWindow::applyTheme() {
    const QString baseStyle = m_darkMode
        ? QString(
              "QMainWindow, QWidget#centralWidget { background: #020817; color: #e5eefb; }"
              "QLabel { color: #cbd5e1; }"
              "QLineEdit, QComboBox, QTextEdit, QPlainTextEdit {"
              "  background: #111827;"
              "  color: #f8fafc;"
              "  border: 1px solid #334155;"
              "  border-radius: 10px;"
              "  padding: 8px 10px;"
              "}"
              "QLineEdit:focus, QComboBox:focus, QTextEdit:focus, QPlainTextEdit:focus { border: 1px solid #60a5fa; }"
              "QTabWidget::pane { border: 1px solid #334155; background: #0f172a; border-radius: 16px; top: -1px; }"
              "QTabBar::tab { background: #1e293b; color: #94a3b8; padding: 10px 18px; border: none; border-top-left-radius: 10px; border-top-right-radius: 10px; margin-right: 6px; }"
              "QTabBar::tab:selected { background: #0f172a; color: #f8fafc; font-weight: 600; }"
              "QTabBar::tab:hover { color: #e2e8f0; }"
              "QProgressBar { background: #1e293b; border: none; border-radius: 8px; text-align: center; color: #e2e8f0; }"
              "QProgressBar::chunk { background: #2563eb; border-radius: 8px; }"
              "QPushButton { background: #111827; color: #f8fafc; border: 1px solid #334155; border-radius: 10px; padding: 8px 14px; }"
              "QPushButton:hover { background: #1f2937; border-color: #475569; }"
              "QPushButton:disabled { background: #0f172a; color: #64748b; border-color: #1e293b; }")
        : QString(
              "QMainWindow, QWidget#centralWidget { background: #f5f7fb; color: #1f2937; }"
              "QLabel { color: #334155; }"
              "QLineEdit, QComboBox, QTextEdit, QPlainTextEdit {"
              "  background: #ffffff;"
              "  color: #0f172a;"
              "  border: 1px solid #d7deea;"
              "  border-radius: 10px;"
              "  padding: 8px 10px;"
              "}"
              "QLineEdit:focus, QComboBox:focus, QTextEdit:focus, QPlainTextEdit:focus { border: 1px solid #60a5fa; }"
              "QTabWidget::pane { border: 1px solid #d7deea; background: #ffffff; border-radius: 16px; top: -1px; }"
              "QTabBar::tab { background: #e9eef6; color: #64748b; padding: 10px 18px; border: none; border-top-left-radius: 10px; border-top-right-radius: 10px; margin-right: 6px; }"
              "QTabBar::tab:selected { background: #ffffff; color: #2563eb; font-weight: 600; }"
              "QTabBar::tab:hover { color: #1d4ed8; }"
              "QProgressBar { background: #e2e8f0; border: none; border-radius: 8px; text-align: center; color: #334155; }"
              "QProgressBar::chunk { background: #3b82f6; border-radius: 8px; }"
              "QPushButton { background: #ffffff; color: #1f2937; border: 1px solid #d7deea; border-radius: 10px; padding: 8px 14px; }"
              "QPushButton:hover { background: #f8fafc; border-color: #bfd2ef; }"
              "QPushButton:disabled { background: #eef2f7; color: #94a3b8; border-color: #e2e8f0; }");
    setStyleSheet(baseStyle);

    m_outputLog->setStyleSheet(m_darkMode
        ? "QTextEdit { background-color: #111827; color: #dbeafe; font-family: 'JetBrains Mono', 'Fira Code', 'Consolas', monospace; font-size: 12px; border: 1px solid #334155; border-radius: 14px; padding: 8px; }"
        : "QTextEdit { background-color: #ffffff; color: #334155; font-family: 'JetBrains Mono', 'Fira Code', 'Consolas', monospace; font-size: 12px; border: 1px solid #d7deea; border-radius: 14px; padding: 8px; }");

    m_mkInfoText->setDarkMode(m_darkMode);
    m_mainTab->setDarkMode(m_darkMode);
    updateThemeButton();
}

void MainWindow::onToggleTheme() {
    m_darkMode = !m_darkMode;
    applyTheme();
    saveConfig();
}
