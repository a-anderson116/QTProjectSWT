#ifndef MENUSCREEN_H
#define MENUSCREEN_H

#include <QWidget>
#include <QTimer>

class MenuScreen : public QWidget {
    Q_OBJECT
public:
    explicit MenuScreen(QWidget* parent = nullptr);

signals:
    void playRequested();
    void highScoresRequested();
    void quitRequested();

protected:
    void paintEvent(QPaintEvent*) override;
    void keyPressEvent(QKeyEvent*)override;

private slots:
    void onBlink();

private:
    QTimer m_blinkTimer;
    bool   m_blinkState;
};

#endif // MENUSCREEN_H
