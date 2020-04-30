#include "Square.h"
#include <QtGui>
#include <QtDebug>

Square::Square(int board_x_in,int board_y_in,QColor color_in,Piece piece_in)
    :board_x(board_x_in),board_y(board_y_in),piece(piece_in),color(color_in)
{
    //Square::setZValue(10);
}

Square::Square()
    :piece(Square::Empty),color(Qt::blue)
{
   qDebug("siema");
}

QRectF Square::boundingRect() const
{
    return QRectF(x(),y(),square_size,square_size);
}

void Square::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);

    QRectF rec = boundingRect();
    QBrush brush(color);

    painter->fillRect(rec,brush);
    painter->drawRect(rec);

    QColor piece_colors[2];

    switch(piece){
        case(Square::White):
        case(Square::WhiteKing):
            piece_colors[0] = Qt::white;
            piece_colors[1] = QColor(240,255,255);
            break;
        case(Square::Black):
        case(Square::BlackKing):
            piece_colors[0] = Qt::red;
            piece_colors[1] = QColor(220,20,60);
        default:
            break;
    }

    bool king = false;
    switch(piece){
        case(Square::BlackKing):
        case(Square::WhiteKing):
            king = true;
            break;
        default:
            break;
    }

    if(piece != Square::Empty){
        painter->setBrush(piece_colors[0]);
        painter->drawEllipse(QRectF(x()+ceil(square_size*0.05), y()+ceil(square_size*0.05), ceil(square_size*0.9), ceil(square_size*0.9)));
        painter->setBrush(piece_colors[1]);
        painter->drawEllipse(QRectF(x()+ceil(square_size*0.1), y()+ceil(square_size*0.1), ceil(square_size*0.8), ceil(square_size*0.8)));
        if(king){
            const QPointF kingPoints[7] = { //the king's crown
                QPointF(x()+floor(square_size*0.23), y()+floor(square_size*0.7)),
                QPointF(x()+floor(square_size*0.77), y()+floor(square_size*0.7)),
                QPointF(x()+floor(square_size*0.8), y()+floor(square_size*0.3)),
                QPointF(x()+floor(square_size*0.64), y()+floor(square_size*0.5)),
                QPointF(x()+floor(square_size*0.5), y()+floor(square_size*0.3)),
                QPointF(x()+floor(square_size*0.36), y()+floor(square_size*0.5)),
                QPointF(x()+floor(square_size*0.2), y()+floor(square_size*0.3))
            };
            painter->setBrush(Qt::yellow);
            painter->drawPolygon(kingPoints, 7);
        }
    }

}

int Square::x() const
{
    return board_x*square_size;
}

int Square::y() const
{
    return board_y*square_size;
}

/*
void Square::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    pressed = true;
    update();
    QGraphicsItem::mousePressEvent(event);
}

void Square::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    pressed = false;
    update();
    QGraphicsItem::mouseReleaseEvent(event);
}
*/


    /*
    Q_UNUSED(option);
    Q_UNUSED(widget);

    painter->setBrush(Qt::lightGray);

    painter->drawEllipse(boundingRect());

    painter->setBrush(colour);
    painter->drawEllipse(interior());
    if(king){
        const QPointF kingPoints[7] = { //the king's crown
            QPointF(x+18.0, y+45.0),
            QPointF(x+58.0, y+45.0),
            QPointF(x+60.0, y+15.0),
            QPointF(x+48.0, y+30.0),
            QPointF(x+38.0, y+15.0),
            QPointF(x+28.0, y+30.0),
            QPointF(x+16.0, y+15.0)
        };
        painter->setBrush(Qt::darkYellow);
        painter->drawPolygon(kingPoints, 7);
    }
    painter->setBrush(Qt::black);
    */

