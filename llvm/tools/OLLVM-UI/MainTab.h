#ifndef MAINTAB_H
#define MAINTAB_H

#include <QWidget>
#include <QLineEdit>
#include <QTextEdit>
#include <QComboBox>
#include <QPushButton>
#include <QCheckBox>
#include <QTimer>
#include <QList>
#include <QLabel>
#include <QFrame>
#include <QToolButton>
#include "ConfigManager.h"

class MainTab : public QWidget {
    Q_OBJECT

public:
    explicit MainTab(QWidget *parent = nullptr);
    
    QString jniFolder() const;
    QString ndkPath() const;
    QString outputFolder() const;
    int optLevel() const;
    QList<PassCheckBox> passChecks() const;
    QString currentMkFile() const;
    QTextEdit* mkInfoText();
    void setDarkMode(bool darkMode);
    bool isDarkMode() const;
    
    void setJniFolder(const QString &path);
    void setNdkPath(const QString &path);
    void setOutputFolder(const QString &path);
    void setOptLevel(int level);
    void updatePassChecks(const QList<PassCheckBox> &checks);
    
    void loadMkContent();
    void refreshMkInfo();
    void loadDefaultNdk();

signals:
    void jniFolderChanged(const QString &path);
    void optionChanged();
    void injectFlagsRequested();
    void buildRequested();
    void collectOutputRequested();
    void helpRequested();
    void cleanRequested();
    void logMessage(const QString &text, const QString &color);
    void mkContentChanged(const QString &content);

private slots:
    void onSelectJniFolder();
    void onSelectNdkPath();
    void onSelectOutputFolder();
    void onRefreshMkInfo();
    void onOptionChanged();
    void onMkTextChanged();

private:
    void applyTheme();
    void setupUI();
    
    QLineEdit *m_jniFolderEdit;
    QLineEdit *m_ndkPathEdit;
    QLineEdit *m_outputFolderEdit;
    QTextEdit *m_mkInfoText;
    QComboBox *m_cmbOptLevel;
    
    QPushButton *m_btnRefreshMk;
    QPushButton *m_btnDefaultNdk;
    
    QList<PassCheckBox> m_passChecks;
    QTimer *m_mkSaveTimer;
    QString m_currentMkFile;
    bool m_darkMode = false;
    QLabel *m_titleLabel = nullptr;
    QList<QLabel *> m_primaryLabels;
    QList<QLabel *> m_secondaryLabels;
    QList<QFrame *> m_cardFrames;
    QList<QFrame *> m_passItemFrames;
    QList<QToolButton *> m_sectionButtons;
};

#endif
