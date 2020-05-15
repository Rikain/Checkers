#include "square.h"

Square::Square(int board_x_in,int board_y_in,Piece piece_in)
    :board_x(board_x_in),board_y(board_y_in),piece(piece_in)
{
}

std::pair<int, int> Square::coordinates() const
{
    return std::make_pair(board_x,board_y);
}

Square::Piece Square::show_piece() const
{
    return piece;
}

Square::Piece Square::update_piece(Square::Piece piece_in)
{
    Square::Piece temp = piece;
    piece = piece_in;
    return temp;
}
