#ifndef GTSALGORTIHMS_H
#define GTSALGORTIHMS_H
#include "ai.h"

class GTSAlgortihms : public AI
{
    int height;
    int min_max(int h);
    int alpha_beta();
    int state_evaluation();
    bool max_player = true;
    void reset();
public:
    GTSAlgortihms(gameLogic *game_in, int height_in, bool pruning);
    void set_height(int h);
};

#endif // GTSALGORTIHMS_H
