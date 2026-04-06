#include "gamewidget.h"

#include <QPainter>
#include <QKeyEvent>
#include <QFont>
#include <QRandomGenerator>
#include <algorithm>
#include <cmath>

// ─────────────────────────────────────────────────────────────────────────────
// 8×8 pixel-art sprites (two animation frames each).
// Each uint8_t stores one row, MSB = leftmost pixel.
// ─────────────────────────────────────────────────────────────────────────────

// Small alien — squid / UFO shape (top row, 30 pts)
static const uint8_t SPR_SMALL[2][8] = {
    {0x18,0x3C,0xFF,0xDB,0xFF,0x24,0x5A,0xA5},
    {0x18,0x3C,0xFF,0xDB,0xFF,0x24,0xA5,0x5A}
};

// Medium alien — crab shape (rows 1–2, 20 pts)
static const uint8_t SPR_MEDIUM[2][8] = {
    {0x81,0x42,0xFF,0xBD,0xFF,0xFF,0x24,0x42},
    {0x81,0x42,0xFF,0xBD,0xFF,0xFF,0x42,0x24}
};

// Large alien — octopus shape (rows 3–4, 10 pts)
static const uint8_t SPR_LARGE[2][8] = {
    {0x3C,0x7E,0xFF,0xE7,0xFF,0x7E,0x42,0xA5},
    {0x3C,0x7E,0xFF,0xE7,0xFF,0x7E,0xA5,0x42}
};

// Mystery ship (8×5)
static const uint8_t SPR_MYSTERY[5] = {
    0x3C, 0xFF, 0xDB, 0xFF, 0x3C
};

// Player ship (10×6) — wider sprite uses 10-bit mask stored MSB in uint16_t
static const uint8_t SPR_PLAYER[6] = {
    0x18, 0x18, 0x7E, 0xFF, 0xFF, 0xFF
};

// Explosion (8×8)
static const uint8_t SPR_EXPLODE[8] = {
    0x81,0x42,0x24,0x18,0x18,0x24,0x42,0x81
};

// ─────────────────────────────────────────────────────────────────────────────
// GameWidget
// ─────────────────────────────────────────────────────────────────────────────

GameWidget::GameWidget(QWidget* parent)
    : QWidget(parent)
    , m_wave(0)
    , m_gameRunning(false), m_paused(false)
    , m_gameOver(false), m_levelComplete(false), m_levelTimer(0)
    , m_stepDir(1), m_stepTimer(1), m_stepInterval(STEP_INT_MAX), m_animFrame(0)
    , m_shootTimer(SHOOT_INT_BASE), m_shootInterval(SHOOT_INT_BASE)
    , m_mysteryTimer(MYSTERY_MIN_TICKS)
    , m_leftHeld(false), m_rightHeld(false), m_fireHeld(false), m_fireCooldown(0)
    , m_waitingName(false)
    , m_nameInput(nullptr)
{
    setFocusPolicy(Qt::StrongFocus);
    setFixedSize(BOARD_W, BOARD_H);
    setStyleSheet("background-color: black;");

    m_nameInput = new QLineEdit(this);
    m_nameInput->setMaxLength(12);
    m_nameInput->setFont(QFont("Courier New", 14));
    m_nameInput->setStyleSheet(
        "QLineEdit { background: #000030; color: #00FF00;"
        "border: 2px solid #00FF00; padding: 4px; }");
    m_nameInput->hide();
    connect(m_nameInput, &QLineEdit::returnPressed, this, &GameWidget::onNameReturn);

    connect(&m_timer, &QTimer::timeout, this, &GameWidget::onTick);
    m_timer.setInterval(16); // ~60 fps
}

GameWidget::~GameWidget() {}

// ─────────────────────────────────────────────────────────────────────────────
// Public interface
// ─────────────────────────────────────────────────────────────────────────────

