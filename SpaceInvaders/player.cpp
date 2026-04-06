#include "player.h"

Player::Player()
    : m_x(0), m_y(0), m_lives(MAX_LIVES), m_score(0)
{}

void Player::reset(int boardW, int playerY)
{
    m_x     = boardW / 2 - WIDTH / 2;
    m_y     = playerY;
    m_lives = MAX_LIVES;
    m_score = 0;
}

void Player::reposition(int boardW, int playerY)
{
    m_x = boardW / 2 - WIDTH / 2;
    m_y = playerY;
}

void Player::moveLeft(int margin)
{
    if (m_x > margin)
        m_x -= SPEED;
}

void Player::moveRight(int boardW, int margin)
{
    if (m_x + WIDTH < boardW - margin)
        m_x += SPEED;
}

QRect Player::rect() const
{
    return QRect(m_x, m_y, WIDTH, HEIGHT);
}
