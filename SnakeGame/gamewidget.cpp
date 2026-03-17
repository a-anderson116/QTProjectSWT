#include "gamewidget.h"
#include <QPainter>
#include <QKeyEvent>
#include <QFont>

// ── helper: board-to-pixel (accounts for score bar) ─────────────────────────
static QRect cellRect(int col, int row, int scoreBar, int cellSize) {
    return QRect(col * cellSize,
                 scoreBar + row * cellSize,
                 cellSize, cellSize);
}

// ════════════════════════════════════════════════════════════════════════════
GameWidget::GameWidget(QWidget* parent)
    : QWidget(parent),
      m_snake1(nullptr), m_snake2(nullptr),
      m_gameMode(GameMode::SinglePlayer),
      m_difficulty(Difficulty::Worm),
      m_gameRunning(false), m_paused(false), m_gameOver(false),
      m_pendingDir1(Direction::None), m_pendingDir2(Direction::None)
{
    setFocusPolicy(Qt::StrongFocus);
    setFixedSize(sizeHint());
    connect(&m_timer, &QTimer::timeout, this, &GameWidget::onTimerTick);
}

GameWidget::~GameWidget() {
    delete m_snake1;
    delete m_snake2;
}

// ── public ──────────────────────────────────────────────────────────────────
void GameWidget::startGame(GameMode mode, Difficulty difficulty) {
    m_gameMode    = mode;
    m_difficulty  = difficulty;

    delete m_snake1; m_snake1 = nullptr;
    delete m_snake2; m_snake2 = nullptr;

    if (mode == GameMode::SinglePlayer) {
        m_snake1 = new Snake(QPoint(10, 14), Direction::Right);
    } else {
        m_snake1 = new Snake(QPoint(7,  14), Direction::Right);
        m_snake2 = new Snake(QPoint(22, 14), Direction::Left);
    }

    m_pendingDir1 = Direction::None;
    m_pendingDir2 = Direction::None;
    m_gameOver    = false;
    m_paused      = false;
    m_winner.clear();

    spawnFood();

    m_gameRunning = true;
    m_timer.start(timerInterval());
    setFocus();
    update();
}

void GameWidget::togglePause() {
    if (!m_gameRunning || m_gameOver) return;
    m_paused = !m_paused;
    m_paused ? m_timer.stop() : m_timer.start(timerInterval());
    emit pauseStateChanged(m_paused);
    update();
}

// ── event handlers ───────────────────────────────────────────────────────────
void GameWidget::keyPressEvent(QKeyEvent* event) {
    if (m_gameOver) {
        if (event->key() == Qt::Key_R) emit restartRequested();
        if (event->key() == Qt::Key_M) emit menuRequested();
        return;
    }

    if (event->key() == Qt::Key_P || event->key() == Qt::Key_Escape) {
        togglePause();
        return;
    }

    if (m_paused || !m_gameRunning) return;

    switch (event->key()) {
        case Qt::Key_W: m_pendingDir1 = Direction::Up;    break;
        case Qt::Key_S: m_pendingDir1 = Direction::Down;  break;
        case Qt::Key_A: m_pendingDir1 = Direction::Left;  break;
        case Qt::Key_D: m_pendingDir1 = Direction::Right; break;
        default: break;
    }

    if (m_gameMode == GameMode::MultiPlayer && m_snake2) {
        switch (event->key()) {
            case Qt::Key_Up:    m_pendingDir2 = Direction::Up;    break;
            case Qt::Key_Down:  m_pendingDir2 = Direction::Down;  break;
            case Qt::Key_Left:  m_pendingDir2 = Direction::Left;  break;
            case Qt::Key_Right: m_pendingDir2 = Direction::Right; break;
            default: break;
        }
    } else {
        switch (event->key()) {
            case Qt::Key_Up:    m_pendingDir1 = Direction::Up;    break;
            case Qt::Key_Down:  m_pendingDir1 = Direction::Down;  break;
            case Qt::Key_Left:  m_pendingDir1 = Direction::Left;  break;
            case Qt::Key_Right: m_pendingDir1 = Direction::Right; break;
            default: break;
        }
    }
}

// ── game loop ────────────────────────────────────────────────────────────────
void GameWidget::onTimerTick() {
    if (!m_gameRunning || m_paused || m_gameOver) return;

    if (m_pendingDir1 != Direction::None) {
        m_snake1->setDirection(m_pendingDir1);
        m_pendingDir1 = Direction::None;
    }
    if (m_snake2 && m_pendingDir2 != Direction::None) {
        m_snake2->setDirection(m_pendingDir2);
        m_pendingDir2 = Direction::None;
    }

    bool s1Moved = m_snake1->move(BOARD_WIDTH, BOARD_HEIGHT);
    bool s2Moved = m_snake2 ? m_snake2->move(BOARD_WIDTH, BOARD_HEIGHT) : true;

    bool s1Dead = !s1Moved || m_snake1->checkSelfCollision();
    bool s2Dead = m_snake2 && (!s2Moved || m_snake2->checkSelfCollision());

    if (m_snake2) {
        if (m_snake2->containsPoint(m_snake1->getHead())) s1Dead = true;
        if (m_snake1->containsPoint(m_snake2->getHead())) s2Dead = true;
    }

    if (s1Dead || s2Dead) { handleDeath(s1Dead, s2Dead); return; }

    checkFoodCollisions();
    update();
}

