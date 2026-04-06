#include "mysteryship.h"
#include <QRandomGenerator>

MysteryShip::MysteryShip()
    : m_x(0), m_y(0), m_dir(1), m_active(false)
{}

void MysteryShip::spawn(int boardW, int shipY)
{
    m_dir = (QRandomGenerator::global()->bounded(2) == 0) ? 1 : -1;
    m_x   = (m_dir == 1) ? -WIDTH : boardW;
    m_y   = shipY;
    m_active = true;
}

void MysteryShip::move(int boardW)
{
    m_x += m_dir * SPEED;
    if (m_dir == 1  && m_x > boardW)  m_active = false;
    if (m_dir == -1 && m_x + WIDTH < 0) m_active = false;
}

QRect MysteryShip::rect() const
{
    return QRect(m_x, m_y, WIDTH, HEIGHT);
}

int MysteryShip::pointValue() const
{
    static const int vals[] = {50, 100, 150, 200, 300};
    return vals[QRandomGenerator::global()->bounded(5)];
}
