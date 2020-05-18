#include <QApplication>
//#include "mainwindow.h"
#include "botfight.h"

void console_run(){
    botFight fight(false,1,false,5);
    fight.print_a_fight();
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
