#include "ai.h"
#include <chrono>

gameLogic *AI::change_game_ptr(gameLogic* new_game)
{
    gameLogic* temp = game;
    game = new_game;
    return temp;
}

int AI::state_evaluation(gameLogic* game)
{
    if(game->game_state() != gameLogic::inProgress){
        int value = 0;
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
        return value;
    }else{
        int value = 0;
        std::array<std::vector<gameLogic::Coordinates>*,4> pieces = game->pieces();
        for(auto piece : (*(pieces[0]))){
            value += 1;//(10 + game->board_size() - (piece.second+1));
        }
        for(auto piece : (*(pieces[1]))){
            value -= 1;//(10 + piece.second);
        }
        for(auto piece : (*(pieces[2]))){
            value += 2;//(game->board_size() + 15);
        }
        for(auto piece : (*(pieces[3]))){
            value -= 2;//(game->board_size() + 15);
        }
        return value;
    }
    return 0;
}

AI::AI(gameLogic *game_in)
    :game(game_in)
{
    gen.seed(std::chrono::system_clock::now().time_since_epoch().count());
}

AI::~AI()
{
}

