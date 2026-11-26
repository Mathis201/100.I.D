#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "DeviceHandler.h"
#include "playwindow.h"
#include <QMainWindow>
using namespace std;

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    bool rotation = false; //false pour horizontal et true pour vertical
    int grid[8][8] = {0}; //grille 8x8
    int boatNum = 0; //index du bateau a placer
    int boatLength[5] = {2,2,3,4,5};
    int m_nextBoatId = 1;
    QStringList algolevels = (QStringList()<<""<<"Facile"<<"Normal"<<"Difficile");
    bool IsModeChosen = false;
    bool apercuClicked = false;

private slots:
    void checkBoat();
    void on_Rotation_clicked();
    void on_Suivant_clicked();
    void printGrid();
    QString convertGrid(int grid[8][8]);

    void on_difAlgo_currentIndexChanged(int index);

private:
    Ui::MainWindow *ui;
    DeviceHandler *m_bleHandler = nullptr; // Instance du gestionnaire BLE
    PlayWindow *m_PlayWindow;
    QVector<QPoint> boatCells(int row, int col, int dRow, int dCol, int length) const;
    bool canPlace(const QVector<QPoint> &cells) const;

};
#endif // MAINWINDOW_H
