#include "ai.h"
#include <chrono>

AI::AI(gameLogic *game_in)
    :game(game_in)
{
    gen.seed(std::chrono::system_clock::now().time_since_epoch().count());
}

AI::~AI()
{
}

