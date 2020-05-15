#ifndef QSQUARE_H
#define QSQUARE_H
#include "square.h"
#include <QGraphicsItem>
#include <QPainter>

//make this QSquare that inherits from Square which won't hold this trash info about Qt
class QSquare : public QGraphicsItem, public Square{
public:
    static constexpr int square_size = 60;
    QSquare(int board_x_in,int board_y_in, Square::Piece piece_in,QColor color_in, QGraphicsItem *parent = nullptr);
    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
    int x() const;
    int y() const;
    void switch_highlight();
    void add_highlight();
    void delete_highlight();
    bool is_highlighted();
    bool correct_board_cor(std::pair<int,int> pos);
protected:
    QColor color;
    enum QSquareEffects{
        Nothing,
        HighlightCicrle
    };
    QSquareEffects effect = Nothing;
};

#endif // QSQUARE_H
