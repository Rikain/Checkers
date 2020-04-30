#ifndef APPLICATION_H
#define APPLICATION_H

#include <QWidget>

namespace Ui {
class Application;
}

class Application : public QWidget
{
    Q_OBJECT

public:
    explicit Application(QWidget *parent = nullptr);
    ~Application();
private slots:
    void on_newGameButton_clicked();
    void on_quitButton_clicked();

public:
    void new_game();
    void open_menu();
signals:
    void hide_toolbar();
    void show_toolbar();
    void exit();

private:
    Ui::Application *ui;
};

#endif // APPLICATION_H
