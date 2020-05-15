#include "gamelogic.h"
#include "assert.h"
#include <stack>
#include <algorithm>

bool gameLogic::switch_player()
{
    white_player = !white_player;
    return white_player;
}

gameLogic::gameLogic(Square **squares_ptr, size_t array_size, size_t board_side_in)
    :squares(squares_ptr)
{
    assert(array_size == squares_size);
    assert(board_side_in == board_side);
    assert(array_size == board_side_in*board_side_in);
    for(size_t i =0;i < array_size ;++i){
        switch (squares_ptr[i]->show_piece()) {
            case(Square::White):
                whiteMen.push_back(board_pos(i));
                break;
            case(Square::Black):
                blackMen.push_back(board_pos(i));
                break;
            case(Square::WhiteKing):
                whiteKings.push_back(board_pos(i));
                break;
            case(Square::BlackKing):
                blackKings.push_back(board_pos(i));
                break;
            default:
                break;
        }
    }
    update_moves();
}

gameLogic::gameLogic(QSquare **squares_ptr, size_t board_side_in)
{
    squares = new Square*[board_side_in*board_side_in];

    for(size_t i = 0;i < board_side_in*board_side_in;++i){
        squares[i] = squares_ptr[i];
    }

    for(size_t i =0;i < board_side_in*board_side_in ;++i){
        switch (squares_ptr[i]->show_piece()) {
            case(Square::White):
                whiteMen.push_back(board_pos(i));
                break;
            case(Square::Black):
                blackMen.push_back(board_pos(i));
                break;
            case(Square::WhiteKing):
                whiteKings.push_back(board_pos(i));
                break;
            case(Square::BlackKing):
                blackKings.push_back(board_pos(i));
                break;
            default:
                break;
        }
    }

    update_moves();
}

gameLogic::gameLogic(size_t board_side_in)
{
    assert(board_side == board_side_in);
    squares = nullptr;
    squares_size = board_side_in*board_side_in;
    bool black = false;
    size_t j = ((board_side_in-2)/2) * board_side_in;
    size_t j2 = squares_size - (((board_side_in-2)/2) * board_side_in);
    for(size_t i =0;i < squares_size ;++i){
        if(black){
            if(i < j){
                blackMen.push_back(board_pos(i));
            }else if(i > j2){
                whiteMen.push_back(board_pos(i));
            }
        }
        if(i%(board_side_in) != (board_side_in-1)){
            black = !black;
        }
    }
}

void gameLogic::update_moves()
{
    gameLogic::Moves moves;
    std::vector<gameLogic::Coordinates>* men;
    std::vector<gameLogic::Coordinates>* kings;

    if(white_player){
        men = &whiteMen;
        kings = &whiteKings;
    }else{
        men = &blackMen;
        kings = &blackKings;
    }

    //start with kings' moves as they are propably longer
    size_t longest_move = 0;
    bool taken = false;

    for(gameLogic::Coordinates p : *kings){
        moves_of_a_piece(p,longest_move,moves,taken,true);
    }

    for(gameLogic::Coordinates p : *men){
        moves_of_a_piece(p,longest_move,moves,taken,false);
    }

    if(longest_move == 0){
        if(white_player){
            state = blackWon;
        }else{
            state = whiteWon;
        }
    }

    possible_moves = moves;
}

void gameLogic::playerInput(const gameLogic::Move &player_move)
{
    Square::Piece played_piece = squares[square_pos(player_move.first.front())]->update_piece(Square::Empty);
    remove_piece(player_move.first.front(),played_piece);
    for(auto it = player_move.second.begin()+1;it != player_move.second.end();++it){
        Square::Piece piece = squares[square_pos(*it)]->update_piece(Square::Empty);
        remove_piece(*it,piece);
    }
    add_piece(player_move.first.back(),played_piece);
    switch_player();
    update_moves();
    return;
}

gameLogic::GameState gameLogic::game_state() const
{
    return state;
}

