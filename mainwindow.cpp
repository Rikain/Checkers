#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "application.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->centralwidget,&Application::hide_toolbar,this,&MainWindow::hide_toolbar);
    connect(ui->centralwidget,&Application::show_toolbar,this,&MainWindow::show_toolbar);
    connect(ui->centralwidget,&Application::exit,this,&MainWindow::exit);

    //connect(ui->centralwidget,SIGNAL(hide_toolbar()),this,SLOT(hide_toolbar()));
    //connect(ui->centralwidget,SIGNAL(show_toolbar()),this,SLOT(show_toolbar()));
    //connect(ui->centralwidget,SIGNAL(exit()),this,SLOT(exit()));
    hide_toolbar();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::hide_toolbar()
{
    ui->toolBar->hide();
}

void MainWindow::show_toolbar()
{
    ui->toolBar->show();
}

void MainWindow::on_action_new_game_triggered()
{
    ui->centralwidget->new_game();
}

void MainWindow::exit()
{
    emit close();
}

void MainWindow::on_actionexit_triggered()
{
    exit();
}

void MainWindow::on_actionactionMenu_triggered()
{
    ui->centralwidget->open_menu();
}
