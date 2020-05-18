#ifndef AI_H
#define AI_H
#include "gamelogic.h"
#include <random>

class AI
{
public:
    enum Eval{
        PiecesOnly,
        BoardSquareOnly,
        Mix
    };
private:
    static bool game_state_eval(gameLogic* game, int &value);
    static void board_square_add_val(int &value, gameLogic::Coordinates piece, int board_len);
    static int pieces_in_game(gameLogic* game);
protected:
    static int state_evaluation_pieces_only(gameLogic* game);
    static int state_evaluation_board_square(gameLogic* game);
    static int state_evaluation_board_pieces_mix(gameLogic* game);
    static int evaluate(gameLogic* game,Eval evaluation);
protected:
    gameLogic *game = nullptr;
    AI::Eval evaluation;
public:
    std::default_random_engine gen;
    gameLogic* change_game_ptr(gameLogic* new_game);
    virtual gameLogic::Move return_a_move() = 0;
    AI(Eval evalutaion_in = AI::PiecesOnly,gameLogic *game = nullptr);
    virtual ~AI();
};

#endif // AI_H