gameLogic::Coordinates gameLogic::go_in_direction(gameLogic::Moves &moves,Move &current_move,Coordinates &current_suqare,std::function<gameLogic::Coordinates(gameLogic::Coordinates)> &next_square_func , std::function<bool(Coordinates)> &boudires_condition,bool player_move_condition, bool& move_ended, bool enemy_piece, size_t& longest_move, bool white_player, bool &taken, bool king)
{

    Coordinates next_square = next_square_func(current_suqare);
    bool in_bounds = boudires_condition(next_square);
    if(!in_bounds){
        //left the board
        if(enemy_piece){
            move_ended = true;
        }else{
            add_move(moves,current_move,longest_move,taken,enemy_piece);
            move_ended = true;
        }
        return Coordinates();
    }else{
        Square::Piece next_piece = squares[square_pos(next_square)]->show_piece();
        if(next_piece == Square::Empty || (std::find(current_move.second.begin(),current_move.second.end(),next_square) != current_move.second.end())){
            if(!enemy_piece){
                if(current_move.second.size() == 1 && player_move_condition){
                    current_move.first.push_back(next_square);
                    add_move(moves,current_move,longest_move,taken,enemy_piece);
                    move_ended = true;
                    current_move.first.pop_back();
                    return next_square;
                }else{
                    move_ended = true;
                    return Coordinates();
                }
            }else{
                current_move.first.push_back(next_square);
                current_move.second.emplace_back(current_suqare);
                add_move(moves,current_move,longest_move,taken,enemy_piece);
                enemy_piece = false;
                return next_square;
            }
        }else if(next_piece == Square::White || next_piece == Square::WhiteKing){
            if(white_player){
                move_ended = true;
                return Coordinates();
            }else{
                if(enemy_piece){
                    move_ended = true;
                    return Coordinates();
                }else{
                    enemy_piece = true;
                    return go_in_direction(moves,current_move,next_square,next_square_func,boudires_condition,player_move_condition,move_ended,enemy_piece,longest_move,white_player,taken,king); //(musisz pojsc jeszcze raz w ten sam direction gdy jest pierwszy raz enemy piece)
                }
            }
        }else{
            if(white_player){
                if(enemy_piece){
                    move_ended = true;
                    return Coordinates();
                }else{
                    enemy_piece = true;
                    return go_in_direction(moves,current_move,next_square,next_square_func,boudires_condition,player_move_condition,move_ended,enemy_piece,longest_move,white_player,taken,king); //(musisz pojsc jeszcze raz w ten sam direction gdy jest pierwszy raz enemy piece)
                }
            }else{
                move_ended = true;
                return Coordinates();
            }
        }
    }
}

void gameLogic::add_move(gameLogic::Moves &moves, gameLogic::Move &current_move, size_t &longest_move, bool &taken, bool enemy_piece)
{
    if(current_move.first.size() == longest_move){
        if(longest_move <= 2){
            if(taken){
                if(enemy_piece){
                     moves.push_back(current_move);
                }
            }else{
                if(enemy_piece){
                    reset_add_move(moves,current_move,longest_move);
                    taken = true;
                }else{
                    moves.push_back(current_move);
                }
            }
        }else{
            moves.push_back(current_move);
        }
    }else if(current_move.first.size() > longest_move){
        reset_add_move(moves,current_move,longest_move);
    }
    return;
}

