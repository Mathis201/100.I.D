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
    QString content = ui->Pos->toPlainText().trimmed();
    static const QRegularExpression coordRegex("^[a-hA-H][1-8]$");

    if (coordRegex.match(content).hasMatch()) {
        ui->Invalide->setText("");

        // Convert coordinate to indexes 0-7
        int col = content[0].toUpper().unicode() - 'A';
        int row = content[1].digitValue() - 1;

        int length = boatLength[boatNum];
        int dRow = rotation ? 1 : 0;   // vertical: moves down
        int dCol = rotation ? 0 : 1;   // horizontal: moves right

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

        // PLace le bateau
        for (int i = 0; i < length; i++) {
            grid[row + dRow * i][col + dCol * i] = length;
            ui->label_3->setText("Position sur le plateau, bateau #" + QString::number(length));
        }

        boatNum++; //on passe au prochain bateau
        printGrid();

    } else {
        ui->Invalide->setText("Entrée invalide");
    }
}

void MainWindow::on_Suivant_clicked()
{
    checkBoat();
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
}
