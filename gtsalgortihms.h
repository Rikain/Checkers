#ifndef GTSALGORTIHMS_H
#define GTSALGORTIHMS_H
#include "ai.h"

class GTSAlgortihms : public AI
{
    int height;
    static int min_max(int h, gameLogic* game, bool max_player);
    gameLogic::Move do_min_max();
    int alpha_beta();
    static int state_evaluation(gameLogic* game);
    void reset();
    bool pruning = false;
public:
    GTSAlgortihms(int height_in, bool pruning_in, gameLogic *game_in = nullptr);
    gameLogic::Move return_a_move() override;
    void set_height(int h);
};

#endif // GTSALGORTIHMS_H
