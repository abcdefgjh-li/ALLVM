#ifndef HELPDOCUMENT_H
#define HELPDOCUMENT_H

#include <QDialog>
#include <QTextEdit>
#include <QString>

class HelpDocument : public QDialog {
    Q_OBJECT

public:
    explicit HelpDocument(QWidget *parent = nullptr, bool darkMode = false);

private:
    QString generateHelpContent();
    QString mdToHtml(const QString &md);
};

#endif