void GameWidget::checkFoodCollisions() {
    QPoint foodPos = m_food.getPosition();

    if (m_snake1->getHead() == foodPos) {
        m_snake1->scheduleGrow();
        m_snake1->addScore(10);
        spawnFood();
        return;
    }
    if (m_snake2 && m_snake2->getHead() == foodPos) {
        m_snake2->scheduleGrow();
        m_snake2->addScore(10);
        spawnFood();
    }
}

void GameWidget::handleDeath(bool s1Dead, bool s2Dead) {
    m_timer.stop();
    m_gameRunning = false;
    m_gameOver    = true;

    if (m_gameMode == GameMode::SinglePlayer) {
        m_winner = "GAME OVER";
    } else {
        if (s1Dead && s2Dead) m_winner = "DRAW!";
        else if (s1Dead)      m_winner = "PLAYER 2 WINS!";
        else                  m_winner = "PLAYER 1 WINS!";
    }

    emit gameOver(m_snake1->getScore(),
                  m_snake2 ? m_snake2->getScore() : 0,
                  m_winner, m_gameMode);
    update();
}

void GameWidget::spawnFood() {
    m_food.randomize(BOARD_WIDTH, BOARD_HEIGHT, allOccupied());
}

QList<QPoint> GameWidget::allOccupied() const {
    QList<QPoint> occ;
    if (m_snake1) occ += m_snake1->getBody();
    if (m_snake2) occ += m_snake2->getBody();
    return occ;
}

int GameWidget::timerInterval() const {
    switch (m_difficulty) {
        case Difficulty::Slug:   return 200;
        case Difficulty::Worm:   return 120;
        case Difficulty::Python: return 70;
    }
    return 120;
}

// ── painting ─────────────────────────────────────────────────────────────────
void GameWidget::paintEvent(QPaintEvent*) {
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing);

    drawBackground(p);

    if (!m_gameRunning && !m_gameOver) {
        p.setPen(QColor(100, 100, 100));
        p.setFont(QFont("Courier", 14));
        p.drawText(rect(), Qt::AlignCenter, "Select a mode from the menu\nand press START");
        return;
    }

    drawFood(p);
    if (m_snake1) drawSnake(p, *m_snake1, QColor(0,180,0),  QColor(0,255,0));
    if (m_snake2) drawSnake(p, *m_snake2, QColor(0,100,200), QColor(0,160,255));

    drawScoreBar(p);
    if (m_paused)   drawPauseOverlay(p);
    if (m_gameOver) drawGameOverOverlay(p);
}

void GameWidget::drawBackground(QPainter& p) const {
    p.fillRect(rect(), QColor(10, 10, 10));

    // Score bar background
    p.fillRect(0, 0, width(), SCORE_BAR, QColor(25, 25, 25));

    // Board background
    QRect board(0, SCORE_BAR, BOARD_WIDTH * CELL_SIZE, BOARD_HEIGHT * CELL_SIZE);
    p.fillRect(board, QColor(18, 18, 18));

    // Grid lines
    p.setPen(QPen(QColor(30, 30, 30), 1));
    for (int x = 0; x <= BOARD_WIDTH; ++x)
        p.drawLine(x * CELL_SIZE, SCORE_BAR,
                   x * CELL_SIZE, SCORE_BAR + BOARD_HEIGHT * CELL_SIZE);
    for (int y = 0; y <= BOARD_HEIGHT; ++y)
        p.drawLine(0,           SCORE_BAR + y * CELL_SIZE,
                   width(),     SCORE_BAR + y * CELL_SIZE);

    // Board border
    p.setPen(QPen(QColor(0, 120, 0), 2));
    p.drawRect(0, SCORE_BAR, BOARD_WIDTH * CELL_SIZE - 1, BOARD_HEIGHT * CELL_SIZE - 1);
}

void GameWidget::drawFood(QPainter& p) const {
    QPoint pos = m_food.getPosition();
    QRect cr = cellRect(pos.x(), pos.y(), SCORE_BAR, CELL_SIZE);

    // Apple body
    p.setBrush(QColor(210, 30, 30));
    p.setPen(QPen(QColor(160, 10, 10), 1));
    p.drawEllipse(cr.adjusted(2, 3, -2, -2));

    // Stem
    p.setPen(QPen(QColor(90, 50, 0), 2));
    p.drawLine(cr.center().x(), cr.top() + 3, cr.center().x() + 3, cr.top());

    // Leaf hint
    p.setPen(QPen(QColor(0, 140, 0), 1));
    p.drawLine(cr.center().x() + 1, cr.top() + 1, cr.center().x() + 4, cr.top() + 3);
}

