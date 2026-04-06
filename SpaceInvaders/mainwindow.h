#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStackedWidget>
#include <QPushButton>
#include <QLabel>

#include "highscore.h"

class MenuScreen;
class GameWidget;

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    explicit MainWindow(QWidget* parent = nullptr);

private slots:
    void onPlayRequested();
    void onHighScoresRequested();
    void onGameOver(int score);
    void onMenuRequested();
    void onPauseStateChanged(bool paused);
    void onNameConfirmed(const QString& name);

private:
    QStackedWidget* m_stack;
    MenuScreen*     m_menu;
    GameWidget*     m_game;

    QPushButton*    m_pauseBtn;
    QPushButton*    m_menuBtn;
    QLabel*         m_scoreLabel;

    HighScore       m_highScore;
    int             m_pendingScore;  // score awaiting name entry

    void buildUI();
    void showMenu();
    void showGame();
};

#endif // MAINWINDOW_H
