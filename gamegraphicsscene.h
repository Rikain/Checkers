#ifndef GAMEGRAPHICSSCENE_H
#define GAMEGRAPHICSSCENE_H
#include <QGraphicsScene>
#include "gamelogic.h"

class gameGraphicsScene : public QGraphicsScene
{
    Q_OBJECT
public:
    static constexpr size_t board_side = 10;
    gameGraphicsScene(QObject *parent = nullptr);
    ~gameGraphicsScene();
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
private:
    QSquare *lastItem = nullptr;
    QSquare *squares[board_side*board_side];
    std::vector<std::pair<gameLogic::Coordinates,Square::Piece>> pieces_stack;
    gameLogic::Moves move_vec;
    gameLogic game;
    size_t move_it = 0;
    void clear_inputs();
    void return_piece_stack();
    void highlight_moves(QSquare* thisItem);
    void highlight_squares();
    void delete_highlights(QSquare* except_thisItem = nullptr);
};



#endif // GAMEGRAPHICSSCENE_H
