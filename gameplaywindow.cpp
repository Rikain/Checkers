#include "gameplaywindow.h"
#include "ui_gameplaywindow.h"
#include <QDebug>
#include "assert.h"

gameplayWindow::gameplayWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::gameplayWindow)
{
    ui->setupUi(this);
}

gameplayWindow::~gameplayWindow()
{
    delete_board();
    delete ui;
}

void gameplayWindow::start_a_new_game()
{
    new_board();
}

void gameplayWindow::new_board()
{
    delete_board();

    scene = new gameGraphicsScene(7,this);

    ui->graphicsView->setScene(scene);
}

void gameplayWindow::delete_board()
{
    if(scene != nullptr){
        delete scene;
        scene = nullptr;
    }
}

void gameplayWindow::on_button_rollback_clicked()
{
    scene->rollback_last_move();
}
