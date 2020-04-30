#ifndef GAMEPLAYWINDOW_H
#define GAMEPLAYWINDOW_H

#include <QWidget>
#include "Square.h"

namespace Ui {
class gameplayWindow;
}

class gameplayWindow : public QWidget
{
    Q_OBJECT

public:
    explicit gameplayWindow(QWidget *parent = nullptr);
    ~gameplayWindow();
public:
    void start_a_new_game();
private:
    Ui::gameplayWindow *ui;
    QGraphicsScene *scene;
    static constexpr size_t board_side = 10;
    Square *squares[board_side*board_side];
    void make_new_board();
    //connect(this, &GraphicsScene::SelectionChanged, this, &Scene::Test);
    //i can implement a reciver
    //ten reciver by odbierał signal od kwadratu przechowywał odwołanie do ostatnio kilniętego kwadratu
    //porównywał czy to możliwy ruch i jak tak/nie to eimotwał do tych kwadratów signale żeby
    //zmieniły się w jakiś sposób w ten sam sposób można by podświeltnie robic itp.
    //tylko ten reciver musiałby miec jaki bool playermove kttóry też signalami byłby zmieniany
    //i  tlyko jak byłby true to zmieniałby board od klinięć
    //selectionChanged() jako emit od sceny i lista selectedItems() jako sprawdzanie czy walid selkcja
    //jak nie to clear a jak tak to zostaje potem sprawdzasz czy para jest valid
    /*
     * class Receiver : public QObject
       {
       Q_OBJECT
       public:
         Receiver() : QObject()
         {}

       public slots:
         void test()
         {
           QMessageBox::information(0, "This", "Is working");
         }
       };
       QGraphicsScene sc(0,0,1055,768);
       Receiver r;
       QGraphicsView v(&sc);
       QObject::connect(&sc, SIGNAL(selectionChanged()), &r, SLOT(test()));

    */
};

#endif // GAMEPLAYWINDOW_H
