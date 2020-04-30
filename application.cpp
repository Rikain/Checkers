#include "application.h"
#include "ui_application.h"

Application::Application(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Application)
{
    ui->setupUi(this);
}

Application::~Application()
{
    delete ui;
}

void Application::new_game()
{
    ui->stackedWidget->setCurrentIndex(1);
    emit show_toolbar();
    ui->GameplayWindow->start_a_new_game();
}

void Application::open_menu()
{
    ui->stackedWidget->setCurrentIndex(0);
    emit hide_toolbar();
}

void Application::on_newGameButton_clicked()
{
    new_game();
}

void Application::on_quitButton_clicked()
{
    emit exit();
}
