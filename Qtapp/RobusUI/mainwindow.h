#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "DeviceHandler.h"
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
    array<array<bool,8>,8> grid= {0}; //grille 8x8
    int boatNum = 0; //index du bateau a placer
    int boatLengh[5] = {2,3,4,4,5};


private slots:
    void on_pushButton_clicked(bool checked);
    void placeBoats(int boatNum, bool grid, int boatLengh[], array<int,2> coord);
    void on_Rotation_clicked();

    void on_Suivant_clicked();

    void setRotation(bool r);
    array<int,2> convertCoordinate(string coord);

private:
    Ui::MainWindow *ui;
    DeviceHandler *m_bleHandler = nullptr; // Instance du gestionnaire BLE

};
#endif // MAINWINDOW_H
