#ifndef HIGHSCOREDIALOG_H
#define HIGHSCOREDIALOG_H

#include <QDialog>
#include "highscore.h"

class HighScoreDialog : public QDialog {
    Q_OBJECT
public:
    explicit HighScoreDialog(const HighScore& hs, QWidget* parent = nullptr);
};

#endif // HIGHSCOREDIALOG_H
