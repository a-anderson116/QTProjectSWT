#include "food.h"
#include <QRandomGenerator>

Food::Food() : m_position(5, 5) {}

void Food::randomize(int boardWidth, int boardHeight, const QList<QPoint>& occupied) {
    QPoint pos;
    int attempts = 0;
    const int maxAttempts = boardWidth * boardHeight;
    do {
        pos = QPoint(QRandomGenerator::global()->bounded(boardWidth),
                     QRandomGenerator::global()->bounded(boardHeight));
        ++attempts;
    } while (occupied.contains(pos) && attempts < maxAttempts);
    m_position = pos;
}
