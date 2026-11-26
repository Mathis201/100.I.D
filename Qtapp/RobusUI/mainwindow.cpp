#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "DeviceHandler.h"
#include <QRegularExpression>
#include "../../Shared/Messaging/ble_setup.h"
#include "BoardWidget.h"

using namespace std;

    MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    m_bleHandler = new DeviceHandler(this);
    m_bleHandler->startScan();
    connect(m_bleHandler, &DeviceHandler::ready, this, [this]() {
        m_bleHandler->subscribe(RobotInfoService::UUID,
                                RobotInfoService::LastShotCoordsCharacteristic::UUID);
        m_bleHandler->subscribe(GameInfoService::UUID,
                                GameInfoService::IsGameOverCharacteristic::UUID);
    });
    ui->difAlgo->addItems(algolevels);
    m_PlayWindow = new PlayWindow(m_bleHandler);

}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_Rotation_clicked()
{
    if (rotation){
        rotation = false;
        qDebug() <<"Rotation is false" ;
        ui->Rotation->setText("Rotation (H)");
    }
    else{
        rotation = true;
        qDebug() <<"Rotation is True" ;
        ui->Rotation->setText("Rotation (V)");
    }
}

void MainWindow::checkBoat(){
    if (!IsModeChosen){
        ui->Invalide->setText("Choisissez un mode de jeu en premier");
        return;
    }

    QString content = ui->Pos->toPlainText().trimmed();
    static const QRegularExpression coordRegex("^[a-hA-H][1-8]$");

    if (coordRegex.match(content).hasMatch()) {
        ui->Invalide->setText("");
        // Convert coordinate to indexes 0-7
        int row = content[0].toUpper().unicode() - 'A';
        int col = content[1].digitValue() - 1;

        int length = boatLength[boatNum];
        int dRow = rotation ? 1 : 0;   // vertical: moves down
        int dCol = rotation ? 0 : 1;   // horizontal: moves right


        if(!apercuClicked){
            QVector<QPoint> cells = boatCells(row, col, dRow, dCol, length);
            ui->myBoard->setPreview(cells, canPlace(cells));
            ui->Suivant->setText("Suivant");
            apercuClicked = true;
            return;
        }

        // Est-ce que le bateau reste dans la grid
        int endRow = row + dRow * (length - 1);
        int endCol = col + dCol * (length - 1);
        if (endRow > 7 || endCol > 7) {
            ui->Invalide->setText("Le bateau sort de la grille");
            return;
        }

        // Est-ce que le bateau tombe sur un autre
        for (int i = 0; i < length; i++) {
            if (grid[row + dRow * i][col + dCol * i]) {
                ui->Invalide->setText("Un bateau est déjà à cet endroit");
                return;
            }
        }

        const int boatId = m_nextBoatId++;

        // Place le bateau
        for (int i = 0; i < length; i++) {
            int r = row + dRow * i;
            int c = col + dCol * i;

            grid[r][c] = boatNum + 1;                          // your game state (unchanged)
            ui->myBoard->setShip(r, c, boatId); // display
            ui->label_3->setText("Placez le bateau #" + QString::number(boatNum + 2) + ", long de " + QString::number(boatLength[boatNum + 1]) + " cases");
        }

        apercuClicked = false;
        boatNum++; //on passe au prochain bateau
        printGrid();

    } else {
        ui->Invalide->setText("Entrée invalide");
    }
}

void MainWindow::on_Suivant_clicked()
{
    QVector<QPoint> cells = boatCells(0,0,0,0,0);
    ui->myBoard->setPreview(cells, canPlace(cells));
    checkBoat();
    if (boatNum >= 5){
        m_bleHandler->writeValue(PlayerInfoService::UUID, PlayerInfoService::GridCharacteristic::UUID, convertGrid(grid).toUtf8());
        ui->Invalide->setText("Tous les bateaux sont en place");
        m_PlayWindow->loadBoats(grid);
        this->hide();
        m_PlayWindow->show();
        return;
    }
    if(!apercuClicked){
        ui->Pos->clear();
    }

}

void MainWindow::printGrid()
{
    QString out = "\n  A B C D E F G H\n";
    for (int row = 0; row < 8; row++) {
        out += QString::number(row + 1) + " ";
        for (int col = 0; col < 8; col++) {
            if (grid[row][col] == 0)
                out += ". ";
            else
                out += QString::number(grid[row][col]) + " ";
        }
        out += "\n";
    }
    qDebug().noquote() << out;
    qDebug().noquote() << convertGrid(grid);
}

QString MainWindow::convertGrid(int grid[8][8]){

    QString sgrid = "";

    for(int i = 0; i < 8; i++){
        for (int j=0; j < 8; j++){
            sgrid += to_string(grid[i][j]);
        }
    }
    return sgrid;
}



void MainWindow::on_difAlgo_currentIndexChanged(int index)
{
    if (index != 0){
        m_bleHandler->writeValue(GameInfoService::UUID, GameInfoService::AlgorithmStrengthCharacteristic::UUID, QString::number(index - 1).toUtf8()); //0 facile, 1 normal, 2 difficile
        IsModeChosen = true;
    }
    else{
        IsModeChosen = false;
    }
}

QVector<QPoint> MainWindow::boatCells(int row, int col, int dRow, int dCol, int length) const
{
    QVector<QPoint> cells;
    for (int i = 0; i < length; ++i)
        cells << QPoint(col + dCol * i, row + dRow * i);   // QPoint is (col, row)
    return cells;
}

bool MainWindow::canPlace(const QVector<QPoint> &cells) const
{
    for (const QPoint &p : cells) {
        if (p.x() < 0 || p.x() >= 8 || p.y() < 0 || p.y() >= 8)
            return false;                  // out of the board
        if (grid[p.y()][p.x()] != 0)
            return false;                  // overlaps another boat
    }
    return true;
}


