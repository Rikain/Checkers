#ifndef GAMEPLAYWINDOW_H
#define GAMEPLAYWINDOW_H

#include <QWidget>

namespace Ui {
class gameplayWindow;
}

class gameplayWindow : public QWidget
{
    Q_OBJECT

public:
    explicit gameplayWindow(QWidget *parent = nullptr);
    ~gameplayWindow();

private:
    Ui::gameplayWindow *ui;
};

#endif // GAMEPLAYWINDOW_H
