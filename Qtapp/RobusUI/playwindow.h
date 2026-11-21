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

private:
    Ui::PlayWindow *ui;
};

#endif // PLAYWINDOW_H
