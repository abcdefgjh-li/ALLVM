#include "ConfigManager.h"
#include <QCoreApplication>

ConfigManager& ConfigManager::instance() {
    static ConfigManager inst;
    return inst;
}

QString ConfigManager::configPath() const {
    return QCoreApplication::applicationDirPath() + "/configs.ini";
}

void ConfigManager::saveConfig(const QString &jniFolder, const QString &ndkPath,
                                const QString &outputFolder, int optLevel,
                                const QList<PassCheckBox> &passChecks) {
    QSettings settings(configPath(), QSettings::IniFormat);
    
    settings.setValue("jniFolder", jniFolder);
    settings.setValue("ndkPath", ndkPath);
    settings.setValue("outputFolder", outputFolder);
    settings.setValue("optLevel", optLevel);
    
    settings.beginGroup("passes");
    for (const auto &pc : passChecks) {
        settings.setValue(pc.flag, pc.chk->isChecked());
        if (pc.levelCombo) {
            settings.setValue(pc.flag + "_level", pc.levelCombo->currentIndex());
        }
    }
    settings.endGroup();
}

void ConfigManager::loadConfig(QString &jniFolder, QString &ndkPath,
                                QString &outputFolder, int &optLevel,
                                QList<PassCheckBox> &passChecks) {
    QSettings settings(configPath(), QSettings::IniFormat);
    
    jniFolder = settings.value("jniFolder").toString();
    ndkPath = settings.value("ndkPath").toString();
    outputFolder = settings.value("outputFolder").toString();
    optLevel = settings.value("optLevel", 2).toInt();
    
    settings.beginGroup("passes");
    for (auto &pc : passChecks) {
        pc.chk->setChecked(settings.value(pc.flag).toBool());
        if (pc.levelCombo) {
            pc.levelCombo->setCurrentIndex(settings.value(pc.flag + "_level", 0).toInt());
        }
    }
    settings.endGroup();
}