void GameWidget::startGame()
{
    m_wave         = 1;
    m_gameOver     = false;
    m_levelComplete= false;
    m_paused       = false;
    m_leftHeld = m_rightHeld = m_fireHeld = false;
    m_fireCooldown = 0;
    m_explosions.clear();
    m_bonusTexts.clear();
    m_bullets.clear();

    m_player.reset(BOARD_W, PLAYER_Y);
    buildBunkers();
    initWave();

    m_mystery      = MysteryShip();
    m_mysteryTimer = MYSTERY_MIN_TICKS +
                     QRandomGenerator::global()->bounded(MYSTERY_RNG_TICKS);

    m_gameRunning = true;
    m_timer.start();
    setFocus();
}

void GameWidget::togglePause()
{
    if (!m_gameRunning || m_gameOver) return;
    m_paused = !m_paused;
    if (m_paused) m_timer.stop(); else m_timer.start();
    emit pauseStateChanged(m_paused);
    update();
}

void GameWidget::beginNameEntry(const QString& prompt)
{
    m_waitingName = true;
    m_namePrompt  = prompt;
    m_nameInput->clear();
    m_nameInput->show();
    positionNameInput();
    m_nameInput->setFocus();
    update();
}

// ─────────────────────────────────────────────────────────────────────────────
// Initialisation helpers
// ─────────────────────────────────────────────────────────────────────────────

void GameWidget::initWave()
{
    m_invaders.clear();

    int gridStartY = SCORE_H + 30 + (m_wave - 1) * 10;
    gridStartY = qMin(gridStartY, SCORE_H + 80); // cap so grid never starts too low

    for (int r = 0; r < GRID_ROWS; r++) {
        InvaderType type = (r == 0) ? InvaderType::Small
                         : (r <= 2) ? InvaderType::Medium
                                    : InvaderType::Large;
        for (int c = 0; c < GRID_COLS; c++) {
            Invader inv(c, r, type);
            int x = GRID_START_X + c * INV_SPAC_X + (INV_SPAC_X - Invader::WIDTH)  / 2;
            int y = gridStartY   + r * INV_SPAC_Y + (INV_SPAC_Y - Invader::HEIGHT) / 2;
            inv.setPos(x, y);
            m_invaders.append(inv);
        }
    }

    m_stepDir      = 1;
    m_stepInterval = qMax(STEP_INT_MIN, STEP_INT_MAX - (m_wave - 1) * 4);
    m_stepTimer    = m_stepInterval;
    m_animFrame    = 0;

    m_shootInterval = qMax(SHOOT_INT_MIN, SHOOT_INT_BASE - (m_wave - 1) * 6);
    m_shootTimer    = m_shootInterval;
}

void GameWidget::buildBunkers()
{
    m_bunkers.clear();
    const int bunkerY = PLAYER_Y - Bunker::HEIGHT - 20;
    const int totalW  = 4 * Bunker::WIDTH;
    const int gap     = (BOARD_W - totalW) / 5;
    for (int i = 0; i < 4; i++) {
        int bx = gap + i * (Bunker::WIDTH + gap);
        m_bunkers.append(Bunker(bx, bunkerY));
    }
}

// ─────────────────────────────────────────────────────────────────────────────
// Main game tick
// ─────────────────────────────────────────────────────────────────────────────

