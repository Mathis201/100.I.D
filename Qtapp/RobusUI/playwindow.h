#ifndef PLAYWINDOW_H
#define PLAYWINDOW_H

#include <QDialog>

class DeviceHandler;

namespace Ui {
class PlayWindow;
}

class PlayWindow : public QDialog
{
    Q_OBJECT

public:
    explicit PlayWindow(DeviceHandler *ble, QWidget *parent = nullptr);
    ~PlayWindow();
    void loadBoats(const int grid[8][8]);

private slots:
    void on_attaquer_clicked();

private:
    Ui::PlayWindow *ui;

    enum States {
        playerTurn,
        robusTurn,
        gameFinished
    };
    States state = playerTurn;
    DeviceHandler *m_bleHandler = nullptr;
};

#endif // PLAYWINDOW_H
