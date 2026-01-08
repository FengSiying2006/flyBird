#include "bird.h"
#include <QGraphicsScene>

Bird::Bird() : velocity(0), gravity(0.5), lift(-10) {
    skinColor = "bluebird";
    QString path = QString(":/assets/images/%1-midflap.png").arg(skinColor);

    setPixmap(QPixmap(path).scaled(40, 40));
    setPos(100, 300);
}

void Bird::flap() {
    velocity = lift;
    QString path = QString(":/assets/images/%1-upflap.png").arg(skinColor);
    setPixmap(QPixmap(path).scaled(40, 40));
}

void Bird::updatePosition() {
    velocity += gravity;
    setY(y() + velocity);

    if (y() < 0) {
        setY(0);
        velocity = 0;
    }
    else if (y() > 560) {
        setY(560);
        velocity = 0;
    }

    QString path = QString(":/assets/images/%1-downflap.png").arg(skinColor);
    setPixmap(QPixmap(path).scaled(40, 40));
}

void Bird::reset()
{
    skinColor = "bluebird";
    QString path = QString(":/assets/images/%1-midflap.png").arg(skinColor);
    setPixmap(QPixmap(path).scaled(40, 40));
    setPos(100, 300);
}

void Bird::setSkin(QString color) {
    skinColor = color;
}