void GameWidget::onTick()
{
    if (!m_gameRunning || m_paused || m_gameOver || m_waitingName) return;

    // Level-complete cooldown
    if (m_levelComplete) {
        m_levelTimer--;
        if (m_levelTimer <= 0) {
            m_levelComplete = false;
            m_wave++;
            m_bullets.clear();
            m_explosions.clear();
            m_bonusTexts.clear();
            buildBunkers();
            initWave();
        }
        update();
        return;
    }

    // ── Player movement ───────────────────────────────────────────────────────
    if (m_leftHeld)  m_player.moveLeft();
    if (m_rightHeld) m_player.moveRight(BOARD_W);

    // ── Fire cooldown ─────────────────────────────────────────────────────────
    if (m_fireCooldown > 0) m_fireCooldown--;
    if (m_fireHeld && m_fireCooldown == 0) {
        // Max 1 player bullet on screen at once
        int playerBullets = 0;
        for (const auto& b : m_bullets)
            if (b.active() && b.owner() == BulletOwner::Player) playerBullets++;
        if (playerBullets == 0) {
            int bx = m_player.x() + Player::WIDTH / 2;
            int by = m_player.y();
            m_bullets.append(Bullet(bx, by, BulletOwner::Player));
            m_fireCooldown = 12;
        }
    }

    // ── Move bullets ─────────────────────────────────────────────────────────
    for (auto& b : m_bullets) {
        if (!b.active()) continue;
        b.move();
        if (b.isOffScreen(SCORE_H, BOARD_H)) b.deactivate();
    }

    // ── Invader step ─────────────────────────────────────────────────────────
    m_stepTimer--;
    if (m_stepTimer <= 0) {
        stepInvaders();
        m_stepTimer = m_stepInterval;
        m_animFrame = 1 - m_animFrame;
    }

    // ── Invader shoot ─────────────────────────────────────────────────────────
    m_shootTimer--;
    if (m_shootTimer <= 0) {
        invaderShoot();
        m_shootTimer = m_shootInterval;
    }

    // ── Mystery ship ──────────────────────────────────────────────────────────
    if (m_mystery.isActive()) {
        m_mystery.move(BOARD_W);
    } else {
        m_mysteryTimer--;
        if (m_mysteryTimer <= 0) {
            m_mystery.spawn(BOARD_W, SCORE_H + 4);
            m_mysteryTimer = MYSTERY_MIN_TICKS +
                             QRandomGenerator::global()->bounded(MYSTERY_RNG_TICKS);
        }
    }

    // ── Collisions ────────────────────────────────────────────────────────────
    checkCollisions();
    checkWinLose();

    // ── Age effects ───────────────────────────────────────────────────────────
    for (auto it = m_explosions.begin(); it != m_explosions.end();) {
        it->timer--;
        if (it->timer <= 0) it = m_explosions.erase(it); else ++it;
    }
    for (auto it = m_bonusTexts.begin(); it != m_bonusTexts.end();) {
        it->timer--;
        it->y -= 1;
        if (it->timer <= 0) it = m_bonusTexts.erase(it); else ++it;
    }

    // ── Clean up inactive bullets ─────────────────────────────────────────────
    m_bullets.erase(
        std::remove_if(m_bullets.begin(), m_bullets.end(),
                       [](const Bullet& b){ return !b.active(); }),
        m_bullets.end());

    update();
}

// ─────────────────────────────────────────────────────────────────────────────
// Invader movement
// ─────────────────────────────────────────────────────────────────────────────

void GameWidget::stepInvaders()
{
    // Move all invaders laterally
    for (auto& inv : m_invaders)
        if (inv.alive()) inv.offsetPos(m_stepDir * STEP_SIZE, 0);

    // Check for wall collision
    bool hitRight = false, hitLeft = false;
    for (const auto& inv : m_invaders) {
        if (!inv.alive()) continue;
        if (inv.x() + Invader::WIDTH >= BOARD_W - 10) hitRight = true;
        if (inv.x() <= 10)                            hitLeft  = true;
    }

    if (hitRight || hitLeft) {
        m_stepDir = -m_stepDir;
        for (auto& inv : m_invaders)
            if (inv.alive()) inv.offsetPos(m_stepDir * STEP_SIZE, DROP_AMT);
    }

    updateDifficulty();
}

void GameWidget::updateDifficulty()
{
    int alive = aliveCount();
    int total = GRID_ROWS * GRID_COLS; // 55
    // Linear interpolation between MAX and MIN interval
    m_stepInterval = STEP_INT_MIN +
                     (alive * (STEP_INT_MAX - STEP_INT_MIN)) / total;
    m_stepInterval = qMax(m_stepInterval, STEP_INT_MIN);

    m_shootInterval = SHOOT_INT_MIN +
                      (alive * (SHOOT_INT_BASE - SHOOT_INT_MIN)) / total;
    m_shootInterval = qMax(m_shootInterval, SHOOT_INT_MIN);
}

// ─────────────────────────────────────────────────────────────────────────────
// Invader shooting — only the bottom-most live invader in each column fires
// ─────────────────────────────────────────────────────────────────────────────

