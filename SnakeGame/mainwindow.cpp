#include "mainwindow.h"
#include "menuscreen.h"
#include "gamewidget.h"
#include "highscore.h"
#include "highscoredialog.h"
#include <QStackedWidget>
#include <QPushButton>
#include <QLabel>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QWidget>
#include <QDate>

static const char* TOOLBAR_STYLE =
    "background:#111; border-bottom:1px solid #2a2a2a;";

static const char* BTN_STYLE =
    "QPushButton { background:#222; color:#ccc; border:1px solid #444;"
    " border-radius:3px; padding:2px 12px; font-size:11px; }"
    "QPushButton:hover { background:#333; color:white; }"
    "QPushButton:disabled { color:#444; }";

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent),
      m_lastMode(GameMode::SinglePlayer),
      m_lastDiff(Difficulty::Worm)
{
    setWindowTitle("Snake Game");
    setStyleSheet("QMainWindow { background:#0a0a0a; }");
    buildUI();
    showMenu();
}

void MainWindow::buildUI() {
    auto* central = new QWidget(this);
    auto* vlay = new QVBoxLayout(central);
    vlay->setContentsMargins(0, 0, 0, 0);
    vlay->setSpacing(0);

    // ── Toolbar ──────────────────────────────────────────────────────────────
    auto* toolbar = new QWidget(central);
    toolbar->setFixedHeight(34);
    toolbar->setStyleSheet(TOOLBAR_STYLE);
    auto* tlay = new QHBoxLayout(toolbar);
    tlay->setContentsMargins(8, 2, 8, 2);

    m_menuBtn = new QPushButton("◀ Menu", toolbar);
    m_menuBtn->setStyleSheet(BTN_STYLE);

    m_pauseBtn = new QPushButton("⏸ Pause", toolbar);
    m_pauseBtn->setStyleSheet(BTN_STYLE);

    m_infoLabel = new QLabel("", toolbar);
    m_infoLabel->setStyleSheet("color:#555; font-size:10px;");

    tlay->addWidget(m_menuBtn);
    tlay->addWidget(m_pauseBtn);
    tlay->addStretch();
    tlay->addWidget(m_infoLabel);

    vlay->addWidget(toolbar);

    // ── Stacked content ──────────────────────────────────────────────────────
    m_stack = new QStackedWidget(central);

    m_menuScreen = new MenuScreen(m_stack);
    m_gameWidget = new GameWidget(m_stack);

    m_stack->addWidget(m_menuScreen);
    m_stack->addWidget(m_gameWidget);

    vlay->addWidget(m_stack);
    setCentralWidget(central);

    // ── Connections ──────────────────────────────────────────────────────────
    connect(m_menuScreen, &MenuScreen::startGame,
            this, &MainWindow::onStartGame);
    connect(m_menuScreen, &MenuScreen::showHighScores,
            this, &MainWindow::onShowHighScores);
    connect(m_gameWidget, &GameWidget::gameOver,
            this, &MainWindow::onGameOver);
    connect(m_gameWidget, &GameWidget::nameConfirmed,
            this, &MainWindow::onNameConfirmed);
    connect(m_gameWidget, &GameWidget::restartRequested,
            this, &MainWindow::onRestartRequested);
    connect(m_gameWidget, &GameWidget::menuRequested,
            this, &MainWindow::onMenuRequested);
    connect(m_gameWidget, &GameWidget::pauseStateChanged,
            this, &MainWindow::onPauseStateChanged);
    connect(m_menuBtn,  &QPushButton::clicked,
            this, &MainWindow::onMenuRequested);
    connect(m_pauseBtn, &QPushButton::clicked,
            m_gameWidget, &GameWidget::togglePause);
}

void MainWindow::showMenu() {
    m_stack->setCurrentWidget(m_menuScreen);
    m_menuBtn->setEnabled(false);
    m_pauseBtn->setEnabled(false);
    m_infoLabel->setText("Select a mode and difficulty, then press Start.");
    adjustSize();
    setFixedSize(size());
}

