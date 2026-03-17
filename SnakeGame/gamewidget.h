#ifndef GAMEWIDGET_H
#define GAMEWIDGET_H

#include <QWidget>
#include <QTimer>
#include "snake.h"
#include "food.h"
#include "direction.h"
#include "gamemode.h"

class GameWidget : public QWidget {
    Q_OBJECT
public:
    static const int CELL_SIZE    = 20;
    static const int BOARD_WIDTH  = 30;   // cells → 600 px
    static const int BOARD_HEIGHT = 28;   // cells → 560 px
    static const int SCORE_BAR    = 40;   // px above board

    explicit GameWidget(QWidget* parent = nullptr);
    ~GameWidget();

    void startGame(GameMode mode, Difficulty difficulty);
    void togglePause();
    bool isPaused()  const { return m_paused; }
    bool isRunning() const { return m_gameRunning; }

    QSize sizeHint() const override {
        return QSize(BOARD_WIDTH * CELL_SIZE,
                     BOARD_HEIGHT * CELL_SIZE + SCORE_BAR);
    }

signals:
    void gameOver(int score1, int score2, const QString& winner, GameMode mode);
    void restartRequested();
    void menuRequested();
    void pauseStateChanged(bool paused);

protected:
    void paintEvent(QPaintEvent* event) override;
    void keyPressEvent(QKeyEvent* event) override;

private slots:
    void onTimerTick();

private:
    Snake*    m_snake1;
    Snake*    m_snake2;
    Food      m_food;
    QTimer    m_timer;
    GameMode  m_gameMode;
    Difficulty m_difficulty;
    bool      m_gameRunning;
    bool      m_paused;
    bool      m_gameOver;
    QString   m_winner;

    Direction m_pendingDir1;
    Direction m_pendingDir2;

    void checkFoodCollisions();
    void handleDeath(bool s1Dead, bool s2Dead);
    void spawnFood();
    QList<QPoint> allOccupied() const;
    int  timerInterval() const;

    void drawBackground(QPainter& p) const;
    void drawFood(QPainter& p)       const;
    void drawSnake(QPainter& p, const Snake& s,
                   QColor body, QColor head)    const;
    void drawScoreBar(QPainter& p)   const;
    void drawPauseOverlay(QPainter& p) const;
    void drawGameOverOverlay(QPainter& p) const;
};

#endif // GAMEWIDGET_H