void GameWidget::invaderShoot()
{
    // Collect bottom-most alive invader for each column
    QVector<const Invader*> shooters;
    for (int c = 0; c < GRID_COLS; c++) {
        const Invader* lowest = nullptr;
        for (const auto& inv : m_invaders) {
            if (!inv.alive() || inv.col() != c) continue;
            if (!lowest || inv.row() > lowest->row()) lowest = &inv;
        }
        if (lowest) shooters.append(lowest);
    }
    if (shooters.isEmpty()) return;

    // Pick a random shooter
    auto* shooter = shooters[QRandomGenerator::global()->bounded(shooters.size())];
    int bx = shooter->x() + Invader::WIDTH / 2;
    int by = shooter->y() + Invader::HEIGHT;

    // Limit invader bullets on screen
    int invBullets = 0;
    for (const auto& b : m_bullets)
        if (b.active() && b.owner() == BulletOwner::Invader) invBullets++;

    int maxInvBullets = qMin(3, 1 + (m_wave / 2));
    if (invBullets < maxInvBullets)
        m_bullets.append(Bullet(bx, by, BulletOwner::Invader));
}

// ─────────────────────────────────────────────────────────────────────────────
// Collision detection
// ─────────────────────────────────────────────────────────────────────────────

void GameWidget::checkCollisions()
{
    for (auto& bullet : m_bullets) {
        if (!bullet.active()) continue;

        if (bullet.owner() == BulletOwner::Player) {
            // vs invaders
            for (auto& inv : m_invaders) {
                if (!inv.alive()) continue;
                if (bullet.rect().intersects(inv.rect())) {
                    inv.kill();
                    bullet.deactivate();
                    m_player.addScore(inv.pointValue());
                    m_explosions.append({inv.x() + Invader::WIDTH/2,
                                         inv.y() + Invader::HEIGHT/2, 25, 18});
                    break;
                }
            }
            // vs mystery ship
            if (bullet.active() && m_mystery.isActive() &&
                    bullet.rect().intersects(m_mystery.rect())) {
                int pts = m_mystery.pointValue();
                m_player.addScore(pts);
                m_bonusTexts.append({m_mystery.x(), m_mystery.y(),
                                     60, QString("+%1").arg(pts)});
                m_explosions.append({m_mystery.x() + MysteryShip::WIDTH/2,
                                     m_mystery.y() + MysteryShip::HEIGHT/2, 35, 28});
                m_mystery.deactivate();
                bullet.deactivate();
            }
        } else { // Invader bullet
            // vs player
            if (bullet.rect().intersects(m_player.rect())) {
                bullet.deactivate();
                m_player.loseLife();
                m_explosions.append({m_player.x() + Player::WIDTH/2,
                                     m_player.y() + Player::HEIGHT/2, 50, 30});
                if (m_player.isDead()) {
                    m_gameOver    = true;
                    m_gameRunning = false;
                    m_timer.stop();
                    emit gameOver(m_player.score());
                    update();
                    return;
                } else {
                    // Re-centre ship without touching lives or score
                    m_player.reposition(BOARD_W, PLAYER_Y);
                }
            }
        }

        // Both bullet types vs bunkers
        if (bullet.active()) {
            for (auto& bunk : m_bunkers) {
                if (bunk.hit(bullet.rect())) {
                    bullet.deactivate();
                    break;
                }
            }
        }
    }
}

// ─────────────────────────────────────────────────────────────────────────────
// Win / Lose condition checks
// ─────────────────────────────────────────────────────────────────────────────

void GameWidget::checkWinLose()
{
    if (m_gameOver) return;

    // All invaders cleared → next wave
    if (aliveCount() == 0) {
        m_levelComplete = true;
        m_levelTimer    = 180; // ~3 seconds
        return;
    }

    // Any invader has reached the ground (GROUND_Y) → immediate game over
    for (const auto& inv : m_invaders) {
        if (!inv.alive()) continue;
        if (inv.y() + Invader::HEIGHT >= GROUND_Y) {
            m_gameOver    = true;
            m_gameRunning = false;
            m_timer.stop();
            emit gameOver(m_player.score());
            update();
            return;
        }
    }
}

// ─────────────────────────────────────────────────────────────────────────────
// Utility
// ─────────────────────────────────────────────────────────────────────────────

int GameWidget::aliveCount() const
{
    int n = 0;
    for (const auto& inv : m_invaders) if (inv.alive()) n++;
    return n;
}