void MainWindow::showGame() {
    m_stack->setCurrentWidget(m_gameWidget);
    m_menuBtn->setEnabled(true);
    m_pauseBtn->setEnabled(true);
    m_pauseBtn->setText("⏸ Pause");
    m_gameWidget->setFocus();
    adjustSize();
    setFixedSize(size());
}

// ── slots ─────────────────────────────────────────────────────────────────────
void MainWindow::onStartGame(GameMode mode, Difficulty difficulty) {
    m_lastMode = mode;
    m_lastDiff = difficulty;

    const QString modeStr = (mode == GameMode::SinglePlayer) ? "Single" : "Multi";
    const QString diffStr = (difficulty == Difficulty::Slug)   ? "Slug"   :
                            (difficulty == Difficulty::Worm)   ? "Worm"   : "Python";
    m_infoLabel->setText(QString("%1 Player  ·  %2  ·  P=Pause  R=Restart  M=Menu")
                         .arg(modeStr).arg(diffStr));

    showGame();
    m_gameWidget->startGame(mode, difficulty);
}

void MainWindow::onGameOver(int score1, int score2, const QString& winner, GameMode mode) {
    m_pauseBtn->setEnabled(false);
    if (mode == GameMode::SinglePlayer)
        m_infoLabel->setText(QString("Game Over!  Score: %1").arg(score1));
    else
        m_infoLabel->setText(QString("%1  P1: %2  P2: %3").arg(winner).arg(score1).arg(score2));

    // Build queue of players whose score qualifies for the leaderboard
    m_pendingScores.clear();
    auto& mgr = HighScoreManager::instance();

    if (mode == GameMode::SinglePlayer) {
        if (mgr.isHighScore(score1, m_lastDiff))
            m_pendingScores.append({"Player", score1});
    } else {
        if (mgr.isHighScore(score1, m_lastDiff))
            m_pendingScores.append({"Player 1", score1});
        if (mgr.isHighScore(score2, m_lastDiff))
            m_pendingScores.append({"Player 2", score2});
    }

    if (!m_pendingScores.isEmpty())
        promptNextName();
    else {
        HighScoreDialog dlg(m_lastDiff, this);
        dlg.exec();
    }
}

void MainWindow::promptNextName() {
    const PendingScore& ps = m_pendingScores.first();
    const QString prompt   = QString("%1  —  Score: %2\nEnter your name:")
                                 .arg(ps.label).arg(ps.score);
    m_gameWidget->beginNameEntry(prompt);
}

void MainWindow::onNameConfirmed(const QString& name) {
    const PendingScore& ps = m_pendingScores.first();

    HighScoreEntry entry;
    entry.name  = name.isEmpty() ? ps.label : name.left(12);
    entry.score = ps.score;
    entry.mode  = m_lastMode;
    entry.date  = QDate::currentDate().toString("yyyy-MM-dd");
    HighScoreManager::instance().addEntry(entry, m_lastDiff);

    m_pendingScores.removeFirst();

    if (!m_pendingScores.isEmpty()) {
        // Next qualifying player
        promptNextName();
    } else {
        // All names collected — open the leaderboard
        HighScoreDialog dlg(m_lastDiff, this);
        dlg.exec();
    }
}

void MainWindow::onRestartRequested() {
    m_pendingScores.clear();
    m_pauseBtn->setEnabled(true);
    m_pauseBtn->setText("⏸ Pause");
    m_gameWidget->startGame(m_lastMode, m_lastDiff);
}

void MainWindow::onMenuRequested() {
    m_pendingScores.clear();
    showMenu();
}

void MainWindow::onPauseStateChanged(bool paused) {
    m_pauseBtn->setText(paused ? "▶ Resume" : "⏸ Pause");
}

void MainWindow::onShowHighScores() {
    HighScoreDialog dlg(Difficulty::Worm, this);
    dlg.exec();
}
