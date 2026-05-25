#include "MainTab.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QGroupBox>
#include <QLabel>
#include <QFileDialog>
#include <QFile>
#include <QFileInfo>
#include <QDir>
#include <QCoreApplication>
#include <QScrollArea>
#include <QSet>

MainTab::MainTab(QWidget *parent) : QWidget(parent) {
    setupUI();
    
    m_mkSaveTimer = new QTimer(this);
    m_mkSaveTimer->setSingleShot(true);
    m_mkSaveTimer->setInterval(1200);
    connect(m_mkSaveTimer, &QTimer::timeout, this, &MainTab::onMkTextChanged);
    connect(m_mkInfoText, &QTextEdit::textChanged, this, [this]() {
        m_mkSaveTimer->start();
    });
}

QString MainTab::jniFolder() const {
    return m_jniFolderEdit->text();
}

QString MainTab::ndkPath() const {
    return m_ndkPathEdit->text();
}

QString MainTab::outputFolder() const {
    return m_outputFolderEdit->text();
}

int MainTab::optLevel() const {
    return m_cmbOptLevel->currentIndex();
}

QList<PassCheckBox> MainTab::passChecks() const {
    return m_passChecks;
}

QString MainTab::currentMkFile() const {
    return m_currentMkFile;
}

QTextEdit* MainTab::mkInfoText() {
    return m_mkInfoText;
}

void MainTab::setJniFolder(const QString &path) {
    m_jniFolderEdit->setText(path);
}

void MainTab::setNdkPath(const QString &path) {
    m_ndkPathEdit->setText(path);
}

void MainTab::setOutputFolder(const QString &path) {
    m_outputFolderEdit->setText(path);
}

void MainTab::setOptLevel(int level) {
    m_cmbOptLevel->setCurrentIndex(level);
}

void MainTab::updatePassChecks(const QList<PassCheckBox> &checks) {
    m_passChecks = checks;
}

