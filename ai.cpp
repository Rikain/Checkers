#include "ai.h"
#include <chrono>

gameLogic *AI::change_game_ptr(gameLogic* new_game)
{
    gameLogic* temp = game;
    game = new_game;
    return temp;
}

AI::AI(gameLogic *game_in)
    :game(game_in)
{
    gen.seed(std::chrono::system_clock::now().time_since_epoch().count());
}

AI::~AI()
{
}

