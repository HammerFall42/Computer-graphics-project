#ifndef PANEL_H
#define PANEL_H

#include <QGraphicsScene>
#include <QMouseEvent>
#include <QObject>

class Panel : public QGraphicsScene
{
    Q_OBJECT

public:
    Panel();
    ~Panel() {}

    void setNumber(int num) {number = num;}

    bool isSelected();

signals:
    void shapeIsChosen(int num);

private:    
    void mousePressEvent(QGraphicsSceneMouseEvent *event);

    int number;
};

#endif // PANEL_H