void MainTab::setupUI() {
    auto *mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(3);
    mainLayout->setContentsMargins(4, 4, 4, 4);

    auto *settingsGroup = new QGroupBox("项目设置", this);
    auto *settingsLayout = new QGridLayout(settingsGroup);
    settingsLayout->setSpacing(3);
    settingsLayout->setContentsMargins(6, 10, 6, 6);

    settingsLayout->addWidget(new QLabel("jni 文件夹:", settingsGroup), 0, 0);
    auto *jniLayout = new QHBoxLayout();
    m_jniFolderEdit = new QLineEdit(settingsGroup);
    m_jniFolderEdit->setPlaceholderText("jni 目录（可选项目根目录自动识别）");
    connect(m_jniFolderEdit, &QLineEdit::textChanged, this, &MainTab::loadMkContent);
    jniLayout->addWidget(m_jniFolderEdit);
    auto *btnJniFolder = new QPushButton("选择", settingsGroup);
    btnJniFolder->setFixedSize(100, 28);
    connect(btnJniFolder, &QPushButton::clicked, this, &MainTab::onSelectJniFolder);
    jniLayout->addWidget(btnJniFolder);
    m_btnRefreshMk = new QPushButton("刷新", settingsGroup);
    m_btnRefreshMk->setFixedSize(100, 28);
    connect(m_btnRefreshMk, &QPushButton::clicked, this, &MainTab::onRefreshMkInfo);
    jniLayout->addWidget(m_btnRefreshMk);
    settingsLayout->addLayout(jniLayout, 0, 1);

    settingsLayout->addWidget(new QLabel("NDK 路径:", settingsGroup), 1, 0);
    auto *ndkLayout = new QHBoxLayout();
    m_ndkPathEdit = new QLineEdit(settingsGroup);
    m_ndkPathEdit->setPlaceholderText("NDK 根目录");
    ndkLayout->addWidget(m_ndkPathEdit);
    m_btnDefaultNdk = new QPushButton("自动检测", settingsGroup);
    m_btnDefaultNdk->setFixedSize(120, 28);
    connect(m_btnDefaultNdk, &QPushButton::clicked, this, &MainTab::loadDefaultNdk);
    ndkLayout->addWidget(m_btnDefaultNdk);
    auto *btnBrowseNdk = new QPushButton("选择", settingsGroup);
    btnBrowseNdk->setFixedSize(100, 28);
    connect(btnBrowseNdk, &QPushButton::clicked, this, &MainTab::onSelectNdkPath);
    ndkLayout->addWidget(btnBrowseNdk);
    settingsLayout->addLayout(ndkLayout, 1, 1);

    settingsLayout->addWidget(new QLabel("输出文件夹:", settingsGroup), 2, 0);
    auto *outLayout = new QHBoxLayout();
    m_outputFolderEdit = new QLineEdit(settingsGroup);
    m_outputFolderEdit->setPlaceholderText("编译产物输出目录");
    outLayout->addWidget(m_outputFolderEdit);
    auto *btnOutputFolder = new QPushButton("选择", settingsGroup);
    btnOutputFolder->setFixedSize(100, 28);
    connect(btnOutputFolder, &QPushButton::clicked, this, &MainTab::onSelectOutputFolder);
    outLayout->addWidget(btnOutputFolder);
    settingsLayout->addLayout(outLayout, 2, 1);

    settingsLayout->addWidget(new QLabel("优化等级:", settingsGroup), 3, 0);
    m_cmbOptLevel = new QComboBox(settingsGroup);
    m_cmbOptLevel->addItems({"O0", "O1", "O2", "Os", "Oz", "O3"});
    m_cmbOptLevel->setCurrentIndex(2);
    m_cmbOptLevel->setFixedWidth(100);
    m_cmbOptLevel->setStyleSheet("QComboBox{padding:2px 8px;}");
    settingsLayout->addWidget(m_cmbOptLevel, 3, 1);

    mainLayout->addWidget(settingsGroup);

    auto *obfGroup = new QGroupBox("混淆功能", this);
    auto *obfLayout = new QGridLayout(obfGroup);
    obfLayout->setSpacing(3);
    obfLayout->setContentsMargins(6, 6, 6, 6);

    struct PassInfo { QString label; QString flag; };
    QList<PassInfo> passes = {
        {"控制流平坦化", "irobf-fla"},
        {"间接分支", "irobf-indbr"},
        {"间接调用", "irobf-icall"},
        {"全局变量间接化", "irobf-indgv"},
        {"字符串加密", "irobf-cse"},
        {"常量整数加密", "irobf-cie"},
        {"常量浮点加密", "irobf-cfe"},
        {"RTTI 擦除", "irobf-rtti"},
        {"系统调用保护", "irobf-syscall"},
        {"内存保护", "irobf-memprotect"},
        {"LD_PRELOAD 检测", "irobf-ldpreload"},
        {"虚拟机检测", "irobf-vmdetect"},
        {"USB 调试保护", "irobf-usb"},
        {"IDA 调试器检测", "irobf-ida"},
        {"VPN 检测", "irobf-vpn"},
        {"代理/iptables 检测", "irobf-proxy"},
        {"时间差检测", "irobf-time"},
        {"Hosts 文件检测", "irobf-hosts"},
        {"内存检测", "irobf-mem"},
        {"Ptrace 检测", "irobf-ptrace"},
        {"Inline Hook 检测", "irobf-inlinehook"},
        {"PLT Hook 检测", "irobf-plthook"},
        {"隐藏 Maps (需Root)", "irobf-hidemaps"},
        {"伪造 Maps 内容", "irobf-fakemaps"},
        {"Root 检测", "irobf-root"},
        {"非 Root 检测", "irobf-noroot"},
        {"调试日志", "irobf-debug"},
    };

    QSet<QString> levelPasses = {
        "irobf-indbr", "irobf-icall", "irobf-indgv",
        "irobf-cie", "irobf-cfe"
    };

    int col = 0, row = 0;
    for (const auto &p : passes) {
        if (levelPasses.contains(p.flag)) {
            auto *container = new QWidget(obfGroup);
            auto *cl = new QHBoxLayout(container);
            cl->setContentsMargins(0, 0, 0, 0);
            cl->setSpacing(2);

            auto *chk = new QCheckBox(p.label, container);
            chk->setToolTip("-mllvm -" + p.flag);
            connect(chk, &QCheckBox::toggled, this, &MainTab::onOptionChanged);
            cl->addWidget(chk);

            auto *lvl = new QComboBox(container);
            lvl->addItems({"L1", "L2", "L3"});
            lvl->setCurrentIndex(0);
            lvl->setMaximumWidth(48);
            lvl->setStyleSheet("QComboBox{background:#1a2a3a;color:#e0e0e0;border:1px solid #3a4a5a;border-radius:2px;padding:1px 4px;font-size:10px;}QComboBox:hover{background:#2a3a4a;}QComboBox QAbstractItemView{background:#1a2a3a;color:#e0e0e0;selection-background-color:#0088cc;}");
            cl->addWidget(lvl);
            cl->addStretch();

            obfLayout->addWidget(container, row, col);
            m_passChecks.append({chk, lvl, p.flag});
        } else {
            auto *chk = new QCheckBox(p.label, obfGroup);
            chk->setToolTip("-mllvm -" + p.flag);
            connect(chk, &QCheckBox::toggled, this, &MainTab::onOptionChanged);
            obfLayout->addWidget(chk, row, col);
            m_passChecks.append({chk, nullptr, p.flag});
        }
        col++;
        if (col >= 3) { col = 0; row++; }
    }

    QCheckBox *rootChk = nullptr;
    QCheckBox *noRootChk = nullptr;
    for (const auto &pc : m_passChecks) {
        if (pc.flag == "irobf-root") rootChk = pc.chk;
        if (pc.flag == "irobf-noroot") noRootChk = pc.chk;
    }
    if (rootChk && noRootChk) {
        connect(rootChk, &QCheckBox::toggled, this, [noRootChk](bool checked) {
            if (checked) { noRootChk->blockSignals(true); noRootChk->setChecked(false); noRootChk->blockSignals(false); }
        });
        connect(noRootChk, &QCheckBox::toggled, this, [rootChk](bool checked) {
            if (checked) { rootChk->blockSignals(true); rootChk->setChecked(false); rootChk->blockSignals(false); }
        });
    }

    auto *vmpChk = new QCheckBox("VMP 虚拟机保护", obfGroup);
    vmpChk->setToolTip("-mllvm -irobf-vmp（需要 __attribute__((annotate(\"vmp\")))）");
    connect(vmpChk, &QCheckBox::toggled, this, [this, vmpChk](bool checked) {
        if (checked) {
            emit logMessage("[VMP] 需要在函数前添加注解: __attribute__((annotate(\"vmp\")))", "#00bfff");
        }
        onOptionChanged();
    });
    obfLayout->addWidget(vmpChk, row, col);
    m_passChecks.append({vmpChk, nullptr, "irobf-vmp"});

    col++;
    QScrollArea *scrollObf = new QScrollArea(this);
    scrollObf->setWidget(obfGroup);
    scrollObf->setWidgetResizable(true);
    scrollObf->setStyleSheet("QScrollArea{border:none;}");
    mainLayout->addWidget(scrollObf);
    
    m_mkInfoText = new QTextEdit(this);
    m_mkInfoText->setVisible(false);
}