int GameWidget::bottomInvaderY() const
{
    int maxY = 0;
    for (const auto& inv : m_invaders)
        if (inv.alive()) maxY = qMax(maxY, inv.y() + Invader::HEIGHT);
    return maxY;
}

// ─────────────────────────────────────────────────────────────────────────────
// Name-entry support
// ─────────────────────────────────────────────────────────────────────────────

void GameWidget::onNameReturn()
{
    QString name = m_nameInput->text().trimmed();
    if (name.isEmpty()) name = "PLAYER";
    m_nameInput->hide();
    m_waitingName = false;
    emit nameConfirmed(name);
    setFocus();
    update();
}

void GameWidget::positionNameInput()
{
    int iw = 220, ih = 36;
    m_nameInput->setGeometry(BOARD_W/2 - iw/2, BOARD_H/2 + 30, iw, ih);
}

// ─────────────────────────────────────────────────────────────────────────────
// Input
// ─────────────────────────────────────────────────────────────────────────────

void GameWidget::keyPressEvent(QKeyEvent* e)
{
    if (m_waitingName) { e->ignore(); return; }

    switch (e->key()) {
    case Qt::Key_Left:  case Qt::Key_A: m_leftHeld  = true; break;
    case Qt::Key_Right: case Qt::Key_D: m_rightHeld = true; break;
    case Qt::Key_Space:
        if (!e->isAutoRepeat()) m_fireHeld = true;
        break;
    case Qt::Key_P:
    case Qt::Key_Escape:
        togglePause();
        break;
    default:
        QWidget::keyPressEvent(e);
    }
}

void GameWidget::keyReleaseEvent(QKeyEvent* e)
{
    switch (e->key()) {
    case Qt::Key_Left:  case Qt::Key_A: m_leftHeld  = false; break;
    case Qt::Key_Right: case Qt::Key_D: m_rightHeld = false; break;
    case Qt::Key_Space: m_fireHeld = false; break;
    default:
        QWidget::keyReleaseEvent(e);
    }
}

// ─────────────────────────────────────────────────────────────────────────────
// Rendering
// ─────────────────────────────────────────────────────────────────────────────

void GameWidget::drawSprite8(QPainter& p, int x, int y,
                              const uint8_t* rows, int nRows,
                              int ps, QColor color)
{
    p.setPen(Qt::NoPen);
    p.setBrush(color);
    for (int r = 0; r < nRows; r++) {
        for (int c = 0; c < 8; c++) {
            if (rows[r] & (0x80 >> c))
                p.drawRect(x + c * ps, y + r * ps, ps, ps);
        }
    }
}

void GameWidget::paintEvent(QPaintEvent*)
{
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing, false);

    drawBackground(p);
    drawBunkers(p);
    drawBullets(p);

    // Invaders
    for (const auto& inv : m_invaders)
        if (inv.alive()) drawInvader(p, inv);

    drawMysteryShip(p);
    drawExplosions(p);
    drawBonusTexts(p);

    if (!m_gameOver) drawPlayer(p);

    drawGround(p);
    drawScoreBar(p);

    if (m_paused)        drawPauseOverlay(p);
    if (m_gameOver)      drawGameOverOverlay(p);
    if (m_levelComplete) drawLevelOverlay(p);
    if (m_waitingName)   drawNameOverlay(p);
}

void GameWidget::drawBackground(QPainter& p) const
{
    p.fillRect(rect(), QColor(0, 0, 10));

    // Starfield
    p.setPen(QColor(200, 200, 255, 160));
    // Deterministic stars via a simple LCG seeded by 42
    quint64 seed = 42;
    auto nextRand = [&](int mod) -> int {
        seed = seed * 6364136223846793005ULL + 1442695040888963407ULL;
        return static_cast<int>((seed >> 33) % mod);
    };
    for (int i = 0; i < 80; i++) {
        int sx = nextRand(BOARD_W);
        int sy = nextRand(BOARD_H);
        p.drawPoint(sx, sy);
    }
}

