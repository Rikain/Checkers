#ifndef GAMELOGIC_H
#define GAMELOGIC_H
#include "qsquare.h"
#include <vector>
#include <functional>
#include <stack>

class gameLogic
{
public:
    enum GameState{
        inProgress,
        whiteWon,
        blackWon,
        draw
    };
    typedef std::pair<int,int> Coordinates; //x,y
    typedef std::pair<std::vector<Coordinates>,std::vector<std::pair<Coordinates,bool>>> Move; //vecotr of moving piece cor , vector of deleted piece cor, is_a_king
    typedef std::vector<Move> Moves;

    bool switch_player();
    gameLogic(Square **squares_ptr, size_t array_size, size_t board_side_in);
    gameLogic(QSquare **squares_ptr,size_t board_side_in);
    gameLogic(size_t board_side_in = 8);
    gameLogic(gameLogic &&game) = delete;
    gameLogic(gameLogic &game) = delete;
    gameLogic& operator=(const gameLogic& rhs) = delete;
    gameLogic& operator=(gameLogic&& rhs) = delete;
    void update_moves();
    Moves possible_moves;
    void playerInput(const Move &player_move); 
    GameState game_state() const;
    int square_pos(Coordinates board_pos);
    void rollback_move(Move& move);
    void rollback_move_no_update(Move& move,Moves& moves);
    Coordinates board_pos(size_t square_pos);
    static bool compere_cordinates(const Coordinates c1, const Coordinates c2);
    ~gameLogic();
private:
    GameState state = inProgress;
    bool white_player = true;
    bool free_array = false;
    void check_direction(std::stack<std::array<bool,4>> &directions_to_check,size_t direction,gameLogic::Moves &moves,Move &current_move,Coordinates &current_suqare,std::function<gameLogic::Coordinates(gameLogic::Coordinates)> &next_square_func , std::function<bool(Coordinates)> &boudires_condition, bool& move_ended, bool enemy_piece, size_t& longest_move, bool &taken, bool king = false);
    Coordinates go_in_direction(gameLogic::Moves &moves,Move &current_move,Coordinates &current_suqare,std::function<gameLogic::Coordinates(gameLogic::Coordinates)> &next_square_func , std::function<bool(Coordinates)> &boudires_condition, bool& move_ended, bool enemy_piece, size_t& longest_move, bool &taken);
    void add_move(gameLogic::Moves &moves,Move &current_move,size_t& longest_move, bool &taken, bool enemy_piece);
    void moves_of_a_piece(gameLogic::Coordinates &piece,size_t &longest_move,gameLogic::Moves &moves, bool &taken,bool king = false);
    void reset_add_move(gameLogic::Moves &moves,Move &current_move,size_t& longest_move);
    void remove_piece(Coordinates cor, Square::Piece &piece);
    void add_piece(Coordinates cor, Square::Piece &piece);

    Square **squares = nullptr;
    size_t squares_size = 64;
    size_t board_side = 8;
    //map of pair to Square::Piece
    std::vector<Coordinates> whiteMen;
    std::vector<Coordinates> whiteKings;
    std::vector<Coordinates> blackMen;
    std::vector<Coordinates> blackKings;
};

#endif // GAMELOGIC_H
