#ifndef GAMEWIDGET_H
#define GAMEWIDGET_H

#include <QWidget>
#include <QTimer>
#include <QVector>
#include <QLineEdit>

#include "player.h"
#include "invader.h"
#include "bullet.h"
#include "bunker.h"
#include "mysteryship.h"
#include "highscore.h"

// ── Board geometry ────────────────────────────────────────────────────────────
static const int BOARD_W   = 780;
static const int BOARD_H   = 640;
static const int SCORE_H   = 50;          // score/lives bar at top
static const int GROUND_Y  = BOARD_H - 28; // drawn ground line
static const int PLAYER_Y  = GROUND_Y - Player::HEIGHT - 4;

// ── Invader grid ──────────────────────────────────────────────────────────────
static const int GRID_COLS  = 11;
static const int GRID_ROWS  = 5;
static const int INV_SPAC_X = 52;   // horizontal spacing per cell
static const int INV_SPAC_Y = 46;   // vertical spacing per cell
static const int GRID_W     = GRID_COLS * INV_SPAC_X;   // 572
static const int GRID_START_X = (BOARD_W - GRID_W) / 2; // 104

// ── Step / shooting parameters ────────────────────────────────────────────────
static const int STEP_SIZE      = 8;
static const int DROP_AMT       = 18;
static const int STEP_INT_MAX   = 55;   // ticks between steps at full grid
static const int STEP_INT_MIN   = 2;    // ticks between steps at 1 invader
static const int SHOOT_INT_BASE = 80;   // ticks between enemy shots (base)
static const int SHOOT_INT_MIN  = 20;

// ── Timing ────────────────────────────────────────────────────────────────────
static const int MYSTERY_MIN_TICKS = 900;
static const int MYSTERY_RNG_TICKS = 600;

struct Explosion {
    int x, y;
    int timer;
    int size;
};

struct BonusText {
    int     x, y;
    int     timer;
    QString text;
};

class GameWidget : public QWidget {
    Q_OBJECT

public:
    explicit GameWidget(QWidget* parent = nullptr);
    ~GameWidget();

    void startGame();
    void togglePause();
    bool isPaused()  const { return m_paused;      }
    bool isRunning() const { return m_gameRunning; }

    // Called by MainWindow to start inline name entry after game-over.
    void beginNameEntry(const QString& prompt);

    QSize sizeHint() const override { return QSize(BOARD_W, BOARD_H); }

signals:
    void gameOver(int score);
    void menuRequested();
    void pauseStateChanged(bool paused);
    void nameConfirmed(const QString& name);

protected:
    void paintEvent(QPaintEvent*) override;
    void keyPressEvent(QKeyEvent*)   override;
    void keyReleaseEvent(QKeyEvent*) override;

private slots:
    void onTick();
    void onNameReturn();

private:
    // ── Game objects ─────────────────────────────────────────────────────────
    Player              m_player;
    QVector<Invader>    m_invaders;   // GRID_ROWS * GRID_COLS entries
    QVector<Bullet>     m_bullets;
    QVector<Bunker>     m_bunkers;
    MysteryShip         m_mystery;
    HighScore           m_highScore;

    // ── State ─────────────────────────────────────────────────────────────────
    int  m_wave;
    bool m_gameRunning;
    bool m_paused;
    bool m_gameOver;
    bool m_levelComplete;    // brief pause between waves
    int  m_levelTimer;

    // ── Invader movement ──────────────────────────────────────────────────────
    int  m_stepDir;          // +1 = right, -1 = left
    int  m_stepTimer;        // ticks until next lateral step
    int  m_stepInterval;     // current step interval (dynamic)
    int  m_animFrame;        // toggles on each grid step

    // ── Enemy shooting ────────────────────────────────────────────────────────
    int  m_shootTimer;
    int  m_shootInterval;

    // ── Mystery ship ──────────────────────────────────────────────────────────
    int  m_mysteryTimer;

    // ── Input ─────────────────────────────────────────────────────────────────
    bool m_leftHeld;
    bool m_rightHeld;
    bool m_fireHeld;         // prevents bullet spam
    int  m_fireCooldown;     // ticks since last player shot

    // ── Effects ───────────────────────────────────────────────────────────────
    QVector<Explosion> m_explosions;
    QVector<BonusText> m_bonusTexts;

    // ── Name entry overlay ────────────────────────────────────────────────────
    bool       m_waitingName;
    QString    m_namePrompt;
    QLineEdit* m_nameInput;

    // ── Timer ─────────────────────────────────────────────────────────────────
    QTimer m_timer;

    // ── Helpers ───────────────────────────────────────────────────────────────
    void initWave();
    void buildBunkers();
    void stepInvaders();
    void invaderShoot();
    void checkCollisions();
    void checkWinLose();
    void updateDifficulty();

    int  aliveCount() const;
    int  bottomInvaderY() const;

    // ── Drawing ───────────────────────────────────────────────────────────────
    void drawBackground(QPainter& p) const;
    void drawScoreBar(QPainter& p)   const;
    void drawGround(QPainter& p)     const;
    void drawPlayer(QPainter& p)     const;
    void drawInvader(QPainter& p, const Invader& inv) const;
    void drawBunkers(QPainter& p)    const;
    void drawMysteryShip(QPainter& p) const;
    void drawBullets(QPainter& p)    const;
    void drawExplosions(QPainter& p) const;
    void drawBonusTexts(QPainter& p) const;
    void drawPauseOverlay(QPainter& p) const;
    void drawGameOverOverlay(QPainter& p) const;
    void drawLevelOverlay(QPainter& p) const;
    void drawNameOverlay(QPainter& p)  const;
    void positionNameInput();

    // Pixel-art sprite helper
    static void drawSprite8(QPainter& p, int x, int y,
                            const uint8_t* row8, int rows,
                            int pixelSize, QColor color);
};

#endif // GAMEWIDGET_H
