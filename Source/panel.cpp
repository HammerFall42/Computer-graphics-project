#include "panel.h"

Panel::Panel(){}

void Panel::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    Q_UNUSED(event)

    emit shapeIsChosen(number);
}
