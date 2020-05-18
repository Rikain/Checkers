#ifndef SQUARE_H
#define SQUARE_H
#include <utility>

class Square{
public:
    enum Piece{
        Empty,
        White,
        Black,
        WhiteKing,
        BlackKing
    };
    Square(int board_x_in,int board_y_in,Piece piece_in);
    Square(std::pair<int,int> coordinates,Piece piece_in);
    std::pair<int,int> coordinates() const;
    Square::Piece show_piece() const;
    Square::Piece update_piece(Piece piece_in);
protected:
    int board_x = 0;
    int board_y = 0;
    Piece piece;
};

#endif // SQUARE_H
