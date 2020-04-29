#include "gameplaywindow.h"
#include "ui_gameplaywindow.h"

gameplayWindow::gameplayWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::gameplayWindow)
{
    ui->setupUi(this);
}

gameplayWindow::~gameplayWindow()
{
    delete ui;
}
