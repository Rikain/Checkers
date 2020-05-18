#include "gamelogic.h"
#include "assert.h"
#include <algorithm>

bool gameLogic::player_color()
{
    return white_player;
}

bool gameLogic::switch_player()
{
    white_player = !white_player;
    return white_player;
}

gameLogic::gameLogic(Square **squares_ptr, size_t array_size, size_t board_side_in , bool free_array_in,bool free_squares_in,int empty_moves_in)
    :empty_moves(empty_moves_in),free_array(free_array_in),free_squares(free_squares_in),squares(squares_ptr)
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
    free_array = true;

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
    squares = new Square*[board_side_in*board_side_in];
    free_array = true;
    free_squares = true;
    bool black = false;

    int rows = 3;
    if(board_side == 8){
        rows = 3;
    }else if(board_side == 10){
        rows = 4;
    }

    size_t i = 0;

    size_t cap = board_side*rows;

    for(;i < cap;++i){
        if(black){
            squares[i] = new Square((i%board_side),(i/board_side),Square::Black);
            if(i%board_side != board_side-1){
                black = false;
            }
        }else{
            squares[i] = new Square((i%board_side),(i/board_side),Square::Empty);
            if(i%board_side != board_side-1){
                black = true;
            }
        }
    }

    cap += board_side*(board_side-2*rows);
    for(;i < cap;++i){
        if(black){
            squares[i] = new Square((i%board_side),(i/board_side),Square::Empty);
            if(i%board_side != board_side-1){
                black = false;
            }
        }else{
            squares[i] = new Square((i%board_side),(i/board_side),Square::Empty);
            if(i%board_side != board_side-1){
                black = true;
            }
        }
    }
    cap +=  board_side*rows;

    for(;i < cap;++i){
        if(black){
            squares[i] = new Square((i%board_side),(i/board_side),Square::White);
            if(i%board_side != board_side-1){
                black = false;
            }
        }else{
            squares[i] = new Square((i%board_side),(i/board_side),Square::Empty);
            if(i%board_side != board_side-1){
                black = true;
            }
        }
    }

    squares_size = board_side_in*board_side_in;
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

gameLogic* gameLogic::clone()
{
    Square** new_squares = new Square*[board_side*board_side];
    size_t new_squares_size = squares_size;
    size_t new_board_side = board_side;
    for(size_t i = 0; i < new_squares_size; ++i){
        new_squares[i] = new Square(squares[i]->coordinates(),squares[i]->show_piece());
    }
    return new gameLogic(new_squares,new_squares_size,new_board_side,true,true,empty_moves);
}

gameLogic::GameState gameLogic::game_state()
{
    return state;
}

void gameLogic::update_moves()
{
    if(game_state() != gameLogic::inProgress){
        return;
    }
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
    }else if(empty_moves >= moves_before_draw){
        state = draw;
    }

    possible_moves = moves;
}

bool gameLogic::playerInput(const gameLogic::Move &player_move)
{
    if(game_state() != gameLogic::inProgress){
        return false;
    }
    if(player_move.second.size() > 1){
        empty_moves = 0;
    }else{
        ++empty_moves;
    }
    Square::Piece played_piece = squares[square_pos(player_move.first[0])]->update_piece(Square::Empty);
    remove_piece(player_move.first.front(),played_piece);
    for(auto it = player_move.second.begin()+1;it != player_move.second.end();++it){
        Square::Piece piece = squares[square_pos((*it).first)]->update_piece(Square::Empty);
        remove_piece(((*it).first),piece);
    }
    add_piece(player_move.first.back(),played_piece);
    switch_player();
    update_moves();
    return true;
}

gameLogic::GameState gameLogic::game_state() const
{
    return state;
}

