#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QRegularExpression>
using namespace std;

    MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->TestLED->setCheckable(true);
    m_bleHandler = new DeviceHandler(this);

    m_bleHandler->startScan();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked(bool checked)
{
    // Transmet la valeur du bouton (true/false) à l'ESP32
    if (m_bleHandler) {
        m_bleHandler->sendButtonClicked(checked);
    }
}

void MainWindow::placeBoats(int boatNum, bool grid, int boatLengh[])
{
    int boatl = boatLengh[boatNum]
}

void MainWindow::CheckBoat(bool grid, int Boatnum)
{

}

void MainWindow::on_Rotation_clicked()
{
    if (rotation){
        rotation = false;
        qDebug() <<"Rotation is false" ;
    }
    else{
        rotation = true;
        qDebug() <<"Rotation is True" ;
    }

}


void MainWindow::on_Suivant_clicked()
{
    QString content = ui->Pos->toPlainText();
    static const QRegularExpression coordRegex("^[a-ha-hA-H][1-8]$");

    if (coordRegex.match(content).hasMatch()) {
        ui->Invalide->setText("");
        PlaceBoats(Boatnum);


        boatNum += 1;
    } else {
        ui->Invalide->setText("Entrée invalide");
    }
}