void GameWidget::drawSnake(QPainter& p, const Snake& s,
                            QColor bodyColor, QColor headColor) const {
    const QList<QPoint>& body = s.getBody();
    const int n = body.size();

    for (int i = n - 1; i >= 0; --i) {
        QRect cr = cellRect(body[i].x(), body[i].y(), SCORE_BAR, CELL_SIZE);

        if (i == 0) {
            p.setBrush(headColor);
            p.setPen(QPen(headColor.darker(160), 1));
            p.drawRoundedRect(cr.adjusted(1,1,-1,-1), 4, 4);

            // Eyes
            p.setBrush(Qt::black);
            p.setPen(Qt::NoPen);
            Direction dir = s.getDirection();
            QPoint e1, e2;
            const int eo = 4, es = 3;
            if      (dir == Direction::Right) { e1 = {cr.right()-eo-es, cr.top()+eo};    e2 = {cr.right()-eo-es, cr.bottom()-eo-es}; }
            else if (dir == Direction::Left)  { e1 = {cr.left()+eo,    cr.top()+eo};    e2 = {cr.left()+eo,    cr.bottom()-eo-es}; }
            else if (dir == Direction::Up)    { e1 = {cr.left()+eo,    cr.top()+eo};    e2 = {cr.right()-eo-es, cr.top()+eo};    }
            else                              { e1 = {cr.left()+eo,    cr.bottom()-eo-es}; e2 = {cr.right()-eo-es, cr.bottom()-eo-es}; }
            p.drawEllipse(e1.x(), e1.y(), es, es);
            p.drawEllipse(e2.x(), e2.y(), es, es);
        } else {
            float t = (float)i / n;
            QColor seg = bodyColor.darker(100 + (int)(t * 70));
            p.setBrush(seg);
            p.setPen(QPen(seg.darker(130), 1));
            p.drawRoundedRect(cr.adjusted(2,2,-2,-2), 3, 3);
        }
    }
}

void GameWidget::drawScoreBar(QPainter& p) const {
    QFont f("Courier", 11, QFont::Bold);
    p.setFont(f);

    if (m_gameMode == GameMode::SinglePlayer) {
        QString diffStr = (m_difficulty == Difficulty::Slug)   ? "SLUG"   :
                          (m_difficulty == Difficulty::Worm)   ? "WORM"   : "PYTHON";
        QString txt = QString("Score: %1   Length: %2   [%3]   P/ESC=Pause  M=Menu")
                      .arg(m_snake1 ? m_snake1->getScore()  : 0)
                      .arg(m_snake1 ? m_snake1->getLength() : 0)
                      .arg(diffStr);
        p.setPen(QColor(0, 220, 0));
        p.drawText(QRect(0, 0, width(), SCORE_BAR), Qt::AlignCenter, txt);
    } else {
        QString diffStr = (m_difficulty == Difficulty::Slug)   ? "SLUG"   :
                          (m_difficulty == Difficulty::Worm)   ? "WORM"   : "PYTHON";
        p.setPen(QColor(0, 255, 0));
        p.drawText(QRect(5, 0, width()/2 - 5, SCORE_BAR), Qt::AlignVCenter | Qt::AlignLeft,
                   QString("P1 WASD: %1").arg(m_snake1 ? m_snake1->getScore() : 0));
        p.setPen(QColor(0, 160, 255));
        p.drawText(QRect(width()/2, 0, width()/2 - 5, SCORE_BAR), Qt::AlignVCenter | Qt::AlignRight,
                   QString("P2 ←→↑↓: %1 ").arg(m_snake2 ? m_snake2->getScore() : 0));
        p.setPen(QColor(140, 140, 140));
        p.drawText(QRect(0, 0, width(), SCORE_BAR), Qt::AlignCenter,
                   QString("[%1]  P=Pause").arg(diffStr));
    }
}

void GameWidget::drawPauseOverlay(QPainter& p) const {
    p.fillRect(rect(), QColor(0, 0, 0, 160));
    p.setPen(Qt::white);
    p.setFont(QFont("Courier", 22, QFont::Bold));
    p.drawText(rect(), Qt::AlignCenter, "PAUSED\n\nPress P to Resume");
}

void GameWidget::drawGameOverOverlay(QPainter& p) const {
    p.fillRect(rect(), QColor(0, 0, 0, 175));

    p.setFont(QFont("Courier", 26, QFont::Bold));
    p.setPen(QColor(255, 60, 60));
    p.drawText(QRect(0, height()/2 - 110, width(), 50), Qt::AlignCenter, m_winner);

    QString details;
    if (m_gameMode == GameMode::SinglePlayer) {
        details = QString("Score: %1\nLength: %2")
                  .arg(m_snake1 ? m_snake1->getScore()  : 0)
                  .arg(m_snake1 ? m_snake1->getLength() : 0);
    } else {
        details = QString("Player 1: %1    Player 2: %2")
                  .arg(m_snake1 ? m_snake1->getScore() : 0)
                  .arg(m_snake2 ? m_snake2->getScore() : 0);
    }
    p.setFont(QFont("Courier", 14));
    p.setPen(Qt::white);
    p.drawText(QRect(0, height()/2 - 55, width(), 130), Qt::AlignCenter,
               details + "\n\n[ R ] Restart     [ M ] Menu");
}
