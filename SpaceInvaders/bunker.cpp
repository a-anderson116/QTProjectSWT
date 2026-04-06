#include "bunker.h"
#include <QRandomGenerator>

// Classic arch-shaped bunker with a notch cut out of the bottom centre.
static const bool SHAPE[Bunker::ROWS][Bunker::COLS] = {
    {0,0,1,1,1,1,1,1,1,1,1,1,1,1,0,0},
    {0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0},
    {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
    {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
    {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
    {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
    {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
    {1,1,1,0,0,0,0,0,0,0,0,0,0,1,1,1},
    {1,1,1,0,0,0,0,0,0,0,0,0,0,1,1,1},
    {1,1,1,0,0,0,0,0,0,0,0,0,0,1,1,1},
};

Bunker::Bunker(int x, int y) : m_x(x), m_y(y)
{
    initShape();
}

void Bunker::initShape()
{
    for (int r = 0; r < ROWS; r++)
        for (int c = 0; c < COLS; c++)
            m_cells[r][c] = SHAPE[r][c];
}

bool Bunker::hit(const QRect& bulletRect)
{
    bool anyHit = false;
    for (int r = 0; r < ROWS; r++) {
        for (int c = 0; c < COLS; c++) {
            if (!m_cells[r][c]) continue;
            QRect cell(m_x + c * CELL, m_y + r * CELL, CELL, CELL);
            if (bulletRect.intersects(cell)) {
                m_cells[r][c] = false;
                anyHit = true;
                // Destroy a small cluster of neighbours for a chunky look
                auto destroy = [&](int nr, int nc) {
                    if (nr >= 0 && nr < ROWS && nc >= 0 && nc < COLS
                            && QRandomGenerator::global()->bounded(2) == 0)
                        m_cells[nr][nc] = false;
                };
                destroy(r - 1, c);
                destroy(r + 1, c);
                destroy(r,     c - 1);
                destroy(r,     c + 1);
            }
        }
    }
    return anyHit;
}

bool Bunker::isCellAlive(int col, int row) const
{
    if (col < 0 || col >= COLS || row < 0 || row >= ROWS) return false;
    return m_cells[row][col];
}
