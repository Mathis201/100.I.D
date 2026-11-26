#include "playwindow.h"
#include "ui_playwindow.h"
#include "mainwindow.h"
#include "BoardWidget.h"
#include <qregularexpression.h>
#include "DeviceHandler.h"
#include "../../Shared/Messaging/ble_setup.h"

PlayWindow::PlayWindow(DeviceHandler *ble, QWidget *parent)
    : QDialog(parent), ui(new Ui::PlayWindow), m_bleHandler(ble)
{
    ui->setupUi(this);
}

PlayWindow::~PlayWindow()
{
    delete ui;
}

void PlayWindow::loadBoats(const int grid[8][8])
{
    ui->myBoard->clear();

    for (int r = 0; r < 8; ++r)
        for (int c = 0; c < 8; ++c)
            if (grid[r][c] > 0)
                ui->myBoard->setShip(r, c, grid[r][c]);   // the value is the boat ID
}

void PlayWindow::setState(States s){
    //copie du .h
    /*enum states {
        playerTurn,
        robusTurn,
        gameFinished
    };
    states state = playerTurn;*/

    state = s;

    switch (state) {
    case playerTurn:
        qDebug() << "state playerTurn";
        ui->attaquer->setEnabled(true);
        ui->attaqueCoords->setEnabled(true);
        ui->turnIndicator->setText("À votre tour");
        break;
    case robusTurn:

        ui->myBoard->setCell(0, 0, BoardWidget::Hit);
        ui->myBoard->setCell(0, 7, BoardWidget::Miss);

        ui->attaquer->setEnabled(false);
        ui->attaqueCoords->setEnabled(false);
        ui->turnIndicator->setText("Tour du robot, attendez le résulat");
        qDebug() << "state robusTurn";
        break;
    case gameFinished:
        break;
    }
}

void PlayWindow::on_attaquer_clicked()
{
    if (!m_bleHandler) {
        qDebug() << "m_bleHandler is NULL";
        return;
    }
    QString coords = ui->attaqueCoords->toPlainText().trimmed();
    static const QRegularExpression coordRegex("^[a-hA-H][1-8]$");

    if (coordRegex.match(coords).hasMatch()) {
        ui->Invalide->setText("");
        m_bleHandler->writeValue(PlayerInfoService::UUID, PlayerInfoService::LastShotCoordsCharacteristic::UUID, coords.toUtf8()); //send player attack coords
        setState(robusTurn);
    }
    else {
        ui->Invalide->setText("Entrée invalide");
    }
}

void onBleValue(const QBluetoothUuid &uuid, const QByteArray &value){
    static const QBluetoothUuid robotCoords(QString::fromStdString(
        RobotInfoService::LastShotCoordsCharacteristic::UUID));
    static const QBluetoothUuid robotResult(QString::fromStdString(
        RobotInfoService::LastShotResultCharacteristic::UUID));
    static const QBluetoothUuid playerResult(QString::fromStdString(
        PlayerInfoService::LastShotResultCharacteristic::UUID));
    static const QBluetoothUuid gameOver(QString::fromStdString(
        GameInfoService::IsGameOverCharacteristic::UUID));
}

