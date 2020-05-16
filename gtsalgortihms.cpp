#include "gtsalgortihms.h"

int GTSAlgortihms::min_max(int h)
{
    assert(h >= 0);
    if(h == 0 || game->game_state() != gameLogic::GameState::inProgress){
       return state_evaluation();
    }
    if(max_player){
        int value = INT_MIN;
        for(auto move : game->possible_moves){
            game->playerInput(move);
            value = std::max(value,min_max(h-1));
            game->rollback_move(move);
        }
        return value;
    }else{
        int value = INT_MAX;
        for(auto move : game->possible_moves){
            game->playerInput(move);
            value = std::min(value,min_max(h-1));
            game->rollback_move(move);
        }
        return value;
    }
}

int GTSAlgortihms::state_evaluation()
{
    return 0;
}

void GTSAlgortihms::reset()
{
    max_player = true;
}

GTSAlgortihms::GTSAlgortihms(gameLogic *game_in, int height_in, bool pruning)
    :AI(game_in),height(height_in)
{
    if(pruning){
        //move_function =
    }else{
        //move_function =
    }
}

void GTSAlgortihms::set_height(int h)
{
    assert(h > 0);
    height = h;
}
