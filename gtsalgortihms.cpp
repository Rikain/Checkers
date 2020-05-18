#include "gtsalgortihms.h"
#include <thread>
#include <future>

int GTSAlgortihms::min_max(int h, gameLogic* game, bool max_player)
{
    assert(h >= 0);
    if(h == 0 || game->game_state() != gameLogic::GameState::inProgress){
       return state_evaluation(game);
    }
    if(max_player){
        int value = INT_MIN;
        max_player = !max_player;
        gameLogic::Moves moves = game->possible_moves;
        for(auto move : moves){
            int empty_move_state = game->moves_towards_draw();
            game->playerInput(move);
            value = std::max(value,min_max(h-1,game,max_player));
            game->rollback_move_no_update(move,moves,empty_move_state);
        }
        return value;
    }else{
        int value = INT_MAX;
        max_player = !max_player;
        gameLogic::Moves moves = game->possible_moves;
        for(auto move : moves){
            int empty_move_state = game->moves_towards_draw();
            game->playerInput(move);
            value = std::min(value,min_max(h-1,game,max_player));
            game->rollback_move_no_update(move,moves,empty_move_state);
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
    bool max_player = !game->player_color();
    int i = 0;
    for(auto move : game->possible_moves){
        new_game[i] = game->clone();
        new_game[i]->playerInput(move);
        values[i] = std::async(min_max,h,new_game[i],max_player);//min_max(h,new_game[i],max_player);// //std::launch::async,
        ++i;
    }

    if(game->player_color()){
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

int GTSAlgortihms::state_evaluation(gameLogic* game)
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

void GTSAlgortihms::reset()
{
}

GTSAlgortihms::GTSAlgortihms(gameLogic *game_in, int height_in, bool pruning_in)
    :AI(game_in),height(height_in),pruning(pruning_in)
{
}

gameLogic::Move GTSAlgortihms::return_a_move()
{
    if(pruning){
        return do_min_max();
    }else{
        return do_min_max();
    }
}

void GTSAlgortihms::set_height(int h)
{
    assert(h > 0);
    height = h;
}
