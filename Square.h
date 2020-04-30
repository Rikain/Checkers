#ifndef SQUARE_H
#define SQUARE_H
#include <QGraphicsItem>
#include <QPainter>

class Square : public QGraphicsItem{
public:
    static constexpr int square_size = 60;
    enum Piece{
        Empty,
        White,
        Black,
        WhiteKing,
        BlackKing
    };
    Square(int board_x_in,int board_y_in,QColor color_in,Piece piece_in);
    Square();
    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
protected:
    //void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    //void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override; --better option for emiting imo
protected:
    int board_x = 0;
    int board_y = 0;
    int x() const;
    int y() const;
    Piece piece;
    QColor color;
    bool pressed = false;
};

#endif // SQUARE_H
