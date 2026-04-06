#include "highscoredialog.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QFont>

HighScoreDialog::HighScoreDialog(const HighScore& hs, QWidget* parent)
    : QDialog(parent)
{
    setWindowTitle("High Scores");
    setModal(true);
    setStyleSheet("background-color: #000010; color: #00FF00;");

    auto* vbox = new QVBoxLayout(this);
    vbox->setSpacing(6);
    vbox->setContentsMargins(30, 20, 30, 20);

    QFont titleFont("Courier New", 20, QFont::Bold);
    QFont entryFont("Courier New", 13);

    auto* title = new QLabel("=== HIGH SCORES ===", this);
    title->setFont(titleFont);
    title->setAlignment(Qt::AlignCenter);
    title->setStyleSheet("color: #FFD700;");
    vbox->addWidget(title);
    vbox->addSpacing(10);

    const auto& entries = hs.entries();
    if (entries.isEmpty()) {
        auto* none = new QLabel("No scores yet. Play to set a record!", this);
        none->setFont(entryFont);
        none->setAlignment(Qt::AlignCenter);
        vbox->addWidget(none);
    } else {
        for (int i = 0; i < entries.size(); i++) {
            QString line = QString("%1. %-16 %2")
                               .arg(i + 1, 2)
                               .arg(entries[i].score, 7);
            // Manually pad name
            QString name = entries[i].name;
            if (name.length() > 16) name = name.left(16);
            else while (name.length() < 16) name += ' ';
            line = QString("%1. %2 %3").arg(i + 1, 2).arg(name).arg(entries[i].score, 7);

            auto* lbl = new QLabel(line, this);
            lbl->setFont(entryFont);
            lbl->setStyleSheet(i == 0 ? "color: #FFD700;" : "color: #00FF00;");
            vbox->addWidget(lbl);
        }
    }

    vbox->addSpacing(14);

    auto* closeBtn = new QPushButton("CLOSE", this);
    closeBtn->setFont(QFont("Courier New", 12, QFont::Bold));
    closeBtn->setStyleSheet(
        "QPushButton { color: #000010; background: #00FF00;"
        "border: 2px solid #00FF00; padding: 6px 20px; }"
        "QPushButton:hover { background: #FFD700; color: #000010; border-color: #FFD700; }");
    connect(closeBtn, &QPushButton::clicked, this, &QDialog::accept);

    auto* hbox = new QHBoxLayout;
    hbox->addStretch();
    hbox->addWidget(closeBtn);
    hbox->addStretch();
    vbox->addLayout(hbox);

    setFixedSize(sizeHint());
}
