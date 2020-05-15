#include "qsquare.h"
#include <QtGui>
#include <QtDebug>

QSquare::QSquare(int board_x_in, int board_y_in, Square::Piece piece_in, QColor color_in, QGraphicsItem *parent)
    :QGraphicsItem(parent),Square(board_x_in,board_y_in,piece_in),color(color_in)
{
    //QSquare::setZValue(10);
    this->setFlag(QGraphicsItem::ItemIsSelectable);
}

QRectF QSquare::boundingRect() const
{
    return QRectF(x(),y(),square_size,square_size);
}

void QSquare::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);

    QRectF rec = boundingRect();
    QBrush brush(color);
    QPen pen(color);

    switch(effect){
        case(HighlightCicrle):
        {
        switch(piece){
        case(Square::Empty):
        {
            painter->fillRect(rec,brush);
            painter->drawRect(rec);

            QBrush e_brush(Qt::DiagCrossPattern);
            e_brush.setColor(Qt::cyan);
            QPen e_pen(Qt::cyan);
            //e_pen.setStyle(Qt::DashLine);
            painter->setBrush(e_brush);
            painter->setPen(e_pen);
            painter->drawEllipse(QRectF(x()+ceil(square_size*0.05), y()+ceil(square_size*0.05), ceil(square_size*0.9), ceil(square_size*0.9)));

            painter->setBrush(brush);
            painter->drawEllipse(QRectF(x()+ceil(square_size*0.1), y()+ceil(square_size*0.1), ceil(square_size*0.8), ceil(square_size*0.8)));
        }
            break;
        default:
        {
            brush.setColor(Qt::green);

            painter->fillRect(rec,brush);
            painter->drawRect(rec);
        }
            break;
        }


        }
            break;
        default:
            painter->fillRect(rec,brush);
            painter->drawRect(rec);
            break;
    }

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
        //assert(effect == Nothing);
        painter->setPen(pen);
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

int QSquare::x() const
{
    return board_x*square_size;
}

int QSquare::y() const
{
    return board_y*square_size;
}

void QSquare::switch_highlight()
{
    if(effect == Nothing){
        effect = HighlightCicrle;
        //assert(piece == Piece::Empty);
    }else if(effect == HighlightCicrle){
        effect = Nothing;
    }
}

void QSquare::add_highlight()
{
    effect = HighlightCicrle;
}

void QSquare::delete_highlight()
{
    effect = Nothing;
}

bool QSquare::is_highlighted()
{
    if(effect == HighlightCicrle){
        return true;
    }else{
        return false;
    }
}

bool QSquare::correct_board_cor(std::pair<int, int> pos)
{
    return (pos.first == board_x && pos.second == board_y);
}
