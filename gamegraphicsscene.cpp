#include "gamegraphicsscene.h"
#include <QGraphicsSceneMouseEvent>
#include <QDebug>

gameGraphicsScene::gameGraphicsScene(QObject *parent)
    :QGraphicsScene(parent)
{
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
            squares[i] = new QSquare((i%board_side),(i/board_side),Square::Black,Qt::black);
            this->addItem(squares[i]);
            if(i%board_side != board_side-1){
                black = false;
            }
        }else{
            squares[i] = new QSquare((i%board_side),(i/board_side),Square::Empty,Qt::white);
            this->addItem(squares[i]);
            if(i%board_side != board_side-1){
                black = true;
            }
        }
    }

    cap += board_side*(board_side-2*rows);
    for(;i < cap;++i){
        if(black){
            squares[i] = new QSquare((i%board_side),(i/board_side),Square::Empty,Qt::black);
            this->addItem(squares[i]);
            if(i%board_side != board_side-1){
                black = false;
            }
        }else{
            squares[i] = new QSquare((i%board_side),(i/board_side),Square::Empty,Qt::white);
            this->addItem(squares[i]);
            if(i%board_side != board_side-1){
                black = true;
            }
        }
    }
    cap +=  board_side*rows;

    for(;i < cap;++i){
        if(black){
            squares[i] = new QSquare((i%board_side),(i/board_side),Square::White,Qt::black);
            this->addItem(squares[i]);
            if(i%board_side != board_side-1){
                black = false;
            }
        }else{
            squares[i] = new QSquare((i%board_side),(i/board_side),Square::Empty,Qt::white);
            this->addItem(squares[i]);
            if(i%board_side != board_side-1){
                black = true;
            }
        }
    }

    game = gameLogic(squares,board_side);
    highlight_squares();
}

gameGraphicsScene::~gameGraphicsScene()
{
    for(size_t i = 0;i < board_side*board_side;++i){
        delete squares[i];
    }
}

void gameGraphicsScene::mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    QGraphicsItem *item = itemAt(mouseEvent->scenePos(), QTransform());
    QSquare* thisItem = dynamic_cast<QSquare*>(item);
    if(lastItem == nullptr){
        assert(this->selectedItems().length() <= 1);
        if(thisItem->show_piece() == Square::Empty){
            clear_inputs();
        }else{
            lastItem = thisItem;
            delete_highlights(thisItem);
            //highlight squares of possible moves
            highlight_moves(thisItem);
            if(move_vec.size() == 0){
                clear_inputs();
            }
        }
    }
    else{
        if(move_vec.size() > 0){
            if(thisItem->is_highlighted()){
                ++move_it;
                for(gameLogic::Moves::iterator move = move_vec.begin();move != move_vec.end();){
                    if(thisItem->correct_board_cor((*move).first[move_it])){
                        Square::Piece piece = lastItem->show_piece();
                        if(move_it < move->second.size()){
                            Square::Piece piece2 = squares[game.square_pos(move->second[move_it])]->update_piece(Square::Empty);
                            pieces_stack.push_back(std::make_pair(move->second[move_it],piece2));
                        }
                        if(move_it >= (*move).first.size()-1){
                            return_piece_stack();
                            game.playerInput((*move));
                            move_it = 0;
                            clear_inputs();
                            break;
                        }else{
                            lastItem->update_piece(Square::Empty);
                            thisItem->update_piece(piece);
                            lastItem = thisItem;
                            delete_highlights();
                            highlight_moves(thisItem);
                            break;
                        }
                    }else{
                        move = move_vec.erase(move);
                    }
                }
            }else{
                clear_inputs();
            }
        }else{
            clear_inputs();
        }
    }
    QGraphicsScene::mousePressEvent(mouseEvent);
    QGraphicsScene::update();
}

void gameGraphicsScene::clear_inputs()
{
    this->selectedItems().clear();
    lastItem = nullptr;
    return_piece_stack();
    delete_highlights();
    highlight_squares();
    return;
}

void gameGraphicsScene::return_piece_stack()
{
    for(auto p: pieces_stack){
        squares[game.square_pos(p.first)]->update_piece(p.second);
    }
    pieces_stack.clear();
    return;
}

void gameGraphicsScene::highlight_moves(QSquare* thisItem)
{
    for(auto move: game.possible_moves){
        if(thisItem->correct_board_cor(move.first[move_it])){
            assert(move_it < move.first.size());
            squares[game.square_pos(move.first[move_it+1])]->add_highlight();
            move_vec.push_back(move);
        }
    }
    return;
}

void gameGraphicsScene::highlight_squares()
{
    for(auto move: game.possible_moves){
        squares[game.square_pos(move.first[0])]->add_highlight();
    }
}

void gameGraphicsScene::delete_highlights(QSquare* except_thisItem)
{
    for(size_t i=0;i<board_side*board_side;++i){
        if(except_thisItem == nullptr || !(except_thisItem->correct_board_cor(game.board_pos(i)))){
            squares[i]->delete_highlight();
        }
    }
    move_vec.clear();
    return;
}
