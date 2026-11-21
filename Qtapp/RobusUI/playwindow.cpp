#include "playwindow.h"
#include "ui_playwindow.h"
#include "mainwindow.h"
#include "BoardWidget.h"

PlayWindow::PlayWindow(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::PlayWindow)
{
    ui->setupUi(this);
    ui->myBoard->setShip(1, 1, 1);

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

>>>>>>> Stashed changes
}

PlayWindow::~PlayWindow()
{
    delete ui;
}

// GameWindow.cpp
void PlayWindow::loadBoats(const int grid[8][8], const int ids[8][8])
{
    ui->myBoard->clear();

    for (int r = 0; r < 8; ++r)
        for (int c = 0; c < 8; ++c)
            if (grid[r][c] > 0)
                ui->myBoard->setShip(r, c, grid[r][c]);   // the value is the boat ID
}
