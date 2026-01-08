#ifndef GAME_H
#define GAME_H

#include <QGraphicsView>
#include <QGraphicsScene>
#include <QTimer>
#include "bird.h"
#include "pipe.h"

class Game : public QGraphicsView {
	Q_OBJECT
public:
	Game(QWidget* parent = nullptr);
	void keyPressEvent(QKeyEvent* event);
	void restartGame();
private slots:
	void gameLoop();

private:
	QGraphicsScene* scene;
	QGraphicsTextItem* scoreText;
	Bird* bird;
	QTimer* timer;
	QList<Pipe*> pipes;
	int score;
	bool isGameOver;
    qint64 commandTime = 0;   // 记录指令发出的时间
    bool waitingForUpdate = false; // 标记是否正在等待位置更新
    int frameCount = 0;
    qint64 lastFpsTime = 0;
};

#endif // GAME_H