void GameWidget::drawScoreBar(QPainter& p) const
{
    p.fillRect(0, 0, BOARD_W, SCORE_H, QColor(0, 0, 10));
    p.setPen(QColor(0, 255, 0));

    QFont f("Courier New", 13, QFont::Bold);
    p.setFont(f);

    // Score
    p.setPen(QColor(255, 255, 255));
    p.drawText(12, 14, "SCORE");
    p.setPen(QColor(0, 255, 0));
    p.drawText(12, 34, QString("%1").arg(m_player.score(), 6, 10, QChar('0')));

    // Wave
    p.setPen(QColor(255, 255, 255));
    p.drawText(BOARD_W/2 - 35, 14, "WAVE");
    p.setPen(QColor(255, 215, 0));
    p.drawText(BOARD_W/2 - 10, 34, QString::number(m_wave));

    // Lives
    p.setPen(QColor(255, 255, 255));
    p.drawText(BOARD_W - 130, 14, "LIVES");
    for (int i = 0; i < m_player.lives(); i++) {
        int lx = BOARD_W - 120 + i * 26;
        drawSprite8(p, lx, 26, SPR_PLAYER, 6, 2, QColor(0, 200, 255));
    }

    // Horizontal separator
    p.setPen(QPen(QColor(0, 180, 0), 1));
    p.drawLine(0, SCORE_H - 1, BOARD_W, SCORE_H - 1);
}

void GameWidget::drawGround(QPainter& p) const
{
    p.setPen(QPen(QColor(0, 200, 0), 2));
    p.drawLine(0, GROUND_Y, BOARD_W, GROUND_Y);
}

void GameWidget::drawPlayer(QPainter& p) const
{
    int x = m_player.x();
    int y = m_player.y();
    // Scale player sprite up: pixel size = 4 (4×4 per dot), sprite is 8 wide × 6 tall → 32×24
    // Centre within Player::WIDTH
    int offX = (Player::WIDTH  - 8 * 4) / 2;
    int offY = (Player::HEIGHT - 6 * 4) / 2;
    drawSprite8(p, x + offX, y + offY, SPR_PLAYER, 6, 4, QColor(0, 220, 255));
}

void GameWidget::drawInvader(QPainter& p, const Invader& inv) const
{
    int x = inv.x();
    int y = inv.y();
    int f = inv.frame() ^ m_animFrame; // combine invader frame with global anim

    QColor color;
    const uint8_t* spr;
    switch (inv.type()) {
    case InvaderType::Small:
        color = QColor(255, 80, 255);
        spr   = SPR_SMALL[f];
        break;
    case InvaderType::Medium:
        color = QColor(0, 255, 200);
        spr   = SPR_MEDIUM[f];
        break;
    case InvaderType::Large:
        color = QColor(0, 255, 80);
        spr   = SPR_LARGE[f];
        break;
    }

    // Pixel size 3 → sprite 24×24, centred in 36×26 cell
    const int PS = 3;
    int offX = (Invader::WIDTH  - 8 * PS) / 2;
    int offY = (Invader::HEIGHT - 8 * PS) / 2;
    drawSprite8(p, x + offX, y + offY, spr, 8, PS, color);
}

void GameWidget::drawMysteryShip(QPainter& p) const
{
    if (!m_mystery.isActive()) return;
    int x = m_mystery.x();
    int y = m_mystery.y();
    // Pixel size 4 → 32×20, centred
    const int PS = 4;
    int offX = (MysteryShip::WIDTH  - 8 * PS) / 2;
    int offY = (MysteryShip::HEIGHT - 5 * PS) / 2;
    drawSprite8(p, x + offX, y + offY, SPR_MYSTERY, 5, PS, QColor(255, 60, 60));
}

void GameWidget::drawBunkers(QPainter& p) const
{
    p.setPen(Qt::NoPen);
    p.setBrush(QColor(0, 210, 0));
    for (const auto& bunk : m_bunkers) {
        for (int r = 0; r < Bunker::ROWS; r++) {
            for (int c = 0; c < Bunker::COLS; c++) {
                if (bunk.isCellAlive(c, r)) {
                    p.drawRect(bunk.x() + c * Bunker::CELL,
                               bunk.y() + r * Bunker::CELL,
                               Bunker::CELL, Bunker::CELL);
                }
            }
        }
    }
}

