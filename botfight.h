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
public:
    botFight(bool pruning1 = false, int height1 = 5, bool pruning2 = false, int height2 = 5);
    ~botFight();
    std::tuple<int,int,int> make_them_fight(int times=10, bool ai1_is_white = true); //win-lose-draw
    void print_a_fight(int times=10, bool ai1_is_white = true);
};

#endif // BOTFIGHT_H
