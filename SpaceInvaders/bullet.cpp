#include "bullet.h"

Bullet::Bullet(int x, int y, BulletOwner owner)
    : m_x(x), m_y(y), m_owner(owner), m_active(true)
{
    m_vy = (owner == BulletOwner::Player) ? -9 : 5;
}

void Bullet::move()
{
    m_y += m_vy;
}

QRect Bullet::rect() const
{
    return QRect(m_x - WIDTH / 2, m_y, WIDTH, HEIGHT);
}

bool Bullet::isOffScreen(int boardTop, int boardBottom) const
{
    return (m_y + HEIGHT < boardTop) || (m_y > boardBottom);
}
