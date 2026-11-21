#ifndef PLAYWINDOW_H
#define PLAYWINDOW_H

#include <QDialog>

namespace Ui {
class PlayWindow;
}

class PlayWindow : public QDialog
{
    Q_OBJECT

public:
    explicit PlayWindow(QWidget *parent = nullptr);
    ~PlayWindow();
    void loadBoats(const int grid[8][8], const int ids[8][8] = nullptr);

private:
    Ui::PlayWindow *ui;

    enum states {
        playerTurn,
        robusTurn,
        gameFinished
    };
    states state = playerTurn;
};

#endif // PLAYWINDOW_H
