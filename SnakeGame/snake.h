#ifndef SNAKE_H
#define SNAKE_H

#include <QList>
#include <QPoint>
#include "direction.h"

class Snake {
public:
    Snake(QPoint startPos, Direction startDir, int initialLength = 3);

    void reset(QPoint startPos, Direction startDir, int initialLength = 3);

    // Returns false on wall collision
    bool move(int boardWidth, int boardHeight);

    void scheduleGrow();
    void setDirection(Direction dir);

    Direction         getDirection() const { return m_direction; }
    QPoint            getHead()      const { return m_body.isEmpty() ? QPoint(-1,-1) : m_body.first(); }
    const QList<QPoint>& getBody()  const { return m_body; }

    bool checkSelfCollision() const;
    bool containsPoint(QPoint p)  const;

    int getScore()  const { return m_score; }
    int getLength() const { return m_body.size(); }
    void addScore(int pts) { m_score += pts; }

private:
    QList<QPoint> m_body;
    Direction     m_direction;
    bool          m_growPending;
    int           m_score;
};

#endif // SNAKE_H
