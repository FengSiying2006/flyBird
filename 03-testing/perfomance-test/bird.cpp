#include "bird.h"
#include <QGraphicsScene>
Bird::Bird() : velocity(0), gravity(0.5), lift(-10) {
	setPixmap(QPixmap(":/assets/images/bluebird-midflap.png").scaled(40, 40));
	setPos(100, 300);
}

void Bird::flap() {
	velocity = lift;
	setPixmap(QPixmap(":/assets/images/bluebird-upflap.png").scaled(40, 40));
}

void Bird::updatePosition() {
	velocity += gravity;
	setY(y() + velocity);

	// 限制鸟的活动范围
	if (y() < 0) {
		setY(0);
		velocity = 0;
	}
	else if (y() > 560) {
		setY(560);
		velocity = 0;
	}

	setPixmap(QPixmap(":/assets/images/bluebird-downflap.png").scaled(40, 40));
}

void Bird::reset()
{
	setPixmap(QPixmap(":/assets/images/bluebird-midflap.png").scaled(40, 40));
	setPos(100, 300);
}
