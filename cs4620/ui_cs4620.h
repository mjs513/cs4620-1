/********************************************************************************
** Form generated from reading UI file 'cs4620.ui'
**
** Created: Sat Sep 25 20:59:16 2010
**      by: Qt User Interface Compiler version 4.7.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CS4620_H
#define UI_CS4620_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_cs4620Class
{
public:

    void setupUi(QWidget *cs4620Class)
    {
        if (cs4620Class->objectName().isEmpty())
            cs4620Class->setObjectName(QString::fromUtf8("cs4620Class"));
        cs4620Class->resize(400, 300);

        retranslateUi(cs4620Class);

        QMetaObject::connectSlotsByName(cs4620Class);
    } // setupUi

    void retranslateUi(QWidget *cs4620Class)
    {
        cs4620Class->setWindowTitle(QApplication::translate("cs4620Class", "cs4620", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class cs4620Class: public Ui_cs4620Class {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CS4620_H
