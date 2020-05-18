#ifndef GTSALGORTIHMS_H
#define GTSALGORTIHMS_H
#include "ai.h"
#include <random>

class GTSAlgortihms : public AI
{
    int height;
    static int min_max(int h, gameLogic* game, bool max_player);
    gameLogic::Move do_min_max();
    int alpha_beta();
    static int state_evaluation(gameLogic* game);
    void reset();
    bool pruning = false;
    std::default_random_engine gen;
public:
    GTSAlgortihms(gameLogic *game_in, int height_in, bool pruning);
    gameLogic::Move return_a_move() override;
    void set_height(int h);
};

#endif // GTSALGORTIHMS_H
