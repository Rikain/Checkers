#ifndef AI_H
#define AI_H
#include "gamelogic.h"
#include <random>

class AI
{
protected:
    gameLogic *game;
public:
    std::default_random_engine gen;
    virtual gameLogic::Move return_a_move() = 0;
    AI(gameLogic *game);
    virtual ~AI();
};

#endif // AI_H
