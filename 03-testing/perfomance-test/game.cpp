#include "game.h"
#include <QKeyEvent>
#include <QGraphicsTextItem>
#include <QIcon>
#include <QDateTime>
#include <QFile>
#include <QTextStream>

Game::Game(QWidget* parent) : QGraphicsView(parent), score(0),isGameOver(false) {
	scene = new QGraphicsScene(this);
	setScene(scene);

    setWindowTitle("Ikun牌小鸟");

    QIcon icon(":/assets/images/bluebird-upflap.png");
    setWindowIcon(icon);

	bird = new Bird();
	scene->addItem(bird);

	// 定时器，控制游戏循环
	timer = new QTimer(this);
	connect(timer, &QTimer::timeout, this, &Game::gameLoop);
	timer->start(20);

	setFixedSize(400, 600);
	scene->setSceneRect(0, 0, 400, 600);

	scene->setBackgroundBrush(QBrush(QImage(":/assets/images/background-day.png").scaled(400, 600)));

	// 取消滚动条
	setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

	// 创建并显示分数文本项
	scoreText = new QGraphicsTextItem(QString("Score: %1").arg(score));
	//放在最前面
	scoreText->setZValue(1);
	scoreText->setDefaultTextColor(Qt::white);
	scoreText->setFont(QFont("Arial", 20));
	scoreText->setPos(10, 10);
	scene->addItem(scoreText);
}

void Game::keyPressEvent(QKeyEvent* event) {
	if (event->key() == Qt::Key_Space) {
		if (isGameOver) {
			restartGame();  // 如果游戏结束，按空格键重置游戏
		}
		else {
			bird->flap();  // 如果游戏在进行，按空格键让小鸟跳跃
		}
	}
}

void Game::restartGame()
{
	// 清除场景中的管道和文本
	for (Pipe* pipe : pipes) {
		scene->removeItem(pipe);
		delete pipe;
	}
	pipes.clear();

	// 重置小鸟的位置和状态
	bird->setPos(100, 300);
	bird->reset();

	// 重置分数
	score = 0;
	scoreText->setPlainText(QString("Score: %1").arg(score));

	// 移除 Game Over 画面
	QList<QGraphicsItem*> items = scene->items();
	for (QGraphicsItem* item : items) {
		if (QGraphicsPixmapItem* pixmapItem = dynamic_cast<QGraphicsPixmapItem*>(item)) 
		{
			if (pixmapItem->pixmap().cacheKey() == QPixmap(":/assets/images/gameover.png").cacheKey()) 
			{
				scene->removeItem(pixmapItem);
				delete pixmapItem;
			}
		}
		if (QGraphicsTextItem* textItem = dynamic_cast<QGraphicsTextItem*>(item)) {
			if (textItem->toPlainText() == "按空格键重新开始") {
				scene->removeItem(textItem);
				delete textItem;
			}
		}	
	}

	// 重置游戏状态
	isGameOver = false;
	timer->start(20);
}

void Game::gameLoop() {
    qreal yBefore = bird->y(); // 记录更新前的高度
	bird->updatePosition();
    qreal yAfter = bird->y();  // 记录更新后的高度

    // --- 性能测试计算开始 ---
    if (waitingForUpdate && yAfter != yBefore) {
        qint64 updateTime = QDateTime::currentMSecsSinceEpoch();
        qint64 latency = updateTime - commandTime;

        QFile file("performance_data1.csv");
        if (file.open(QIODevice::Append | QIODevice::Text)) {
            QTextStream out(&file);
            // 记录格式：时间戳, 延迟(ms)
            out << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss")
                << "," << latency << "\n";
            file.close();
        }

        waitingForUpdate = false; // 重置标记，直到下次跳跃
    }
    // -----------------------
    // 寻找离小鸟最近的一个未通过的管道

    Pipe* targetPipe = nullptr;
    for (Pipe* pipe : pipes) {
        // 如果管道在小鸟右侧（或者刚好在小鸟位置），且还没通过
        if (pipe->x() + pipe->boundingRect().width() > bird->x() && !pipe->isPassed) {
            targetPipe = pipe;
            break;
        }
    }


    if (targetPipe) {
        // 算出管道空隙的中心高度
        // 假设 gap 是 180 (根据你的调试数据)
        // 上管道的高度就是 topHeight
        // 目标Y = 上管道高度 + 间隙的一半
        qreal targetY = targetPipe->getTopHeight() + 180 / 2;

        // 如果小鸟掉到了目标高度以下，就跳一下
        // 加个 20 像素的容错，让它跳得更稳一点

        if (bird->y() > targetY + 20) {
            // --- 性能测试埋点开始 ---
            commandTime = QDateTime::currentMSecsSinceEpoch();
            waitingForUpdate = true;
            // -----------------------
            bird->flap(); // 自动执行跳跃
        }
    } else {
        if (bird->y() > 300) {
            bird->flap();
        }
    }
	// 生成新的管道
	if (pipes.isEmpty() || pipes.last()->x() < 200) {
		Pipe* pipe = new Pipe();
		pipes.append(pipe);
		scene->addItem(pipe);
	}

	// 管道移动与检测碰撞
	auto it = pipes.begin();
	while (it != pipes.end()) {
		Pipe* pipe = *it;
		pipe->movePipe();

		// 检测与小鸟的碰撞
		if (bird->collidesWithItem(pipe)) {
			timer->stop();
			QGraphicsPixmapItem* gameOverItem = scene->addPixmap(QPixmap(":/assets/images/gameover.png"));
			// 将 Game Over 画面放在中间位置
			gameOverItem->setPos(this->width() / 2 - gameOverItem->pixmap().width() / 2, this->height() / 2 - gameOverItem->pixmap().height() / 2);
			isGameOver = true;
            //提示按空格重新游戏，用QGraphicsTextItem
            QGraphicsTextItem* restartText = new QGraphicsTextItem("按空格键重新开始");
			restartText->setDefaultTextColor(Qt::black);
			restartText->setFont(QFont("Arial", 12, QFont::Bold));
			//放在中间
			restartText->setPos(this->width() / 2 - restartText->boundingRect().width() / 2, this->height() / 2 + gameOverItem->pixmap().height() / 2 + 10);
           
            scene->addItem(restartText);
			return;
		}

		// 如果小鸟通过了管道（即小鸟的x坐标刚好超过管道的x坐标）
		if (pipe->x() + pipe->boundingRect().width() < bird->x() && !pipe->isPassed) {
			// 增加分数
			score++;
			pipe->isPassed = true;  // 确保不会重复加分

			// 更新分数显示
			scoreText->setPlainText(QString("Score: %1").arg(score));
		}

		// 如果管道移出了屏幕，将其从场景和列表中删除
		if (pipe->x() < -60) {
			scene->removeItem(pipe);
			delete pipe;
			it = pipes.erase(it);  // 从列表中安全移除管道
		}
		else {
			++it;  // 继续遍历
		}
	}
    frameCount++;
    qint64 currentTime = QDateTime::currentMSecsSinceEpoch();
    qint64 elapsed = currentTime - lastFpsTime;

    if (elapsed >= 1000) { // 每秒计算一次
        double fps = frameCount / (elapsed / 1000.0);

        // 写入 CSV 文件以便后续 Python 绘图
        QFile file("fps_data.csv");
        if (file.open(QIODevice::Append | QIODevice::Text)) {
            QTextStream out(&file);
            out << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss")
                << "," << fps << "\n";
            file.close();
        }

        frameCount = 0;
        lastFpsTime = currentTime;
    }
}
