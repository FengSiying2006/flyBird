#include "pipe.h"
#include <QRandomGenerator>
#include <QGraphicsScene>

Pipe::Pipe() : speed(5), gap(180),isPassed(false) {
	// 随机生成管道高度
	topHeight = QRandomGenerator::global()->bounded(100,250);
	bottomHeight = 600 - topHeight - gap;

	// 加载并缩放下管道图片
	bottomPipe = new QGraphicsPixmapItem(QPixmap(":/assets/images/pipe-green.png").scaled(50,bottomHeight, Qt::IgnoreAspectRatio));
	// 加载上管道图片，并进行180度旋转
	QPixmap pixmap(":/assets/images/pipe-green.png");
	QTransform transform;
	transform.translate(pixmap.width() / 2, pixmap.height() / 2);  // 设置旋转中心到图片中心
	transform.rotate(180);  // 旋转180度
	transform.translate(-pixmap.width() / 2, -pixmap.height() / 2);  // 还原位置
	QPixmap rotatedPixmap = pixmap.transformed(transform);
	// 将旋转后的图片缩放并作为上管道
	topPipe = new QGraphicsPixmapItem(rotatedPixmap.scaled(50,topHeight, Qt::IgnoreAspectRatio));
	// 将上、下管道加入到该组中
	addToGroup(topPipe);
	addToGroup(bottomPipe);

	// 设置上管道和下管道的位置
	topPipe->setPos(0, 0);  // 上管道位于(0,0)
	bottomPipe->setPos(0, topHeight + gap);  // 下管道位于上管道的下方，并留有空隙

	// 将整个组移动到屏幕的右侧开始位置
	setPos(400, 0);
}

void Pipe::movePipe() {
	// 管道向左移动
	setX(x() - speed);
}

QRectF Pipe::boundingRect() const {
	// 返回上下管道的联合矩形区域
	QRectF topRect = topPipe->boundingRect().translated(topPipe->pos());
	QRectF bottomRect = bottomPipe->boundingRect().translated(bottomPipe->pos());
	return topRect.united(bottomRect);
}

QPainterPath Pipe::shape() const {
	// 定义碰撞检测区域
	QPainterPath path;
	path.addRect(topPipe->boundingRect().translated(topPipe->pos()));
	path.addRect(bottomPipe->boundingRect().translated(bottomPipe->pos()));
	return path;
}