void GameWidget::drawBullets(QPainter& p) const
{
    for (const auto& b : m_bullets) {
        if (!b.active()) continue;
        QColor c = (b.owner() == BulletOwner::Player)
                   ? QColor(255, 255, 255)
                   : QColor(255, 100, 0);
        p.fillRect(b.rect(), c);
    }
}

void GameWidget::drawExplosions(QPainter& p) const
{
    for (const auto& ex : m_explosions) {
        float alpha = static_cast<float>(ex.timer) / 50.f;
        QColor c(255, 200, 50, static_cast<int>(alpha * 255));
        p.setPen(Qt::NoPen);
        p.setBrush(c);
        int half = ex.size / 2;
        // Draw star-like explosion using the sprite
        drawSprite8(p, ex.x - half + 2, ex.y - half + 2,
                    SPR_EXPLODE, 8, ex.size / 8, c);
    }
}

void GameWidget::drawBonusTexts(QPainter& p) const
{
    QFont f("Courier New", 11, QFont::Bold);
    p.setFont(f);
    for (const auto& bt : m_bonusTexts) {
        int alpha = qMin(255, bt.timer * 5);
        p.setPen(QColor(255, 215, 0, alpha));
        p.drawText(bt.x, bt.y, bt.text);
    }
}

// ─────────────────────────────────────────────────────────────────────────────
// Overlays
// ─────────────────────────────────────────────────────────────────────────────

static void drawCentredText(QPainter& p, int cx, int y,
                             const QString& text, const QFont& font,
                             QColor color)
{
    p.setFont(font);
    p.setPen(color);
    QFontMetrics fm(font);
    int w = fm.horizontalAdvance(text);
    p.drawText(cx - w / 2, y, text);
}

void GameWidget::drawPauseOverlay(QPainter& p) const
{
    p.fillRect(rect(), QColor(0, 0, 0, 140));
    QFont big("Courier New", 32, QFont::Bold);
    QFont small("Courier New", 14);
    drawCentredText(p, BOARD_W/2, BOARD_H/2 - 20, "PAUSED",    big,   QColor(255,215,0));
    drawCentredText(p, BOARD_W/2, BOARD_H/2 + 20, "Press P to resume", small, QColor(200,200,200));
}

void GameWidget::drawGameOverOverlay(QPainter& p) const
{
    p.fillRect(rect(), QColor(0, 0, 0, 160));
    QFont big("Courier New", 36, QFont::Bold);
    QFont med("Courier New", 16, QFont::Bold);
    QFont small("Courier New", 13);
    drawCentredText(p, BOARD_W/2, BOARD_H/2 - 60, "GAME OVER",  big,   QColor(255, 40, 40));
    drawCentredText(p, BOARD_W/2, BOARD_H/2,
                    QString("SCORE: %1").arg(m_player.score()), med, QColor(255, 215, 0));
    if (!m_waitingName)
        drawCentredText(p, BOARD_W/2, BOARD_H/2 + 40,
                        "Return to menu via the button above", small, QColor(180,180,180));
}

void GameWidget::drawLevelOverlay(QPainter& p) const
{
    p.fillRect(rect(), QColor(0, 0, 0, 130));
    QFont big("Courier New", 30, QFont::Bold);
    QFont med("Courier New", 16);
    drawCentredText(p, BOARD_W/2, BOARD_H/2 - 20,
                    QString("WAVE %1 CLEARED!").arg(m_wave), big, QColor(0, 255, 100));
    drawCentredText(p, BOARD_W/2, BOARD_H/2 + 20,
                    QString("Get ready for Wave %1 ...").arg(m_wave + 1), med, QColor(200,200,200));
}

void GameWidget::drawNameOverlay(QPainter& p) const
{
    p.fillRect(rect(), QColor(0, 0, 0, 160));
    QFont med("Courier New", 15, QFont::Bold);
    QFont small("Courier New", 12);
    drawCentredText(p, BOARD_W/2, BOARD_H/2 - 30, m_namePrompt, med, QColor(255, 215, 0));
    drawCentredText(p, BOARD_W/2, BOARD_H/2 + 80,
                    "Press ENTER to confirm", small, QColor(160,160,160));
}
