#include "ai.h"

gameLogic::Move AI::return_a_move()
{
    assert(move_function);
    return move_function();
}

AI::AI(gameLogic *game_in)
    :game(game_in)
{
}

AI::~AI()
{
}

