#include "botfight.h"
#include <iostream>

void botFight::flip_ai(AI *current_ai)
{
    assert(current_ai == ai1 || current_ai == ai2);
    if(current_ai == ai1){
        current_ai = ai2;
        return;
    }else{
        current_ai = ai1;
        return;
    }
}

void botFight::flip_time_ptr(unsigned long long* &over_all_time, unsigned long long *over_all_time_ai1, unsigned long long *over_all_time_ai2)
{
    assert(over_all_time == over_all_time_ai1 || over_all_time == over_all_time_ai2);
    if(over_all_time == over_all_time_ai1){
        over_all_time = over_all_time_ai2;
    }else if (over_all_time == over_all_time_ai2){
        over_all_time = over_all_time_ai1;
    }
    return;
}

void botFight::flip_int_ptr(int* &move, int *move_ai1, int *move_ai2)
{
    assert(move == move_ai1 || move == move_ai2);
    if(move == move_ai1){
        move = move_ai2;
    }else if (move == move_ai2){
        move = move_ai1;
    }
    return;
}

botFight::botFight(bool pruning1, int height1, AI::Eval eval1, bool pruning2, int height2, AI::Eval eval2)
{
    game = new gameLogic();
    game_ai1 = game->clone();
    game_ai2 = game->clone();
    ai1 = new GTSAlgortihms(height1,pruning1,eval1);
    ai2 = new GTSAlgortihms(height2,pruning2,eval2);
}

botFight::~botFight()
{
    if(ai1 != nullptr){
        delete ai1;
    }
    if(ai2 != nullptr){
        delete ai2;
    }
    if(game != nullptr){
        delete game;
    }
    if(game_ai1 != nullptr){
        delete game_ai1;
    }
    if(game_ai2 != nullptr){
        delete game_ai2;
    }
}

std::tuple<int, int, int> botFight::make_them_fight(int times, bool ai1_is_white,bool measure_time, unsigned long long* &over_all_time_ai1,unsigned long long*& over_all_time_ai2,int*& moves_ai1, int* &moves_ai2)
{
    int wins = 0;
    int loses = 0;
    int draws = 0;
    assert(ai1 != nullptr);
    assert(ai2 != nullptr);
    for(int i =0;i<times;++i){
        gameLogic* their_game = game->clone();
        ai1->change_game_ptr(their_game);
        ai2->change_game_ptr(their_game);
        AI* ai_to_move;
        unsigned long long *over_all_time;
        int *moves;
        if(ai1_is_white){
            ai_to_move = ai1;
            over_all_time = over_all_time_ai1;
            moves = moves_ai1;
        }else{
            ai_to_move = ai2;
            over_all_time = over_all_time_ai2;
            moves = moves_ai2;
        }
        while(their_game->game_state() == gameLogic::inProgress){
            if(measure_time){
                (*moves) +=1;
                std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();
                auto move = ai_to_move->return_a_move();
                std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
                their_game->playerInput(move);
                (*over_all_time) += (std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count());
                flip_time_ptr(over_all_time,over_all_time_ai1,over_all_time_ai2);
                flip_int_ptr(moves,moves_ai1,moves_ai2);
            }else{
                their_game->playerInput(ai_to_move->return_a_move());
            }
            flip_ai(ai_to_move);
        }
        if(their_game->game_state() == gameLogic::whiteWon){
            ++wins;
        }else if(their_game->game_state() == gameLogic::blackWon){
            ++loses;
        }else if(their_game->game_state() == gameLogic::draw){
            ++draws;
        }
        delete their_game;
        their_game = nullptr;
    }
    ai1->change_game_ptr(nullptr);
    ai2->change_game_ptr(nullptr);
    return std::make_tuple(wins,loses,draws);
}

void botFight::print_a_fight(int times, bool ai1_is_white)
{
    unsigned long long *over_all_time_ai1 = new unsigned long long();
    unsigned long long *over_all_time_ai2 = new unsigned long long();
    int *moves_ai1 = new int();
    int *moves_ai2 = new int();
    auto tuple = make_them_fight(times,ai1_is_white,true,over_all_time_ai1,over_all_time_ai2,moves_ai1,moves_ai2);
    std::cout << "AI 1 won: " << std::get<0>(tuple) << " times, lost: " << std::get<1>(tuple) << " times, draw: " << std::get<2>(tuple) << " times!" << std::endl;
    std::cout << "Time spent by Ai1 = " << (*over_all_time_ai1) << " ms." << std::endl;
    std::cout << "Time spent by Ai2 = " << (*over_all_time_ai2) << " ms." << std::endl;
    std::cout << "Average time spent by Ai1 = " << (*over_all_time_ai1)/(*moves_ai1) << " ms. It made :" << (*moves_ai1)/times << " moves on average." << std::endl;
    std::cout << "Average time spent by Ai2 = " << (*over_all_time_ai2)/(*moves_ai2) << " ms. It made :" << (*moves_ai2)/times << " moves on average." << std::endl;
    delete over_all_time_ai1;
    delete over_all_time_ai2;
    delete moves_ai1;
    delete moves_ai2;
}
