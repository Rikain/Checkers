#ifndef AI_H
#define AI_H
#include "gamelogic.h"

class AI
{
protected:
    gameLogic *game;
public:
    virtual gameLogic::Move return_a_move() = 0;
    AI(gameLogic *game);
    virtual ~AI();
};

#endif // AI_H
