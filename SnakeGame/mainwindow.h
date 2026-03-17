#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
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

private:
    QStackedWidget* m_stack;
    MenuScreen*     m_menuScreen;
    GameWidget*     m_gameWidget;

    QPushButton*    m_pauseBtn;
    QPushButton*    m_menuBtn;
    QLabel*         m_infoLabel;

    GameMode    m_lastMode;
    Difficulty  m_lastDiff;

    void buildUI();
    void showMenu();
    void showGame();

    // Prompt for a name if the score qualifies; returns true if entry was added
    bool tryRecordScore(int score, GameMode mode, Difficulty diff,
                        const QString& playerLabel);
};

#endif // MAINWINDOW_H
