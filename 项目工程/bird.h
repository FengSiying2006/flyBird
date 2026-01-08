#ifndef BIRD_H
#define BIRD_H

#include <QGraphicsPixmapItem>
#include <QPixmap>

class Bird : public QGraphicsPixmapItem {
public:
    Bird();
    void flap();
    void updatePosition();
    void reset();
    void setSkin(QString color);

private:
    qreal velocity;
    qreal gravity;
    qreal lift;
    QString skinColor;
};

#endif // BIRD_H
