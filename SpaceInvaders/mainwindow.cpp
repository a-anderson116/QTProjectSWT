#include "mainwindow.h"
#include "menuscreen.h"
#include "gamewidget.h"
#include "highscoredialog.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QWidget>
#include <QInputDialog>
#include <QFont>

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent), m_pendingScore(0)
{
    buildUI();
    showMenu();
}

void MainWindow::buildUI()
{
    setWindowTitle("Space Invaders");
    setStyleSheet("QMainWindow { background: #000010; }");

    // ── Central container ─────────────────────────────────────────────────────
    auto* central = new QWidget(this);
    setCentralWidget(central);

    auto* vbox = new QVBoxLayout(central);
    vbox->setContentsMargins(0, 0, 0, 0);
    vbox->setSpacing(0);

    // ── Top toolbar ────────────────────────────────────────────────────────────
    auto* toolbar = new QWidget(this);
    toolbar->setFixedHeight(36);
    toolbar->setStyleSheet("background:#000020; border-bottom:1px solid #003300;");

    auto* toolLayout = new QHBoxLayout(toolbar);
    toolLayout->setContentsMargins(8, 2, 8, 2);

    QString btnStyle =
        "QPushButton { color:#00FF00; background:transparent; border:1px solid #00FF00;"
        "font:bold 11px 'Courier New'; padding:2px 12px; }"
        "QPushButton:hover { background:#00FF00; color:#000010; }"
        "QPushButton:disabled { color:#224422; border-color:#224422; }";

    m_menuBtn  = new QPushButton("MENU",  toolbar);
    m_pauseBtn = new QPushButton("PAUSE", toolbar);
    m_menuBtn ->setStyleSheet(btnStyle);
    m_pauseBtn->setStyleSheet(btnStyle);

    m_scoreLabel = new QLabel("", toolbar);
    m_scoreLabel->setFont(QFont("Courier New", 11, QFont::Bold));
    m_scoreLabel->setStyleSheet("color:#FFD700;");

    toolLayout->addWidget(m_menuBtn);
    toolLayout->addWidget(m_pauseBtn);
    toolLayout->addStretch();
    toolLayout->addWidget(m_scoreLabel);

    vbox->addWidget(toolbar);

    // ── Stacked game area ─────────────────────────────────────────────────────
    m_stack = new QStackedWidget(this);
    m_menu  = new MenuScreen(m_stack);
    m_game  = new GameWidget(m_stack);
    m_stack->addWidget(m_menu);
    m_stack->addWidget(m_game);
    vbox->addWidget(m_stack);

    // ── Connections ───────────────────────────────────────────────────────────
    connect(m_menu, &MenuScreen::playRequested,       this, &MainWindow::onPlayRequested);
    connect(m_menu, &MenuScreen::highScoresRequested, this, &MainWindow::onHighScoresRequested);
    connect(m_menu, &MenuScreen::quitRequested,       this, &QMainWindow::close);

    connect(m_game, &GameWidget::gameOver,        this, &MainWindow::onGameOver);
    connect(m_game, &GameWidget::menuRequested,   this, &MainWindow::onMenuRequested);
    connect(m_game, &GameWidget::pauseStateChanged, this, &MainWindow::onPauseStateChanged);
    connect(m_game, &GameWidget::nameConfirmed,   this, &MainWindow::onNameConfirmed);

    connect(m_menuBtn,  &QPushButton::clicked, this, &MainWindow::onMenuRequested);
    connect(m_pauseBtn, &QPushButton::clicked, m_game, &GameWidget::togglePause);

    setFixedSize(BOARD_W, BOARD_H + 36); // game + toolbar
}

void MainWindow::showMenu()
{
    m_stack->setCurrentWidget(m_menu);
    m_pauseBtn->setEnabled(false);
    m_scoreLabel->setText("");
    m_menu->setFocus();
}

void MainWindow::showGame()
{
    m_stack->setCurrentWidget(m_game);
    m_pauseBtn->setEnabled(true);
    m_game->startGame();
}

// ── Slots ─────────────────────────────────────────────────────────────────────

void MainWindow::onPlayRequested()
{
    showGame();
}

void MainWindow::onHighScoresRequested()
{
    HighScoreDialog dlg(m_highScore, this);
    dlg.exec();
}

void MainWindow::onGameOver(int score)
{
    m_pauseBtn->setEnabled(false);
    m_scoreLabel->setText(QString("FINAL SCORE: %1").arg(score));

    if (m_highScore.isHighScore(score)) {
        m_pendingScore = score;
        m_game->beginNameEntry(QString("NEW HIGH SCORE: %1 \nEnter your name:").arg(score));
    } else {
        // No high score — show the scoreboard then return to menu
        HighScoreDialog dlg(m_highScore, this);
        dlg.exec();
        showMenu();
    }
}

void MainWindow::onMenuRequested()
{
    showMenu();
}

void MainWindow::onPauseStateChanged(bool paused)
{
    m_pauseBtn->setText(paused ? "RESUME" : "PAUSE");
}

void MainWindow::onNameConfirmed(const QString& name)
{
    if (m_pendingScore > 0) {
        m_highScore.addScore(name, m_pendingScore);
        m_pendingScore = 0;
    }

    // Show the high score table, then return to menu
    HighScoreDialog dlg(m_highScore, this);
    dlg.exec();
    showMenu();
}
