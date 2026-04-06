#ifndef PLAYER_H
#define PLAYER_H

#include <QRect>

class Player {
public:
    static const int WIDTH     = 52;
    static const int HEIGHT    = 24;
    static const int SPEED     = 5;
    static const int MAX_LIVES = 3;

    Player();

    void reset(int boardW, int playerY);
    void reposition(int boardW, int playerY);
    void moveLeft(int margin = 10);
    void moveRight(int boardW, int margin = 10);

    QRect rect() const;
    int   x()      const { return m_x; }
    int   y()      const { return m_y; }
    int   lives()  const { return m_lives; }
    int   score()  const { return m_score; }

    void addScore(int pts) { m_score += pts; }
    void loseLife()        { if (m_lives > 0) m_lives--; }
    bool isDead()  const   { return m_lives <= 0; }

private:
    int m_x, m_y;
    int m_lives;
    int m_score;
};

#endif // PLAYER_H
