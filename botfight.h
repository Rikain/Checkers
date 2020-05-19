#ifndef BOTFIGHT_H
#define BOTFIGHT_H
#include "gtsalgortihms.h"
#include <tuple>

class botFight
{
    AI *ai1 = nullptr;
    AI *ai2 = nullptr;
    gameLogic *game = nullptr;
    gameLogic *game_ai1 = nullptr;
    gameLogic *game_ai2 = nullptr;

private:
    void flip_ai(AI* current_ai);
    void flip_time_ptr(unsigned long long* &over_all_time,unsigned long long *over_all_time_ai1,unsigned long long *over_all_time_ai2);
    void flip_int_ptr(int* &move, int* move_ai1, int* move_ai2);
public:
    botFight(bool pruning1, int height1, AI::Eval eval1, bool pruning2, int height2, AI::Eval eval2);
    ~botFight();
    std::tuple<int,int,int> make_them_fight(int times, bool ai1_is_white, bool measure_time ,unsigned long long* &over_all_time_ai1,unsigned long long* &over_all_time_ai2,int* &moves_ai1, int* &moves_ai2); //win-lose-draw
    void print_a_fight(int times=10, bool ai1_is_white = true);
};

#endif // BOTFIGHT_H
