#ifndef BULLET_H
#define BULLET_H

#include <QRect>

enum class BulletOwner { Player, Invader };

class Bullet {
public:
    static const int WIDTH  = 3;
    static const int HEIGHT = 14;

    Bullet(int x, int y, BulletOwner owner);

    void  move();
    QRect rect()       const;
    bool  isOffScreen(int boardTop, int boardBottom) const;

    BulletOwner owner()  const { return m_owner;  }
    bool        active() const { return m_active; }
    void        deactivate()   { m_active = false; }

private:
    int         m_x, m_y;
    int         m_vy;
    BulletOwner m_owner;
    bool        m_active;
};

#endif // BULLET_H
