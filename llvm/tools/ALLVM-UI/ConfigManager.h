#ifndef CONFIGMANAGER_H
#define CONFIGMANAGER_H

#include <QString>
#include <QSettings>
#include <QList>
#include <QCheckBox>
#include <QComboBox>

struct PassCheckBox {
    QCheckBox *chk;
    QComboBox *levelCombo;
    QString flag;
};

class ConfigManager {
public:
    static ConfigManager& instance();
    
    void saveConfig(const QString &jniFolder, const QString &ndkPath,
                    const QString &outputFolder, int optLevel,
                    const QList<PassCheckBox> &passChecks);
    
    void loadConfig(QString &jniFolder, QString &ndkPath,
                    QString &outputFolder, int &optLevel,
                    QList<PassCheckBox> &passChecks);
    
    QString configPath() const;

private:
    ConfigManager() = default;
    ConfigManager(const ConfigManager&) = delete;
    ConfigManager& operator=(const ConfigManager&) = delete;
};

#endif
