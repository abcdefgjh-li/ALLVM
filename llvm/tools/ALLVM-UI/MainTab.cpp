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
#include <QFrame>
#include <QToolButton>
#include <QPropertyAnimation>
#include <QEasingCurve>
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

bool MainTab::isDarkMode() const {
    return m_darkMode;
}

void MainTab::setDarkMode(bool darkMode) {
    m_darkMode = darkMode;
    applyTheme();
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

void MainTab::applyTheme() {
    if (m_titleLabel) {
        m_titleLabel->setStyleSheet(QString("QLabel{font-size:24px;font-weight:700;color:%1;margin-left:12px;}")
                                        .arg(m_darkMode ? "#f8fafc" : "#0f172a"));
    }

    for (auto *label : m_primaryLabels) {
        if (label) {
            label->setStyleSheet(QString("QLabel{font-size:18px;font-weight:700;color:%1;}")
                                     .arg(m_darkMode ? "#f8fafc" : "#0f172a"));
        }
    }

    for (auto *label : m_secondaryLabels) {
        if (label) {
            label->setStyleSheet(QString("QLabel{font-size:12px;color:%1;}")
                                     .arg(m_darkMode ? "#94a3b8" : "#64748b"));
        }
    }

    for (auto *frame : m_cardFrames) {
        if (frame) {
            frame->setStyleSheet(QString("QFrame{background:%1;border:1px solid %2;border-radius:18px;}")
                                     .arg(m_darkMode ? "#111827" : "#ffffff",
                                          m_darkMode ? "#334155" : "#d7deea"));
        }
    }

    for (auto *button : m_sectionButtons) {
        if (button) {
            button->setStyleSheet(QString(
                "QToolButton{border:none;text-align:left;padding:16px 18px;font-size:16px;font-weight:700;color:%1;}"
                "QToolButton:hover{background:%2;border-radius:18px;}")
                                      .arg(m_darkMode ? "#f8fafc" : "#0f172a",
                                           m_darkMode ? "#1e293b" : "#f8fafc"));
        }
    }

    for (auto *frame : m_passItemFrames) {
        if (frame) {
            frame->setStyleSheet(QString("QFrame{background:%1;border:1px solid %2;border-radius:14px;}")
                                     .arg(m_darkMode ? "#0f172a" : "#f8fafc",
                                          m_darkMode ? "#334155" : "#e2e8f0"));
        }
    }

    for (const auto &pc : m_passChecks) {
        if (pc.chk) {
            pc.chk->setStyleSheet(QString(
                "QCheckBox{font-size:13px;color:%1;font-weight:500;spacing:8px;}"
                "QCheckBox::indicator{width:18px;height:18px;border:1px solid %2;border-radius:6px;background:%3;}"
                "QCheckBox::indicator:checked{background:#2563eb;border-color:#2563eb;}")
                                      .arg(m_darkMode ? "#e5eefb" : "#1f2937",
                                           m_darkMode ? "#475569" : "#cbd5e1",
                                           m_darkMode ? "#111827" : "#ffffff"));
        }
        if (pc.levelCombo) {
            pc.levelCombo->setStyleSheet(QString(
                "QComboBox{background:%1;color:%2;border:1px solid %3;border-radius:8px;padding:6px 10px;font-size:12px;font-weight:600;}")
                                             .arg(m_darkMode ? "#0f172a" : "#ffffff",
                                                  m_darkMode ? "#f8fafc" : "#1e293b",
                                                  m_darkMode ? "#475569" : "#cbd5e1"));
        }
    }
}

void MainTab::setupUI() {
    auto *mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(12);
    mainLayout->setContentsMargins(0, 0, 0, 0);

    auto *scrollArea = new QScrollArea(this);
    scrollArea->setWidgetResizable(true);
    scrollArea->setFrameShape(QFrame::NoFrame);
    scrollArea->setStyleSheet("QScrollArea{border:none;background:transparent;}QScrollArea > QWidget > QWidget{background:transparent;}");
    mainLayout->addWidget(scrollArea);

    auto *page = new QWidget(scrollArea);
    auto *pageLayout = new QVBoxLayout(page);
    pageLayout->setSpacing(12);
    pageLayout->setContentsMargins(2, 2, 2, 2);
    scrollArea->setWidget(page);

    m_titleLabel = new QLabel("混淆配置", page);
    pageLayout->addWidget(m_titleLabel);

    auto createCard = [&](const QString &title, const QString &subtitle) {
        auto *card = new QFrame(page);
        card->setFrameShape(QFrame::NoFrame);
        m_cardFrames.append(card);
        auto *layout = new QVBoxLayout(card);
        layout->setSpacing(10);
        layout->setContentsMargins(18, 16, 18, 16);

        auto *titleText = new QLabel(title, card);
        m_primaryLabels.append(titleText);
        layout->addWidget(titleText);

        if (!subtitle.isEmpty()) {
            auto *subtitleText = new QLabel(subtitle, card);
            subtitleText->setWordWrap(true);
            m_secondaryLabels.append(subtitleText);
            layout->addWidget(subtitleText);
        }
        return qMakePair(card, layout);
    };

    auto settingsCard = createCard("项目设置", "先选择 jni、NDK 和输出目录，再在下方展开混淆功能分组。");
    auto *settingsLayout = new QGridLayout();
    settingsLayout->setHorizontalSpacing(12);
    settingsLayout->setVerticalSpacing(12);
    settingsLayout->setColumnStretch(1, 1);
    settingsCard.second->addLayout(settingsLayout);

    auto styleActionButton = [](QPushButton *button, const QString &background, const QString &hover) {
        button->setFixedHeight(36);
        button->setStyleSheet(
            "QPushButton{background:" + background + ";color:#ffffff;border:none;border-radius:10px;padding:0 14px;font-weight:600;}"
            "QPushButton:hover{background:" + hover + ";}"
            "QPushButton:pressed{padding-top:1px;}");
    };

    settingsLayout->addWidget(new QLabel("jni 文件夹", settingsCard.first), 0, 0);
    auto *jniLayout = new QHBoxLayout();
    jniLayout->setSpacing(8);
    m_jniFolderEdit = new QLineEdit(settingsCard.first);
    m_jniFolderEdit->setPlaceholderText("jni 目录（可选项目根目录自动识别）");
    connect(m_jniFolderEdit, &QLineEdit::textChanged, this, &MainTab::loadMkContent);
    jniLayout->addWidget(m_jniFolderEdit, 1);
    auto *btnJniFolder = new QPushButton("选择目录", settingsCard.first);
    btnJniFolder->setMinimumWidth(96);
    styleActionButton(btnJniFolder, "#2563eb", "#1d4ed8");
    connect(btnJniFolder, &QPushButton::clicked, this, &MainTab::onSelectJniFolder);
    jniLayout->addWidget(btnJniFolder);
    m_btnRefreshMk = new QPushButton("刷新 MK", settingsCard.first);
    m_btnRefreshMk->setMinimumWidth(96);
    styleActionButton(m_btnRefreshMk, "#0ea5e9", "#0284c7");
    connect(m_btnRefreshMk, &QPushButton::clicked, this, &MainTab::onRefreshMkInfo);
    jniLayout->addWidget(m_btnRefreshMk);
    settingsLayout->addLayout(jniLayout, 0, 1);

    settingsLayout->addWidget(new QLabel("NDK 路径", settingsCard.first), 1, 0);
    auto *ndkLayout = new QHBoxLayout();
    ndkLayout->setSpacing(8);
    m_ndkPathEdit = new QLineEdit(settingsCard.first);
    m_ndkPathEdit->setPlaceholderText("NDK 根目录");
    ndkLayout->addWidget(m_ndkPathEdit, 1);
    m_btnDefaultNdk = new QPushButton("自动检测", settingsCard.first);
    m_btnDefaultNdk->setMinimumWidth(96);
    styleActionButton(m_btnDefaultNdk, "#22c55e", "#16a34a");
    connect(m_btnDefaultNdk, &QPushButton::clicked, this, &MainTab::loadDefaultNdk);
    ndkLayout->addWidget(m_btnDefaultNdk);
    auto *btnBrowseNdk = new QPushButton("选择目录", settingsCard.first);
    btnBrowseNdk->setMinimumWidth(96);
    styleActionButton(btnBrowseNdk, "#2563eb", "#1d4ed8");
    connect(btnBrowseNdk, &QPushButton::clicked, this, &MainTab::onSelectNdkPath);
    ndkLayout->addWidget(btnBrowseNdk);
    settingsLayout->addLayout(ndkLayout, 1, 1);

    settingsLayout->addWidget(new QLabel("输出文件夹", settingsCard.first), 2, 0);
    auto *outLayout = new QHBoxLayout();
    outLayout->setSpacing(8);
    m_outputFolderEdit = new QLineEdit(settingsCard.first);
    m_outputFolderEdit->setPlaceholderText("编译产物输出目录");
    outLayout->addWidget(m_outputFolderEdit, 1);
    auto *btnOutputFolder = new QPushButton("选择目录", settingsCard.first);
    btnOutputFolder->setMinimumWidth(96);
    styleActionButton(btnOutputFolder, "#2563eb", "#1d4ed8");
    connect(btnOutputFolder, &QPushButton::clicked, this, &MainTab::onSelectOutputFolder);
    outLayout->addWidget(btnOutputFolder);
    settingsLayout->addLayout(outLayout, 2, 1);

    settingsLayout->addWidget(new QLabel("优化等级", settingsCard.first), 3, 0);
    m_cmbOptLevel = new QComboBox(settingsCard.first);
    m_cmbOptLevel->addItems({"O0", "O1", "O2", "Os", "Oz", "O3"});
    m_cmbOptLevel->setCurrentIndex(2);
    m_cmbOptLevel->setMinimumWidth(120);
    settingsLayout->addWidget(m_cmbOptLevel, 3, 1, 1, 1, Qt::AlignLeft);
    pageLayout->addWidget(settingsCard.first);

    struct PassInfo {
        QString label;
        QString flag;
        QString category;
        QString tooltip;
    };
    QList<PassInfo> passes = {
        {"控制流平坦化", "irobf-fla", "代码混淆", "-mllvm -irobf-fla"},
        {"间接分支", "irobf-indbr", "代码混淆", "-mllvm -irobf-indbr"},
        {"间接调用", "irobf-icall", "代码混淆", "-mllvm -irobf-icall"},
        {"全局变量间接化", "irobf-indgv", "代码混淆", "-mllvm -irobf-indgv"},
        {"字符串加密", "irobf-cse", "代码混淆", "-mllvm -irobf-cse"},
        {"常量整数加密", "irobf-cie", "常量保护", "-mllvm -irobf-cie"},
        {"常量浮点加密", "irobf-cfe", "常量保护", "-mllvm -irobf-cfe"},
        {"RTTI 擦除", "irobf-rtti", "环境检测与反调试", "-mllvm -irobf-rtti"},
        {"LD_PRELOAD 检测", "irobf-ldpreload", "环境检测与反调试", "-mllvm -irobf-ldpreload"},
        {"虚拟机检测", "irobf-vmdetect", "环境检测与反调试", "-mllvm -irobf-vmdetect"},
        {"USB 调试保护", "irobf-usb", "环境检测与反调试", "-mllvm -irobf-usb"},
        {"调试器检测", "irobf-ida", "环境检测与反调试", "-mllvm -irobf-ida"},
        {"VPN 检测", "irobf-vpn", "环境检测与反调试", "-mllvm -irobf-vpn"},
        {"代理/iptables 检测", "irobf-proxy", "环境检测与反调试", "-mllvm -irobf-proxy"},
        {"时间差检测", "irobf-time", "环境检测与反调试", "-mllvm -irobf-time"},
        {"Hosts 文件检测", "irobf-hosts", "环境检测与反调试", "-mllvm -irobf-hosts"},
        {"Root 检测", "irobf-root", "环境检测与反调试", "-mllvm -irobf-root"},
        {"非 Root 检测", "irobf-noroot", "环境检测与反调试", "-mllvm -irobf-noroot"},
        {"系统调用保护", "irobf-syscall", "运行时保护与壳", "-mllvm -irobf-syscall"},
        {"内存 Dump 保护", "irobf-bandump", "运行时保护与壳", "-mllvm -irobf-bandump"},
        {"隐藏 Maps 保护", "irobf-hidemaps", "运行时保护与壳", "-mllvm -irobf-hidemaps"},
        {"伪造 Maps 内容", "irobf-fakemaps", "运行时保护与壳", "-mllvm -irobf-fakemaps"},
        {"ELF 加壳", "firobf-linker", "运行时保护与壳", "-firobf-linker（ChaCha20 加密 + 环境变量校验）"},
        {"GZ 压缩壳", "firobf-gz", "运行时保护与壳", "-firobf-gz（base64 包装壳，可与 ELF 加壳同时启用）"},
        {"VMP 虚拟机保护", "irobf-vmp", "高级能力", "-mllvm -irobf-vmp（支持注解或 vm_functions）"},
        {"调试日志", "irobf-debug", "高级能力", "-mllvm -irobf-debug"}
    };

    QSet<QString> levelPasses = {
        "irobf-fla", "irobf-indbr", "irobf-icall", "irobf-indgv",
        "irobf-cie", "irobf-cfe"
    };

    struct SectionWidgets {
        QString name;
        QFrame *card;
        QToolButton *toggle;
        QWidget *content;
        QGridLayout *grid;
        int row;
        int col;
    };

    QList<SectionWidgets> sections;
    auto createSection = [&](const QString &title, const QString &subtitle, bool expanded) -> SectionWidgets {
        auto *card = new QFrame(page);
        card->setFrameShape(QFrame::NoFrame);
        m_cardFrames.append(card);

        auto *cardLayout = new QVBoxLayout(card);
        cardLayout->setSpacing(0);
        cardLayout->setContentsMargins(0, 0, 0, 0);

        auto *toggle = new QToolButton(card);
        toggle->setCheckable(true);
        toggle->setChecked(expanded);
        toggle->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
        toggle->setArrowType(expanded ? Qt::DownArrow : Qt::RightArrow);
        m_sectionButtons.append(toggle);
        toggle->setText(title);
        cardLayout->addWidget(toggle);

        auto *subtitleLabel = new QLabel(subtitle, card);
        subtitleLabel->setWordWrap(true);
        subtitleLabel->setContentsMargins(18, 0, 18, 12);
        m_secondaryLabels.append(subtitleLabel);
        cardLayout->addWidget(subtitleLabel);

        auto *content = new QWidget(card);
        content->setStyleSheet("QWidget{background:transparent;border:none;}");
        auto *grid = new QGridLayout(content);
        grid->setContentsMargins(18, 0, 18, 18);
        grid->setHorizontalSpacing(12);
        grid->setVerticalSpacing(10);
        grid->setColumnStretch(0, 1);
        grid->setColumnStretch(1, 1);
        content->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
        cardLayout->addWidget(content);

        auto *animation = new QPropertyAnimation(content, "maximumHeight", card);
        animation->setDuration(120);
        animation->setEasingCurve(QEasingCurve::OutQuad);

        content->setMaximumHeight(expanded ? QWIDGETSIZE_MAX : 0);
        content->setVisible(expanded);

        connect(toggle, &QToolButton::toggled, this, [toggle, content, animation](bool checked) {
            toggle->setArrowType(checked ? Qt::DownArrow : Qt::RightArrow);
            content->layout()->activate();
            const int targetHeight = content->layout()->sizeHint().height();
            animation->stop();
            if (checked) {
                content->setVisible(true);
                animation->setStartValue(content->maximumHeight());
                animation->setEndValue(targetHeight);
                animation->start();
            } else {
                animation->setStartValue(content->maximumHeight());
                animation->setEndValue(0);
                animation->start();
                connect(animation, &QPropertyAnimation::finished, content, [content]() {
                    if (content->maximumHeight() == 0) {
                        content->setVisible(false);
                    }
                }, Qt::SingleShotConnection);
            }
        });

        pageLayout->addWidget(card);
        return {title, card, toggle, content, grid, 0, 0};
    };

    sections.append(createSection("代码混淆", "控制流、调用与字符串相关的主要混淆能力。", false));
    sections.append(createSection("常量保护", "给数值常量增加更多保护层。", false));
    sections.append(createSection("环境检测与反调试", "用于检测 Root、代理、调试器、虚拟机等风险环境。", false));
    sections.append(createSection("运行时保护与壳", "运行时保护、ELF/GZ 外壳与系统调用保护。", false));
    sections.append(createSection("高级能力", "高强度保护和辅助调试功能。", false));

    auto findSection = [&](const QString &name) -> SectionWidgets* {
        for (auto &section : sections) {
            if (section.name == name) {
                return &section;
            }
        }
        return nullptr;
    };

    auto addPassWidget = [&](SectionWidgets *section, const PassInfo &pass) {
        if (!section) {
            return;
        }

        auto *item = new QFrame(section->content);
        item->setFrameShape(QFrame::NoFrame);
        m_passItemFrames.append(item);
        auto *itemLayout = new QHBoxLayout(item);
        itemLayout->setContentsMargins(12, 10, 12, 10);
        itemLayout->setSpacing(8);

        auto *checkBox = new QCheckBox(pass.label, item);
        checkBox->setToolTip(pass.tooltip);
        itemLayout->addWidget(checkBox, 1);
        connect(checkBox, &QCheckBox::toggled, this, &MainTab::onOptionChanged);

        QComboBox *levelCombo = nullptr;
        if (levelPasses.contains(pass.flag)) {
            levelCombo = new QComboBox(item);
            levelCombo->addItems({"L1", "L2", "L3"});
            levelCombo->setCurrentIndex(0);
            levelCombo->setMinimumWidth(64);
            itemLayout->addWidget(levelCombo);
        }

        if (pass.flag == "irobf-vmp") {
            connect(checkBox, &QCheckBox::toggled, this, [this](bool checked) {
                if (checked) {
                    emit logMessage("[VMP] 使用注解: __attribute__((annotate(\"vmp\")))", "#2563eb");
                    emit logMessage("[VMP] 或命令行: -mllvm -irobf-vm_functions=func1;func2", "#2563eb");
                }
                onOptionChanged();
            });
        }

        section->grid->addWidget(item, section->row, section->col);
        m_passChecks.append({checkBox, levelCombo, pass.flag});

        section->col++;
        if (section->col >= 2) {
            section->col = 0;
            section->row++;
        }
    };

    for (const auto &pass : passes) {
        addPassWidget(findSection(pass.category), pass);
    }

    QCheckBox *rootChk = nullptr;
    QCheckBox *noRootChk = nullptr;
    for (const auto &pc : m_passChecks) {
        if (pc.flag == "irobf-root") rootChk = pc.chk;
        if (pc.flag == "irobf-noroot") noRootChk = pc.chk;
    }
    if (rootChk && noRootChk) {
        connect(rootChk, &QCheckBox::toggled, this, [noRootChk](bool checked) {
            if (checked) {
                noRootChk->blockSignals(true);
                noRootChk->setChecked(false);
                noRootChk->blockSignals(false);
            }
        });
        connect(noRootChk, &QCheckBox::toggled, this, [rootChk](bool checked) {
            if (checked) {
                rootChk->blockSignals(true);
                rootChk->setChecked(false);
                rootChk->blockSignals(false);
            }
        });
    }

    pageLayout->addStretch();

    m_mkInfoText = new QTextEdit(this);
    m_mkInfoText->setVisible(false);
    applyTheme();
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

    // 搜索顺序：先当前目录本身，再当前目录的子目录，最后上级目录
    QStringList candidates = {
        appDir,  // 当前目录本身（allvm-ui.exe 可能就在 ndk-build 同目录）
        appDir + "/android-ndk-r30-beta1-windows",  // 当前目录的子目录
        appDir + "/../android-ndk-r30-beta1-windows", // 上级目录
        appDir + "/../../android-ndk-r30-beta1-windows",
        appDir + "/../../../android-ndk-r30-beta1-windows",
    };

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
