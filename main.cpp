#include <QApplication>
//#include "mainwindow.h"
#include "botfight.h"
#include <iostream>

void console_run(){
    //std::cout << "alphabeta:\n";
    //botFight fight1(true,6,AI::Mix,true,6,AI::Mix);
    //fight1.print_a_fight(10);
    std::cout << "minmax:\n";
    botFight fight(false,6,AI::Mix,false,6,AI::Mix);
    fight.print_a_fight(1);
    return;
}

int main(int argc, char *argv[])
{
    console_run();
    return 0;
    //QApplication a(argc, argv);
    //MainWindow main;
    //main.show();
    //return a.exec();
}
