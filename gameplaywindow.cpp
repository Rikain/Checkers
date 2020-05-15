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

/*
void gameplayWindow::HandleSelection()
{
    //i guess i have ot change it to a overridden QGrphicsScene with mousepressevent
    if(start){
        qDebug() << "siema";
        Square* thisItem = dynamic_cast<Square*>(scene->selectedItems().back());
        if(lastItem == nullptr){
            if(scene->selectedItems().length() > 1){
                qDebug() << "Error more than 1 sleected item at once! " + QString::number(scene->selectedItems().length());
            }
            else{
                if(thisItem->show_piece() == Square::Empty){
                    scene->selectedItems().clear();
                    //clear selectedItems somehow;
                }else{
                    lastItem = thisItem;

                    //make it so possible moves for this piece are highlited (if any else treat it as Empty Square)
                }

            }
        }else if(lastItem == thisItem){
            //do nothing
            qDebug() << "eluwina";
        }
        else{
            if(scene->selectedItems().length() > 2){
                qDebug() << "Error more than 2 sleected item at once! " + QString::number(scene->selectedItems().length());
            }
            Square::Piece piece = lastItem->update_piece(Square::Empty);
            thisItem->update_piece(piece);
            scene->selectedItems().clear();
            qDebug() << scene->selectedItems().length();
            lastItem = nullptr;
        }
    }
}
*/

void gameplayWindow::new_board()
{
    delete_board();

    qDebug() << "s1";

    scene = new gameGraphicsScene(this);

    qDebug() << "s2";

    ui->graphicsView->setScene(scene);
}

void gameplayWindow::delete_board()
{
    if(scene != nullptr){
        delete scene;
        scene = nullptr;
    }
}