gameLogic::Coordinates gameLogic::go_in_direction(gameLogic::Moves &moves,Move &current_move,Coordinates &current_suqare,std::function<gameLogic::Coordinates(gameLogic::Coordinates)> &next_square_func , std::function<bool(Coordinates)> &boudires_condition, bool& move_ended, bool enemy_piece, size_t& longest_move, bool &taken)
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
        if(next_piece == Square::Empty || (std::find_if(current_move.second.begin(),current_move.second.end(),[next_square](std::pair<Coordinates,bool> pair){return next_square == pair.first;}) != current_move.second.end())){
            if(!enemy_piece){
                if(current_move.second.size() == 1){
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
                if(squares[square_pos(current_suqare)]->show_piece() == Square::WhiteKing || squares[square_pos(current_suqare)]->show_piece() == Square::BlackKing){
                    current_move.second.emplace_back(std::make_pair(current_suqare, true));
                }else{
                    current_move.second.emplace_back(std::make_pair(current_suqare, false));
                }
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
                    return go_in_direction(moves,current_move,next_square,next_square_func,boudires_condition,move_ended,enemy_piece,longest_move,taken); //(musisz pojsc jeszcze raz w ten sam direction gdy jest pierwszy raz enemy piece)
                }
            }
        }else{
            if(white_player){
                if(enemy_piece){
                    move_ended = true;
                    return Coordinates();
                }else{
                    enemy_piece = true;
                    return go_in_direction(moves,current_move,next_square,next_square_func,boudires_condition,move_ended,enemy_piece,longest_move,taken); //(musisz pojsc jeszcze raz w ten sam direction gdy jest pierwszy raz enemy piece)
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
        if(enemy_piece){
            taken = true;
        }
    }
    return;
}

void gameLogic::moves_of_a_piece(gameLogic::Coordinates &piece,size_t &longest_move,gameLogic::Moves &moves, bool &taken,bool king)
{
    const int board_size = gameLogic::board_side;
    static std::function<bool(gameLogic::Coordinates)> br_condition = [board_size](Coordinates c){return (c.first < board_size && c.second < board_size);};
    static std::function<bool(gameLogic::Coordinates)> bl_condition = [board_size](Coordinates c){return (c.first >= 0 && c.second < board_size);};
    static std::function<bool(gameLogic::Coordinates)> tr_condition = [board_size](Coordinates c){return (c.first < board_size && c.second >= 0);};
    static std::function<bool(gameLogic::Coordinates)> tl_condition = [](Coordinates c){return (c.first >= 0 && c.second >= 0);};

    static std::function<gameLogic::Coordinates(gameLogic::Coordinates)> br_square = [](gameLogic::Coordinates c){return std::make_pair(c.first + 1,c.second+1);};
    static std::function<gameLogic::Coordinates(gameLogic::Coordinates)> bl_square = [](gameLogic::Coordinates c){return std::make_pair(c.first - 1,c.second+1);};
    static std::function<gameLogic::Coordinates(gameLogic::Coordinates)> tr_square = [](gameLogic::Coordinates c){return std::make_pair(c.first + 1,c.second-1);};
    static std::function<gameLogic::Coordinates(gameLogic::Coordinates)> tl_square = [](gameLogic::Coordinates c){return std::make_pair(c.first - 1,c.second-1);};

    std::stack<std::array<bool,4>> directions_to_check;
    Coordinates current_suqare = piece;
    if(king){
         directions_to_check.push(std::array<bool,4>{true,true,true,true});
    }
    else if(white_player){
        directions_to_check.push(std::array<bool,4>{false,false,true,true});
    }else{
        directions_to_check.push(std::array<bool,4>{true,true,false,false});
    }
    Move current_move;
    current_move.first.push_back(piece);
    current_move.second.push_back(std::make_pair(piece,king));

     //direction order is: BR,BL,TR,TL

    while(!directions_to_check.empty()){
        bool move_ended = false;
        bool enemy_piece = false;
        if(directions_to_check.top()[0]){
            check_direction(directions_to_check,0,moves,current_move,current_suqare,br_square,br_condition,move_ended,enemy_piece,longest_move,taken,king);
        }else if(directions_to_check.top()[1]){
            check_direction(directions_to_check,1,moves,current_move,current_suqare,bl_square,bl_condition,move_ended,enemy_piece,longest_move,taken,king);
        }else if(directions_to_check.top()[2]){
            check_direction(directions_to_check,2,moves,current_move,current_suqare,tr_square,tr_condition,move_ended,enemy_piece,longest_move,taken,king);
        }else if(directions_to_check.top()[3]){
            check_direction(directions_to_check,3,moves,current_move,current_suqare,tl_square,tl_condition,move_ended,enemy_piece,longest_move,taken,king);
        }else{
            directions_to_check.pop();
            current_move.first.pop_back();
            current_suqare = current_move.first.back();
            current_move.second.pop_back();
            assert((current_move.first.size() != 1 && current_move.second.size() != 1) || (current_move.first.size() == 1 && current_move.second.size() == 1));
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

void gameLogic::rollback(gameLogic::Move &move, int empty_moves_in)
{
    empty_moves = empty_moves_in;
    state = gameLogic::inProgress;
    Square::Piece played_piece = squares[square_pos(move.first.back())]->update_piece(Square::Empty);
    remove_piece(move.first.back(),played_piece);
    for(auto it = move.second.begin()+1;it != move.second.end();++it){
        Square::Piece piece;
        if((*it).second){
            if(white_player){
                piece = Square::WhiteKing;
            }
            else{
                piece = Square::BlackKing;
            }
        }else{
            if(white_player){
                piece = Square::White;
            }
            else{
                piece = Square::Black;
            }
        }
        add_piece((*it).first,piece);
    }
    add_piece(move.second.front().first,played_piece);
    switch_player();
}

void gameLogic::rollback_move(gameLogic::Move& move, int empty_moves_in)
{
    rollback(move,empty_moves_in);
    update_moves();
    return;
}

void gameLogic::rollback_move_no_update(gameLogic::Move &move, gameLogic::Moves &moves, int empty_moves_in)
{
    rollback(move,empty_moves_in);
    possible_moves = moves;
    return;
}

gameLogic::Coordinates gameLogic::board_pos(size_t square_pos)
{
    return std::make_pair(square_pos%board_side,square_pos/board_side);
}

bool gameLogic::compere_cordinates(const gameLogic::Coordinates c1, const gameLogic::Coordinates c2)
{
    return (c1.first == c2.first && c1.second == c2.second);
}

Square **gameLogic::board_ptr()
{
    return squares;
}

std::array<std::vector<gameLogic::Coordinates> *, 4> gameLogic::pieces()
{
    return std::array<std::vector<gameLogic::Coordinates> *, 4>{&whiteMen,&blackMen,&whiteKings,&blackKings};
}

size_t gameLogic::board_size()
{
    return board_side;
}

int gameLogic::moves_towards_draw()
{
    return empty_moves;
}

void gameLogic::check_direction(std::stack<std::array<bool, 4>> &directions_to_check, size_t direction, gameLogic::Moves &moves, gameLogic::Move &current_move, gameLogic::Coordinates &current_suqare, std::function<gameLogic::Coordinates (gameLogic::Coordinates)> &next_square_func, std::function<bool (gameLogic::Coordinates)> &boudires_condition, bool &move_ended, bool enemy_piece, size_t &longest_move, bool &taken, bool king)
{
    //direction order is: BR,BL,TR,TL

    gameLogic::Coordinates temp_square = go_in_direction(moves,current_move,current_suqare,next_square_func,boudires_condition,move_ended,enemy_piece,longest_move,taken);

    directions_to_check.top()[direction] = false;

    if(!move_ended){
        std::array<bool,4> array;
        if(king){
            switch(direction){
            case 0:
                array = std::array<bool,4>{true,true,true,false};
                break;
            case 1:
                array = std::array<bool,4>{true,true,false,true};
                break;
            case 2:
                array = std::array<bool,4>{true,false,true,true};
                break;
            case 3:
                array = std::array<bool,4>{false,true,true,true};
                break;
            default:
                assert(true);
                // by default all are false
                break;
            }
        }else{
            if(white_player){
                array = std::array<bool,4>{false,false,true,true};
            }else{
                array = std::array<bool,4>{true,true,false,false};
            }
        }
        directions_to_check.push(array);
        current_suqare = temp_square;
    }
}

gameLogic::~gameLogic()
{
    if(free_array && squares != nullptr){
        if(free_squares){
            for(size_t i =0; i < squares_size;++i){
                delete squares[i];
            }
        }
        delete squares;
    }
}
