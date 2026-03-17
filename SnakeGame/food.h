#ifndef FOOD_H
#define FOOD_H

#include <QPoint>
#include <QList>

class Food {
public:
    Food();
    void   randomize(int boardWidth, int boardHeight, const QList<QPoint>& occupied);
    QPoint getPosition() const { return m_position; }

private:
    QPoint m_position;
};

#endif // FOOD_H
