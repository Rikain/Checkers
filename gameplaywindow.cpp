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
    for(size_t i = 0;i < board_side*board_side;++i){
        delete squares[i];
    }
    delete scene;
}

void gameplayWindow::start_a_new_game()
{
    make_new_board();
}

void gameplayWindow::make_new_board()
{
    for(size_t i = 0;i < board_side*board_side;++i){
        delete squares[i];
    }
    delete scene;

    scene = new QGraphicsScene(this);

    bool black = false;

    int rows;
    if(board_side == 8){
        rows = 3;
    }else if(board_side == 10){
        rows = 4;
    }

    size_t i = 0;

    size_t cap = board_side*rows;

    for(;i < cap;++i){
        if(black){
            squares[i] = new Square((i%board_side),(i/board_side),Qt::black,Square::Black);
            scene->addItem(squares[i]);
            if(i%board_side != board_side-1){
                black = false;
            }
        }else{
            squares[i] = new Square((i%board_side),(i/board_side),Qt::white,Square::Empty);
            scene->addItem(squares[i]);
            if(i%board_side != board_side-1){
                black = true;
            }
        }
    }

    cap += board_side*(board_side-2*rows);
    for(;i < cap;++i){
        if(black){
            squares[i] = new Square((i%board_side),(i/board_side),Qt::black,Square::Empty);
            scene->addItem(squares[i]);
            if(i%board_side != board_side-1){
                black = false;
            }
        }else{
            squares[i] = new Square((i%board_side),(i/board_side),Qt::white,Square::Empty);
            scene->addItem(squares[i]);
            if(i%board_side != board_side-1){
                black = true;
            }
        }
    }
    cap +=  board_side*rows;

    for(;i < cap;++i){
        if(black){
            squares[i] = new Square((i%board_side),(i/board_side),Qt::black,Square::White);
            scene->addItem(squares[i]);
            if(i%board_side != board_side-1){
                black = false;
            }
        }else{
            squares[i] = new Square((i%board_side),(i/board_side),Qt::white,Square::Empty);
            scene->addItem(squares[i]);
            if(i%board_side != board_side-1){
                black = true;
            }
        }
    }
    ui->graphicsView->setScene(scene);
}