void MainTab::onSelectJniFolder() {
    QString path = QFileDialog::getExistingDirectory(this, "选择项目目录 (自动识别 jni)");
    if (path.isEmpty()) return;

    QString jniPath = path + "/jni";
    if (QDir(path).exists("jni") && QFileInfo::exists(jniPath + "/Android.mk")) {
        path = jniPath;
    }

    m_jniFolderEdit->setText(QDir::toNativeSeparators(path));
    emit logMessage("[识别] jni 路径: " + QDir::toNativeSeparators(path), "#00d4aa");
    emit logMessage("[识别] Android.mk: " + QDir::toNativeSeparators(path + "/Android.mk"), "#a0a0b0");
    emit jniFolderChanged(path);
}

void MainTab::onSelectNdkPath() {
    QString path = QFileDialog::getExistingDirectory(this, "选择 NDK 根目录");
    if (!path.isEmpty()) {
        m_ndkPathEdit->setText(QDir::toNativeSeparators(path));
    }
}

void MainTab::onSelectOutputFolder() {
    QString path = QFileDialog::getExistingDirectory(this, "选择输出文件夹");
    if (!path.isEmpty()) {
        m_outputFolderEdit->setText(QDir::toNativeSeparators(path));
    }
}

void MainTab::onRefreshMkInfo() {
    loadMkContent();
    emit logMessage("[刷新] Android.mk 已重新加载", "#00d4aa");
}

