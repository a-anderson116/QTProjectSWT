#include "snake.h"

Snake::Snake(QPoint startPos, Direction startDir, int initialLength)
    : m_direction(startDir), m_growPending(false), m_score(0)
{
    reset(startPos, startDir, initialLength);
}

void Snake::reset(QPoint startPos, Direction startDir, int initialLength) {
    m_body.clear();
    m_direction   = startDir;
    m_growPending = false;
    m_score       = 0;

    for (int i = 0; i < initialLength; ++i) {
        QPoint seg;
        switch (startDir) {
            case Direction::Right: seg = QPoint(startPos.x() - i, startPos.y()); break;
            case Direction::Left:  seg = QPoint(startPos.x() + i, startPos.y()); break;
            case Direction::Down:  seg = QPoint(startPos.x(), startPos.y() - i); break;
            case Direction::Up:    seg = QPoint(startPos.x(), startPos.y() + i); break;
            default:               seg = QPoint(startPos.x() - i, startPos.y()); break;
        }
        m_body.append(seg);
    }
}

bool Snake::move(int boardWidth, int boardHeight) {
    if (m_body.isEmpty()) return false;

    QPoint newHead = m_body.first();
    switch (m_direction) {
        case Direction::Up:    newHead.ry()--; break;
        case Direction::Down:  newHead.ry()++; break;
        case Direction::Left:  newHead.rx()--; break;
        case Direction::Right: newHead.rx()++; break;
        default: break;
    }

    if (newHead.x() < 0 || newHead.x() >= boardWidth ||
        newHead.y() < 0 || newHead.y() >= boardHeight)
        return false;

    m_body.prepend(newHead);
    if (m_growPending) {
        m_growPending = false;
    } else {
        m_body.removeLast();
    }
    return true;
}

void Snake::scheduleGrow() {
    m_growPending = true;
}

void Snake::setDirection(Direction dir) {
    if (dir != Direction::None && !areOpposite(dir, m_direction))
        m_direction = dir;
}

bool Snake::checkSelfCollision() const {
    if (m_body.size() < 2) return false;
    const QPoint& head = m_body.first();
    for (int i = 1; i < m_body.size(); ++i)
        if (m_body[i] == head) return true;
    return false;
}

bool Snake::containsPoint(QPoint p) const {
    return m_body.contains(p);
}
