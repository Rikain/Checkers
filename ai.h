#ifndef AI_H
#define AI_H
#include "gamelogic.h"

class AI
{
protected:
    gameLogic *game;
    std::function<gameLogic::Move()> move_function;
public:
    gameLogic::Move return_a_move();
    AI(gameLogic *game);
    virtual ~AI();
};

#endif // AI_H
