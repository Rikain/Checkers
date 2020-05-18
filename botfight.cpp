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

botFight::botFight(bool pruning1, int height1, bool pruning2, int height2)
{
    game = new gameLogic();
    game_ai1 = game->clone();
    game_ai2 = game->clone();
    ai1 = new GTSAlgortihms(height1,pruning1);
    ai2 = new GTSAlgortihms(height2,pruning2);
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

std::tuple<int, int, int> botFight::make_them_fight(int times, bool ai1_is_white)
{
    int wins = 0;
    int loses = 0;
    int draws = 0;
    for(int i =0;i<times;++i){
        gameLogic* their_game = game->clone();
        ai1->change_game_ptr(their_game);
        ai2->change_game_ptr(their_game);
        AI* ai_to_move;
        if(ai1_is_white){
            ai_to_move = ai1;
        }else{
            ai_to_move = ai2;
        }
        while(their_game->game_state() == gameLogic::inProgress){
            their_game->playerInput(ai_to_move->return_a_move());
            flip_ai(ai_to_move);
        }
        if(their_game->game_state() == gameLogic::whiteWon){
            std::cout<< "AI 1 won!" << std::endl;
            ++wins;
        }else if(their_game->game_state() == gameLogic::blackWon){
            std::cout<< "AI 1 lost!" << std::endl;
            ++loses;
        }else if(their_game->game_state() == gameLogic::draw){
            std::cout<< "AI 1 draw!" << std::endl;
            ++draws;
        }
        delete their_game;
        their_game = nullptr;
    }
    return std::make_tuple(wins,loses,draws);
}

void botFight::print_a_fight(int times, bool ai1_is_white)
{
    auto tuple = make_them_fight(times,ai1_is_white);
    std::cout << "AI 1 won: " << std::get<0>(tuple) << " times, lost: " << std::get<1>(tuple) << " times, draw: " << std::get<2>(tuple) << " times!";
}
