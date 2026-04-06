#ifndef MYSTERYSHIP_H
#define MYSTERYSHIP_H

#include <QRect>

class MysteryShip {
public:
    static const int WIDTH  = 60;
    static const int HEIGHT = 22;
    static const int SPEED  = 3;

    MysteryShip();

    void spawn(int boardW, int shipY);
    void move(int boardW);

    bool isActive()  const { return m_active; }
    void deactivate()      { m_active = false; }

    QRect rect()     const;
    int   x()        const { return m_x; }
    int   y()        const { return m_y; }
    int   direction() const { return m_dir; }

    // Returns a random bonus value (50 / 100 / 150 / 200 / 300)
    int pointValue() const;

private:
    int  m_x, m_y;
    int  m_dir;    // +1 = right, -1 = left
    bool m_active;
};

#endif // MYSTERYSHIP_H
