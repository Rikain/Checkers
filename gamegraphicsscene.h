#ifndef GAMEGRAPHICSSCENE_H
#define GAMEGRAPHICSSCENE_H
#include <QGraphicsScene>
//#include "gamelogic.h"
#include "gtsalgortihms.h"

class gameGraphicsScene : public QGraphicsScene
{
    Q_OBJECT
public:
    static constexpr size_t board_side = 8;
    gameGraphicsScene(int AI_height = 0, QObject *parent = nullptr);
    ~gameGraphicsScene();
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void rollback_last_move();
private:
    AI* computer_player = nullptr;
    std::stack<gameLogic::Move> played_moves;
    QSquare *lastItem = nullptr;
    QSquare *squares[board_side*board_side];
    std::vector<std::pair<gameLogic::Coordinates,Square::Piece>> pieces_stack;
    gameLogic::Moves move_vec;
    gameLogic *game = nullptr;
    size_t move_it = 0;
    void new_first_click(QSquare* thisItem);
    void clear_inputs();
    void return_piece_stack();
    void highlight_moves(QSquare* thisItem);
    void highlight_squares();
    void delete_highlights(QSquare* except_thisItem = nullptr);
    bool check_if_ended();
    void end_the_game();
    void computer_move();
};



#endif // GAMEGRAPHICSSCENE_H
