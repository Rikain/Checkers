#include "ai.h"
#include <chrono>

int AI::state_evaluation_board_square(gameLogic *game)
{
    int value = 0;
    if(game_state_eval(game,value)){
        return value;
    }
    else{
        int value = 0;
        int board_len = game->board_size();
        std::array<std::vector<gameLogic::Coordinates>*,4> pieces = game->pieces();
        for(auto piece : (*(pieces[0]))){
            board_square_add_val(value,piece,board_len);
        }
        for(auto piece : (*(pieces[1]))){
            int val2 = 0;
            board_square_add_val(val2,piece,board_len);
            value -= val2;
        }
        for(auto piece : (*(pieces[2]))){
            board_square_add_val(value,piece,board_len);
            board_square_add_val(value,piece,board_len);
        }
        for(auto piece : (*(pieces[3]))){
            int val2 = 0;
            board_square_add_val(value,piece,board_len);
            value -= (val2*2);
        }
        return value;
    }
    return 0;
}

int AI::state_evaluation_board_pieces_mix(gameLogic *game)
{
    if(pieces_in_game(game) <= static_cast<int>(2*game->board_size())){
        return state_evaluation_pieces_only(game);
    }else{
        return  state_evaluation_board_square(game);
    }
}

int AI::evaluate(gameLogic *game, AI::Eval evaluation)
{
    switch(evaluation){
    case(AI::Mix):
        return state_evaluation_board_pieces_mix(game);
        break;
    case(AI::BoardSquareOnly):
        return state_evaluation_board_square(game);
        break;
    default:
        return state_evaluation_pieces_only(game);
        break;
    }
    return 0;
}

gameLogic *AI::change_game_ptr(gameLogic* new_game)
{
    gameLogic* temp = game;
    game = new_game;
    return temp;
}

bool AI::game_state_eval(gameLogic* game, int &value)
{
    if(game->game_state() != gameLogic::inProgress){
        value = 0;
        switch(game->game_state()){
        case(gameLogic::whiteWon):
            value = INT_MAX;
            break;
        case(gameLogic::blackWon):
            value = INT_MIN;
        default:
            //draw
            break;
        }
        return true;
    }else {
        return false;
    }
}

void AI::board_square_add_val(int &value, gameLogic::Coordinates piece, int board_len)
{
    if(piece.first == 0 || piece.first == (board_len-1) || piece.second == 0 || piece.second == (board_len-1)){
        value += 4;
    }else if(piece.first == 1|| piece.first == (board_len-1)-1 || piece.second == 1 || piece.second == (board_len-1)-1){
        value += 3;
    }else if(piece.first == 2|| piece.first == (board_len-1)-2 || piece.second == 2 || piece.second == (board_len-1)-2){
        value += 2;
    }else{
        value += 1;
    }
}

int AI::pieces_in_game(gameLogic *game)
{
    std::array<std::vector<gameLogic::Coordinates>*,4> pieces = game->pieces();
    return pieces[0]->size() + pieces[1]->size() + pieces[2]->size() + pieces[3]->size();
}

int AI::state_evaluation_pieces_only(gameLogic* game)
{
    int value = 0;
    if(game_state_eval(game,value)){
        return value;
    }
    else{
        int value = 0;
        std::array<std::vector<gameLogic::Coordinates>*,4> pieces = game->pieces();
        for(auto piece : (*(pieces[0]))){
            value += 1;
        }
        for(auto piece : (*(pieces[1]))){
            value -= 1;
        }
        for(auto piece : (*(pieces[2]))){
            value += 2;
        }
        for(auto piece : (*(pieces[3]))){
            value -= 2;
        }
        return value;
    }
    return 0;
}

AI::AI(Eval evalutaion_in, gameLogic *game_in)
    :game(game_in),evaluation(evalutaion_in)
{
    gen.seed(std::chrono::system_clock::now().time_since_epoch().count());
}

AI::~AI()
{
}

