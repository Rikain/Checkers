#include "gamegraphicsscene.h"
#include <QGraphicsSceneMouseEvent>
#include <QDebug>
#include <QThread>

gameGraphicsScene::gameGraphicsScene(int AI_height, QObject *parent)
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
            if(i == cap-2){
                squares[i] = new QSquare((i%board_side),(i/board_side),Square::BlackKing,Qt::black);
            }else{
                squares[i] = new QSquare((i%board_side),(i/board_side),Square::Black,Qt::black);
            }

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

    game = new gameLogic(squares,board_side);
    highlight_squares();
    if(AI_height != 0){
        assert(AI_height < 11);
        computer_player = new GTSAlgortihms(AI_height,true,AI::PiecesOnly,game);
        std::uniform_int_distribution<> dist(0,1);
        human_player = dist(computer_player->gen);
        if(!human_player){
            computer_move();
        }
    }
}

gameGraphicsScene::~gameGraphicsScene()
{
    for(size_t i = 0;i < board_side*board_side;++i){
        delete squares[i];
    }
    if(computer_player != nullptr){
        delete computer_player;
    }
    delete game;
}

void gameGraphicsScene::mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    if(!check_if_ended()){
        QGraphicsItem *item = itemAt(mouseEvent->scenePos(), QTransform());
        QSquare* thisItem = dynamic_cast<QSquare*>(item);
        if(lastItem == nullptr){
            assert(this->selectedItems().length() <= 1);
            if(thisItem->show_piece() == Square::Empty){
                clear_inputs();
            }else{
                new_first_click(thisItem);
            }
        }
        else{
            if(move_vec.size() > 0){
                if(thisItem->is_highlighted() && (thisItem->show_piece() == Square::Empty)){
                    ++move_it;
                    for(gameLogic::Moves::iterator move = move_vec.begin();move != move_vec.end();){
                        if(thisItem->correct_board_cor((*move).first[move_it])){
                            Square::Piece piece = lastItem->show_piece();
                            if(move_it == 1){
                                Square::Piece piece2 = squares[(*game).square_pos(move->second[0].first)]->update_piece(Square::Empty);
                                pieces_stack.push_back(std::make_pair(move->second[0].first,piece2));
                            }
                            if(move_it < move->second.size()){
                                Square::Piece piece2 = squares[(*game).square_pos(move->second[move_it].first)]->update_piece(Square::Empty);
                                pieces_stack.push_back(std::make_pair(move->second[move_it].first,piece2));
                            }
                            if(move_it >= (*move).first.size()-1){
                                squares[(*game).square_pos(move->first[0])]->update_piece(piece);
                                return_piece_stack();
                                int move_state = game->moves_towards_draw();
                                if((*game).playerInput((*move))){
                                    played_moves.emplace(std::make_pair(*move,move_state));
                                }
                                clear_inputs();
                                if(computer_player != nullptr && !check_if_ended()){
                                    computer_move();
                                }
                                if(check_if_ended()){
                                    end_the_game();
                                }
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
                }else if(thisItem->show_piece() != Square::Empty){
                    new_first_click(thisItem);
                }else{
                    clear_inputs();
                }
            }else{
                clear_inputs();
            }
        }
    }
    QGraphicsScene::mousePressEvent(mouseEvent);
    QGraphicsScene::update();
}

void gameGraphicsScene::rollback_last_move()
{
    if(!played_moves.empty() && !(played_moves.size() == 1 && !human_player)){
        clear_inputs();
        (*game).rollback_move(played_moves.top().first,played_moves.top().second);
        played_moves.pop();
        delete_highlights();
        if((computer_player != nullptr) && !((game->player_color() == human_player))){ // if human player won rollback only once
            (*game).rollback_move(played_moves.top().first,played_moves.top().second);
            played_moves.pop();
        }
        highlight_squares();
        QGraphicsScene::update();
    }
}

void gameGraphicsScene::new_first_click(QSquare *thisItem)
{
    bool found = false;
    for(auto move: (*game).possible_moves){
        if(thisItem->correct_board_cor(move.first[0])){
            found = true;
            break;
        }
    }
    if(found){
        delete_highlights(thisItem);
        highlight_moves(thisItem);
        if(move_vec.size() == 0){
            clear_inputs();
        }
        lastItem = thisItem;
        thisItem->add_highlight();
    }else{
        clear_inputs();
    }
}

void gameGraphicsScene::clear_inputs()
{
    move_it = 0;
    return_piece_stack();
    delete_highlights();
    highlight_squares();
    lastItem = nullptr;
    return;
}

void gameGraphicsScene::return_piece_stack()
{
    if(lastItem != nullptr && !pieces_stack.empty()){
        lastItem->update_piece(Square::Empty);
    }
    for(auto p: pieces_stack){
        squares[(*game).square_pos(p.first)]->update_piece(p.second);
    }
    pieces_stack.clear();
    return;
}

void gameGraphicsScene::highlight_moves(QSquare* thisItem)
{
    for(auto move: (*game).possible_moves){
        if(thisItem->correct_board_cor(move.first[move_it])){
            assert(move_it < move.first.size());
            squares[(*game).square_pos(move.first[move_it+1])]->add_highlight();
            move_vec.push_back(move);
        }
    }
    return;
}

void gameGraphicsScene::highlight_squares()
{
    for(auto move: (*game).possible_moves){
        squares[(*game).square_pos(move.first[0])]->add_highlight();
    }
}

void gameGraphicsScene::delete_highlights(QSquare* except_thisItem)
{
    for(size_t i=0;i<board_side*board_side;++i){
        if(except_thisItem == nullptr || !(except_thisItem->correct_board_cor((*game).board_pos(i)))){
            squares[i]->delete_highlight();
        }
    }
    move_vec.clear();
    return;
}

bool gameGraphicsScene::check_if_ended()
{
    return(game->game_state() != gameLogic::inProgress);
}

void gameGraphicsScene::end_the_game()
{
    assert(check_if_ended());
    delete_highlights();
}

void gameGraphicsScene::computer_move()
{
    gameLogic::Move computer_move = computer_player->return_a_move();
    int move_state = game->moves_towards_draw();
    if(game->playerInput(computer_move)){
        played_moves.emplace(std::make_pair(computer_move,move_state));
    }
    clear_inputs();
    QGraphicsScene::update();
}
