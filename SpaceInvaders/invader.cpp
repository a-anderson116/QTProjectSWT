#include "invader.h"

Invader::Invader()
    : m_x(0), m_y(0), m_col(0), m_row(0)
    , m_type(InvaderType::Large), m_alive(false), m_frame(0)
{}

Invader::Invader(int col, int row, InvaderType type)
    : m_x(0), m_y(0), m_col(col), m_row(row)
    , m_type(type), m_alive(true), m_frame(0)
{}

void Invader::setPos(int x, int y)
{
    m_x = x;
    m_y = y;
}

void Invader::offsetPos(int dx, int dy)
{
    m_x += dx;
    m_y += dy;
}

void Invader::toggleFrame()
{
    m_frame = 1 - m_frame;
}

QRect Invader::rect() const
{
    return QRect(m_x, m_y, WIDTH, HEIGHT);
}

int Invader::pointValue() const
{
    switch (m_type) {
    case InvaderType::Small:  return 30;
    case InvaderType::Medium: return 20;
    case InvaderType::Large:  return 10;
    }
    return 10;
}
