/********************************************************************************
** Form generated from reading UI file 'qfilternamedialog.ui'
**
** Created by: Qt User Interface Compiler version 6.10.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_QFILTERNAMEDIALOG_H
#define UI_QFILTERNAMEDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QAbstractButton>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QSpacerItem>

QT_BEGIN_NAMESPACE

class Ui_FilterNameDialogClass
{
public:
    QGridLayout *gridLayout;
    QLabel *label;
    QLineEdit *lineEdit;
    QSpacerItem *verticalSpacer;
    QDialogButtonBox *buttonBox;

    void setupUi(QDialog *FilterNameDialogClass)
    {
        if (FilterNameDialogClass->objectName().isEmpty())
            FilterNameDialogClass->setObjectName("FilterNameDialogClass");
        FilterNameDialogClass->resize(312, 77);
        gridLayout = new QGridLayout(FilterNameDialogClass);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName("gridLayout");
        label = new QLabel(FilterNameDialogClass);
        label->setObjectName("label");

        gridLayout->addWidget(label, 0, 0, 1, 1);

        lineEdit = new QLineEdit(FilterNameDialogClass);
        lineEdit->setObjectName("lineEdit");

        gridLayout->addWidget(lineEdit, 0, 1, 1, 1);

        verticalSpacer = new QSpacerItem(20, 1, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        gridLayout->addItem(verticalSpacer, 1, 0, 1, 1);

        buttonBox = new QDialogButtonBox(FilterNameDialogClass);
        buttonBox->setObjectName("buttonBox");
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

        gridLayout->addWidget(buttonBox, 2, 0, 1, 2);


        retranslateUi(FilterNameDialogClass);

        QMetaObject::connectSlotsByName(FilterNameDialogClass);
    } // setupUi

    void retranslateUi(QDialog *FilterNameDialogClass)
    {
        FilterNameDialogClass->setWindowTitle(QCoreApplication::translate("FilterNameDialogClass", "Add Filter", nullptr));
        label->setText(QCoreApplication::translate("FilterNameDialogClass", "Filter Name:", nullptr));
    } // retranslateUi

};

namespace Ui {
    class FilterNameDialogClass: public Ui_FilterNameDialogClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_QFILTERNAMEDIALOG_H
