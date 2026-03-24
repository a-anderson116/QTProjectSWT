#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QList>
#include <QPair>
#include "gamemode.h"

class QStackedWidget;
class QPushButton;
class QLabel;
class MenuScreen;
class GameWidget;

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    explicit MainWindow(QWidget* parent = nullptr);

private slots:
    void onStartGame(GameMode mode, Difficulty difficulty);
    void onGameOver(int score1, int score2, const QString& winner, GameMode mode);
    void onRestartRequested();
    void onMenuRequested();
    void onPauseStateChanged(bool paused);
    void onShowHighScores();
    void onNameConfirmed(const QString& name);

private:
    QStackedWidget* m_stack;
    MenuScreen*     m_menuScreen;
    GameWidget*     m_gameWidget;

    QPushButton*    m_pauseBtn;
    QPushButton*    m_menuBtn;
    QLabel*         m_infoLabel;

    GameMode    m_lastMode;
    Difficulty  m_lastDiff;

    // Queue of {playerLabel, score} pairs awaiting name entry
    struct PendingScore { QString label; int score; };
    QList<PendingScore> m_pendingScores;

    void buildUI();
    void showMenu();
    void showGame();
    void promptNextName();   // shows name entry for front of m_pendingScores
};

#endif // MAINWINDOW_H
