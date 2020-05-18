#include "gtsalgortihms.h"
#include <thread>
#include <future>

int GTSAlgortihms::min_max(int h, gameLogic* game, bool max_player, int alpha, int beta, bool pruning)
{
    assert(h >= 0);
    if(h == 0 || game->game_state() != gameLogic::GameState::inProgress){
       return AI::state_evaluation(game);
    }
    if(max_player){
        int value = INT_MIN;
        gameLogic::Moves moves = game->possible_moves;
        for(auto move : moves){
            int empty_move_state = game->moves_towards_draw();
            game->playerInput(move);
            value = std::max(value,min_max(h-1,game,false,alpha,beta,pruning));
            game->rollback_move_no_update(move,moves,empty_move_state);
            if(pruning){
                alpha = std::max(alpha,value);
                if(alpha >= beta){
                    break;
                }
            }
        }
        return value;
    }else{
        int value = INT_MAX;
        gameLogic::Moves moves = game->possible_moves;
        for(auto move : moves){
            int empty_move_state = game->moves_towards_draw();
            game->playerInput(move);
            value = std::min(value,min_max(h-1,game,true,alpha,beta,pruning));
            game->rollback_move_no_update(move,moves,empty_move_state);
            if(pruning){
                beta = std::min(beta,value);
                if(alpha >= beta){
                    break;
                }
            }
        }
        return value;
    }
}

gameLogic::Move GTSAlgortihms::do_min_max()
{
    int h = height -1;
    int len = game->possible_moves.size();
    std::future<int> values[len];
    //int values[len];
    gameLogic *new_game[len];
    std::vector<int> correct_move;
    bool max_player = game->player_color();
    int i = 0;
    int alpha = INT_MIN;
    int beta = INT_MAX;
    for(auto move : game->possible_moves){
        new_game[i] = game->clone();
        new_game[i]->playerInput(move);
        values[i] = std::async(min_max,h,new_game[i],!max_player,alpha,beta,pruning);//min_max(h,new_game[i],max_player);// //std::launch::async,
        ++i;
    }

    if(max_player){
        int max = INT_MIN;
        for(int i = 0;i < len;++i){
            int temp = values[i].get();
            if(temp > max){
                correct_move.clear();
                correct_move.push_back(i);
                max = temp;
            }else if(temp == max){
                correct_move.push_back(i);
            }
        }
    }else{
        int min = INT_MAX;
        for(int i = 0;i < len;++i){
            int temp = values[i].get();
            if(temp < min){
                correct_move.clear();
                correct_move.push_back(i);
                min = temp;
            }else if(temp == min){
                correct_move.push_back(i);
            }
        }
    }
    for(int i=0;i<len;++i){
        delete new_game[i];
    }
    assert(!correct_move.empty());
    std::uniform_int_distribution<int> distribution(0,correct_move.size()-1);
    int index = distribution(gen);
    return game->possible_moves[correct_move[index]];
}


void GTSAlgortihms::reset()
{
}

GTSAlgortihms::GTSAlgortihms(int height_in, bool pruning_in, gameLogic *game_in)
    :AI(game_in),height(height_in),pruning(pruning_in)
{
}

gameLogic::Move GTSAlgortihms::return_a_move()
{
   return do_min_max();
}

void GTSAlgortihms::set_height(int h)
{
    assert(h > 0);
    height = h;
}

void GTSAlgortihms::set_pruning(bool prune)
{
    pruning = prune;
}