void gameLogic::moves_of_a_piece(gameLogic::Coordinates &piece,size_t &longest_move,gameLogic::Moves &moves, bool &taken,bool king)
{
    static std::function<bool(gameLogic::Coordinates)> br_condition = [](Coordinates c){return (c.first < 10 && c.second < 10);};
    static std::function<bool(gameLogic::Coordinates)> bl_condition = [](Coordinates c){return (c.first >= 0 && c.second < 10);};
    static std::function<bool(gameLogic::Coordinates)> tr_condition = [](Coordinates c){return (c.first < 10 && c.second >= 0);};
    static std::function<bool(gameLogic::Coordinates)> tl_condition = [](Coordinates c){return (c.first >= 0 && c.second >= 0);};

    static std::function<gameLogic::Coordinates(gameLogic::Coordinates)> br_square = [](gameLogic::Coordinates c){return std::make_pair(c.first + 1,c.second+1);};
    static std::function<gameLogic::Coordinates(gameLogic::Coordinates)> bl_square = [](gameLogic::Coordinates c){return std::make_pair(c.first - 1,c.second+1);};
    static std::function<gameLogic::Coordinates(gameLogic::Coordinates)> tr_square = [](gameLogic::Coordinates c){return std::make_pair(c.first + 1,c.second-1);};
    static std::function<gameLogic::Coordinates(gameLogic::Coordinates)> tl_square = [](gameLogic::Coordinates c){return std::make_pair(c.first - 1,c.second-1);};

    std::stack<std::array<bool,4>> directions_to_check;
    Coordinates current_suqare = piece;
    directions_to_check.push(std::array<bool,4>{true,true,true,true});
    Move current_move;
    current_move.first.push_back(piece);
    current_move.second.push_back(piece);

     //direction order is: BR,BL,TR,TL

    while(!directions_to_check.empty()){
        bool move_ended = false;
        bool enemy_piece = false;
        if(directions_to_check.top()[0]){

            bool player_move_condition = !white_player;

            gameLogic::Coordinates temp_square = go_in_direction(moves,current_move,current_suqare,br_square,br_condition,player_move_condition,move_ended,enemy_piece,longest_move,white_player,taken,king);

            directions_to_check.top()[0] = false;

            if(!move_ended){
                directions_to_check.push(std::array<bool,4>{true,true,true,true});
                current_suqare = temp_square;
            }
        }else if(directions_to_check.top()[1]){

            bool player_move_condition = !white_player;

            gameLogic::Coordinates temp_square = go_in_direction(moves,current_move,current_suqare,bl_square,bl_condition,player_move_condition,move_ended,enemy_piece,longest_move,white_player,taken,king);

            directions_to_check.top()[1] = false;

            if(!move_ended){
                directions_to_check.push(std::array<bool,4>{true,true,true,true});
                current_suqare = temp_square;
            }

        }else if(directions_to_check.top()[2]){

            bool player_move_condition = white_player;

            gameLogic::Coordinates temp_square = go_in_direction(moves,current_move,current_suqare,tr_square,tr_condition,player_move_condition,move_ended,enemy_piece,longest_move,white_player,taken,king);

            directions_to_check.top()[2] = false;

            if(!move_ended){
                directions_to_check.push(std::array<bool,4>{true,true,true,true});
                current_suqare = temp_square;
            }

        }else if(directions_to_check.top()[3]){

            bool player_move_condition = white_player;

            gameLogic::Coordinates temp_square = go_in_direction(moves,current_move,current_suqare,tl_square,tl_condition,player_move_condition,move_ended,enemy_piece,longest_move,white_player,taken,king);

            directions_to_check.top()[3] = false;

            if(!move_ended){
                directions_to_check.push(std::array<bool,4>{true,true,true,true});
                current_suqare = temp_square;
            }

        }else{
            directions_to_check.pop();
            current_move.first.pop_back();
            current_suqare = current_move.first.back();
            current_move.second.pop_back();
            if(current_move.first.size() == 1){
                assert(current_move.second.size() == 1);
            }
        }
    }
}

void gameLogic::reset_add_move(gameLogic::Moves &moves, gameLogic::Move &current_move, size_t &longest_move)
{
    moves.clear();
    moves.push_back(current_move);
    longest_move = current_move.first.size();
    return;
}

int gameLogic::square_pos(gameLogic::Coordinates board_pos)
{
    return board_pos.second * board_side + board_pos.first;
}

void gameLogic::remove_piece(gameLogic::Coordinates cor, Square::Piece &piece)
{
    switch(piece){
    case Square::White:
        whiteMen.erase(std::remove(whiteMen.begin(), whiteMen.end(), cor), whiteMen.end());
        break;
    case Square::Black:
         blackMen.erase(std::remove(blackMen.begin(), blackMen.end(), cor), blackMen.end());
        break;
    case Square::WhiteKing:
        whiteKings.erase(std::remove(whiteKings.begin(), whiteKings.end(), cor), whiteKings.end());
        break;
    case Square::BlackKing:
        blackKings.erase(std::remove(blackKings.begin(), blackKings.end(), cor), blackKings.end());
        break;
    default:
        break;
    }
}

void gameLogic::add_piece(gameLogic::Coordinates cor, Square::Piece &piece)
{
   switch(piece){
   case Square::White:
       if(cor.second == 0){
            piece = Square::WhiteKing;
       }
       break;
   case Square::Black:
       if(static_cast<size_t>(cor.second) == (board_side-1)){
            piece = Square::BlackKing;
       }
   default:
       break;
   }
    squares[square_pos(cor)]->update_piece(piece);
    switch(piece){
    case Square::White:
        whiteMen.push_back(cor);
        break;
    case Square::Black:
         blackMen.push_back(cor);
        break;
    case Square::WhiteKing:
        whiteKings.push_back(cor);
        break;
    case Square::BlackKing:
        blackKings.push_back(cor);
        break;
    default:
        assert(true);
        break;
    }
}

gameLogic::Coordinates gameLogic::board_pos(size_t square_pos)
{
    return std::make_pair(square_pos%board_side,square_pos/board_side);
}

bool gameLogic::compere_cordinates(const gameLogic::Coordinates c1, const gameLogic::Coordinates c2)
{
    return (c1.first == c2.first && c1.second == c2.second);
}

gameLogic::~gameLogic()
{

}
