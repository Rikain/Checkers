#ifndef GAMELOGIC_H
#define GAMELOGIC_H
#include <vector>
#include "qsquare.h"
#include <functional>

class gameLogic
{
public:
    enum GameState{
        inProgress,
        whiteWon,
        blackWon,
        draw
    };
    typedef std::pair<int,int> Coordinates;
    typedef std::pair<std::vector<Coordinates>,std::vector<Coordinates>> Move;
    typedef std::vector<Move> Moves;

    bool switch_player();
    gameLogic(Square **squares_ptr, size_t array_size, size_t board_side_in);
    gameLogic(QSquare **squares_ptr,size_t board_side_in);
    gameLogic(size_t board_side_in = 10);
    void update_moves();
    Moves possible_moves;
    void playerInput(const Move &player_move);
    GameState game_state() const;
    int square_pos(Coordinates board_pos);
    void remove_piece(Coordinates cor, Square::Piece &piece);
    void add_piece(Coordinates cor, Square::Piece &piece);
    Coordinates board_pos(size_t square_pos);
    static bool compere_cordinates(const Coordinates c1, const Coordinates c2);
    ~gameLogic();
private:
    GameState state = inProgress;
    bool white_player = true;

    Coordinates go_in_direction(gameLogic::Moves &moves,Move &current_move,Coordinates &current_suqare,std::function<gameLogic::Coordinates(gameLogic::Coordinates)> &next_square_func , std::function<bool(Coordinates)> &boudires_condition,bool player_move_condition, bool& move_ended, bool enemy_piece, size_t& longest_move, bool white_player, bool &taken, bool king = false);
    void add_move(gameLogic::Moves &moves,Move &current_move,size_t& longest_move, bool &taken, bool enemy_piece);
    void moves_of_a_piece(gameLogic::Coordinates &piece,size_t &longest_move,gameLogic::Moves &moves, bool &taken,bool king = false);
    void reset_add_move(gameLogic::Moves &moves,Move &current_move,size_t& longest_move);

    Square **squares = nullptr;
    size_t squares_size = 100;
    size_t board_side = 10;
    //map of pair to Square::Piece
    std::vector<Coordinates> whiteMen;
    std::vector<Coordinates> whiteKings;
    std::vector<Coordinates> blackMen;
    std::vector<Coordinates> blackKings;
};

#endif // GAMELOGIC_H
