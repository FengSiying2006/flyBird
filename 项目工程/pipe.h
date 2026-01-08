#ifndef PIPE_H
#define PIPE_H

#include <QGraphicsItemGroup>
#include <QGraphicsPixmapItem>

class Pipe : public QGraphicsItemGroup {
public:
	Pipe();
	void movePipe();
	QRectF boundingRect() const override;  // 自定义边界矩形
	QPainterPath shape() const override;   // 自定义形状，便于碰撞检测
    int getTopHeight() const { return topHeight;}

public:
	bool isPassed;
private:
	QGraphicsPixmapItem* topPipe;
	QGraphicsPixmapItem* bottomPipe;
	int speed;
	int gap;
	int topHeight;
	int bottomHeight;
};

#endif // PIPE_H
