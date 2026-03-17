#ifndef MENUSCREEN_H
#define MENUSCREEN_H

#include <QWidget>
#include "gamemode.h"

class QButtonGroup;
class QRadioButton;
class QPushButton;

class MenuScreen : public QWidget {
    Q_OBJECT
public:
    explicit MenuScreen(QWidget* parent = nullptr);

signals:
    void startGame(GameMode mode, Difficulty difficulty);
    void showHighScores();

private slots:
    void onStartClicked();

private:
    QButtonGroup* m_modeGroup;
    QButtonGroup* m_diffGroup;
    QRadioButton* m_singleBtn;
    QRadioButton* m_multiBtn;
    QRadioButton* m_slugBtn;
    QRadioButton* m_wormBtn;
    QRadioButton* m_pythonBtn;
    QPushButton*  m_startBtn;
    QPushButton*  m_highScoreBtn;

    void buildUI();
};

#endif // MENUSCREEN_H
