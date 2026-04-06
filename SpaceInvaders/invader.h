#ifndef INVADER_H
#define INVADER_H

#include <QRect>

enum class InvaderType { Large, Medium, Small };

class Invader {
public:
    static const int WIDTH  = 36;
    static const int HEIGHT = 26;

    Invader();
    Invader(int col, int row, InvaderType type);

    void setPos(int x, int y);
    void offsetPos(int dx, int dy);
    void toggleFrame();

    QRect       rect()       const;
    int         x()          const { return m_x;     }
    int         y()          const { return m_y;     }
    int         col()        const { return m_col;   }
    int         row()        const { return m_row;   }
    InvaderType type()       const { return m_type;  }
    bool        alive()      const { return m_alive; }
    int         frame()      const { return m_frame; }
    int         pointValue() const;

    void kill() { m_alive = false; }

private:
    int         m_x, m_y;
    int         m_col, m_row;
    InvaderType m_type;
    bool        m_alive;
    int         m_frame;   // 0 or 1 — drives leg/arm animation
};

#endif // INVADER_H
