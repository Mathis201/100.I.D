/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.9.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPlainTextEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QLabel *label;
    QLabel *label_2;
    QPushButton *TestLED;
    QPlainTextEdit *Pos;
    QLabel *label_3;
    QPushButton *Rotation;
    QPushButton *Suivant;
    QLabel *Invalide;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(728, 541);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        label = new QLabel(centralwidget);
        label->setObjectName("label");
        label->setGeometry(QRect(20, 30, 331, 31));
        label_2 = new QLabel(centralwidget);
        label_2->setObjectName("label_2");
        label_2->setGeometry(QRect(20, 60, 341, 21));
        TestLED = new QPushButton(centralwidget);
        TestLED->setObjectName("TestLED");
        TestLED->setGeometry(QRect(620, 40, 80, 18));
        Pos = new QPlainTextEdit(centralwidget);
        Pos->setObjectName("Pos");
        Pos->setGeometry(QRect(200, 80, 101, 31));
        label_3 = new QLabel(centralwidget);
        label_3->setObjectName("label_3");
        label_3->setGeometry(QRect(20, 80, 201, 31));
        Rotation = new QPushButton(centralwidget);
        Rotation->setObjectName("Rotation");
        Rotation->setGeometry(QRect(340, 90, 80, 18));
        Suivant = new QPushButton(centralwidget);
        Suivant->setObjectName("Suivant");
        Suivant->setGeometry(QRect(430, 90, 80, 18));
        Invalide = new QLabel(centralwidget);
        Invalide->setObjectName("Invalide");
        Invalide->setGeometry(QRect(200, 120, 101, 16));
        MainWindow->setCentralWidget(centralwidget);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName("statusbar");
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        label->setText(QCoreApplication::translate("MainWindow", "Bienvenue dans l'interface utilisateur du battleship 100.I.D", nullptr));
        label_2->setText(QCoreApplication::translate("MainWindow", "Positions des Bateaux :", nullptr));
        TestLED->setText(QCoreApplication::translate("MainWindow", "Test Led", nullptr));
        Pos->setPlainText(QString());
        label_3->setText(QCoreApplication::translate("MainWindow", "Position sur le plateau, bateau #1", nullptr));
        Rotation->setText(QCoreApplication::translate("MainWindow", "Rotation", nullptr));
        Suivant->setText(QCoreApplication::translate("MainWindow", "Suivant", nullptr));
        Invalide->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
