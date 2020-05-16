#ifndef GAMEPLAYWINDOW_H
#define GAMEPLAYWINDOW_H

#include <QWidget>
#include "gamegraphicsscene.h"

namespace Ui {
class gameplayWindow;
}

class gameplayWindow : public QWidget
{
    Q_OBJECT

public:
    explicit gameplayWindow(QWidget *parent = nullptr);
    ~gameplayWindow();
public:
    void start_a_new_game();
private slots:
    void on_button_rollback_clicked();

private:
    Ui::gameplayWindow *ui;
    gameGraphicsScene *scene = nullptr;
    void new_board();
    void delete_board();
};

#endif // GAMEPLAYWINDOW_H
