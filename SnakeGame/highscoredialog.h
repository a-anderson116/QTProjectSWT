#ifndef HIGHSCOREDIALOG_H
#define HIGHSCOREDIALOG_H

#include <QDialog>
#include "highscore.h"

class QTableWidget;
class QPushButton;

class HighScoreDialog : public QDialog {
    Q_OBJECT
public:
    // Opens with the given difficulty tab pre-selected (defaults to Slug)
    explicit HighScoreDialog(Difficulty initialTab = Difficulty::Slug,
                             QWidget* parent = nullptr);

private slots:
    void showSlug();
    void showWorm();
    void showPython();

private:
    QTableWidget* m_table;
    QPushButton*  m_slugBtn;
    QPushButton*  m_wormBtn;
    QPushButton*  m_pythonBtn;

    void buildUI();
    void populateTable(Difficulty diff);
    void setActiveTab(Difficulty diff);
};

#endif // HIGHSCOREDIALOG_H
