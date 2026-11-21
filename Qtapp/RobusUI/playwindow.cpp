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

    //copie du .h
    /*enum states {
        playerTurn,
        robusTurn,
        gameFinished
    };
    states state = playerTurn;*/

    switch (state) {
    case playerTurn:

        break;
    case robusTurn:

        break;
    case gameFinished:
        break;
    }
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
        m_bleHandler->writeValue(PlayerInfoService::UUID, PlayerInfoService::LastShotCoordsCharacteristic::UUID, coords.toUtf8());
        state = robusTurn;
    }
    else {
        ui->Invalide->setText("Entrée invalide");
    }
}

