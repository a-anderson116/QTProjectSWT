#ifndef BUNKER_H
#define BUNKER_H

#include <QRect>
#include <QVector>

class Bunker {
public:
    static const int CELL  = 5;   // px per cell
    static const int COLS  = 16;
    static const int ROWS  = 10;
    static const int WIDTH  = COLS * CELL;   // 80
    static const int HEIGHT = ROWS * CELL;   // 50

    explicit Bunker(int x, int y);

    // Returns true if any cells were destroyed (bullet should stop).
    bool hit(const QRect& bulletRect);

    bool isCellAlive(int col, int row) const;
    int  x() const { return m_x; }
    int  y() const { return m_y; }

private:
    int  m_x, m_y;
    bool m_cells[ROWS][COLS];

    void initShape();
};

#endif // BUNKER_H