void MainTab::loadDefaultNdk() {
    QString appDir = QCoreApplication::applicationDirPath();
    QStringList candidates = {
        appDir + "/../../../android-ndk-r30-beta1-windows",
        appDir + "/../android-ndk-r30-beta1-windows",
        appDir + "/../../android-ndk-r30-beta1-windows",
    };

    QDir ndkDir(QCoreApplication::applicationDirPath());
    ndkDir.cdUp();
    ndkDir.cdUp();
    ndkDir.cdUp();
    QString rootPath = ndkDir.absolutePath();
    candidates.prepend(rootPath + "/android-ndk-r30-beta1-windows");

    for (const auto &path : candidates) {
        QString ndkBuildPath = path + "/ndk-build.cmd";
        if (QFileInfo::exists(ndkBuildPath)) {
            m_ndkPathEdit->setText(QDir::toNativeSeparators(path));
            emit logMessage("[自动检测] 已找到 NDK: " + ndkBuildPath, "#00aa66");
            return;
        }
    }

    m_ndkPathEdit->setPlaceholderText("未检测到 NDK，请手动选择路径...");
    emit logMessage("[警告] 未找到 NDK，请手动设置路径", "#cc6600");
}

void MainTab::onOptionChanged() {
    emit optionChanged();
}

void MainTab::loadMkContent() {
    QString jniPath = m_jniFolderEdit->text();
    if (jniPath.isEmpty()) {
        m_currentMkFile.clear();
        m_mkInfoText->blockSignals(true);
        m_mkInfoText->setPlainText("# 请选择 jni 文件夹");
        m_mkInfoText->blockSignals(false);
        emit mkContentChanged(m_mkInfoText->toPlainText());
        return;
    }

    QString mkFile = jniPath + "/Android.mk";
    if (!QFileInfo::exists(mkFile)) {
        m_currentMkFile.clear();
        m_mkInfoText->blockSignals(true);
        m_mkInfoText->setPlainText("# 未找到 " + mkFile);
        m_mkInfoText->blockSignals(false);
        emit mkContentChanged(m_mkInfoText->toPlainText());
        return;
    }

    QFile f(mkFile);
    if (!f.open(QIODevice::ReadOnly | QIODevice::Text)) {
        m_currentMkFile.clear();
        m_mkInfoText->blockSignals(true);
        m_mkInfoText->setPlainText("# 无法读取 Android.mk");
        m_mkInfoText->blockSignals(false);
        emit mkContentChanged(m_mkInfoText->toPlainText());
        return;
    }

    m_currentMkFile = mkFile;
    m_mkInfoText->blockSignals(true);
    m_mkInfoText->setPlainText(QString::fromUtf8(f.readAll()));
    m_mkInfoText->blockSignals(false);
    f.close();
    emit mkContentChanged(m_mkInfoText->toPlainText());
}

void MainTab::refreshMkInfo() {
    loadMkContent();
}

void MainTab::onMkTextChanged() {
    if (m_currentMkFile.isEmpty()) return;
    QFile f(m_currentMkFile);
    if (!f.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate)) return;
    f.write(m_mkInfoText->toPlainText().toUtf8());
    f.close();
}
