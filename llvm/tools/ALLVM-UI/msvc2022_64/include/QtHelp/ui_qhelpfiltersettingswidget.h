/********************************************************************************
** Form generated from reading UI file 'qhelpfiltersettingswidget.ui'
**
** Created by: Qt User Interface Compiler version 6.10.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_QHELPFILTERSETTINGSWIDGET_H
#define UI_QHELPFILTERSETTINGSWIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QToolButton>
#include <QtWidgets/QWidget>
#include "qoptionswidget_p.h"

QT_BEGIN_NAMESPACE

class Ui_QHelpFilterSettingsWidget
{
public:
    QGridLayout *gridLayout;
    QLabel *filterLabel;
    QLabel *componentsLabel;
    QLabel *versionsLabel;
    QListWidget *filterWidget;
    QOptionsWidget *componentWidget;
    QOptionsWidget *versionWidget;
    QToolButton *addButton;
    QToolButton *renameButton;
    QToolButton *removeButton;

    void setupUi(QWidget *QHelpFilterSettingsWidget)
    {
        if (QHelpFilterSettingsWidget->objectName().isEmpty())
            QHelpFilterSettingsWidget->setObjectName("QHelpFilterSettingsWidget");
        QHelpFilterSettingsWidget->resize(347, 127);
        gridLayout = new QGridLayout(QHelpFilterSettingsWidget);
        gridLayout->setObjectName("gridLayout");
        filterLabel = new QLabel(QHelpFilterSettingsWidget);
        filterLabel->setObjectName("filterLabel");

        gridLayout->addWidget(filterLabel, 0, 0, 1, 3);

        componentsLabel = new QLabel(QHelpFilterSettingsWidget);
        componentsLabel->setObjectName("componentsLabel");
        componentsLabel->setFrameShape(QFrame::NoFrame);

        gridLayout->addWidget(componentsLabel, 0, 3, 1, 1);

        versionsLabel = new QLabel(QHelpFilterSettingsWidget);
        versionsLabel->setObjectName("versionsLabel");

        gridLayout->addWidget(versionsLabel, 0, 4, 1, 1);

        filterWidget = new QListWidget(QHelpFilterSettingsWidget);
        filterWidget->setObjectName("filterWidget");

        gridLayout->addWidget(filterWidget, 1, 0, 1, 3);

        componentWidget = new QOptionsWidget(QHelpFilterSettingsWidget);
        componentWidget->setObjectName("componentWidget");

        gridLayout->addWidget(componentWidget, 1, 3, 2, 1);

        versionWidget = new QOptionsWidget(QHelpFilterSettingsWidget);
        versionWidget->setObjectName("versionWidget");

        gridLayout->addWidget(versionWidget, 1, 4, 2, 1);

        addButton = new QToolButton(QHelpFilterSettingsWidget);
        addButton->setObjectName("addButton");

        gridLayout->addWidget(addButton, 2, 0, 1, 1);

        renameButton = new QToolButton(QHelpFilterSettingsWidget);
        renameButton->setObjectName("renameButton");

        gridLayout->addWidget(renameButton, 2, 1, 1, 1);

        removeButton = new QToolButton(QHelpFilterSettingsWidget);
        removeButton->setObjectName("removeButton");

        gridLayout->addWidget(removeButton, 2, 2, 1, 1);


        retranslateUi(QHelpFilterSettingsWidget);

        QMetaObject::connectSlotsByName(QHelpFilterSettingsWidget);
    } // setupUi

    void retranslateUi(QWidget *QHelpFilterSettingsWidget)
    {
        QHelpFilterSettingsWidget->setWindowTitle(QCoreApplication::translate("QHelpFilterSettingsWidget", "Form", nullptr));
        filterLabel->setText(QCoreApplication::translate("QHelpFilterSettingsWidget", "Filter", nullptr));
        componentsLabel->setText(QCoreApplication::translate("QHelpFilterSettingsWidget", "Components", nullptr));
        versionsLabel->setText(QCoreApplication::translate("QHelpFilterSettingsWidget", "Versions", nullptr));
        addButton->setText(QCoreApplication::translate("QHelpFilterSettingsWidget", "Add...", nullptr));
        renameButton->setText(QCoreApplication::translate("QHelpFilterSettingsWidget", "Rename...", nullptr));
        removeButton->setText(QCoreApplication::translate("QHelpFilterSettingsWidget", "Remove", nullptr));
    } // retranslateUi

};

namespace Ui {
    class QHelpFilterSettingsWidget: public Ui_QHelpFilterSettingsWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_QHELPFILTERSETTINGSWIDGET_H
