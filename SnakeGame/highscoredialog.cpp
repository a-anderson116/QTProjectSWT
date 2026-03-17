#include "highscoredialog.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QHeaderView>
#include <QFont>

static const char* DIALOG_STYLE =
    "QDialog { background:#0a0a0a; color:white; }"
    "QTableWidget { background:#111; color:white; gridline-color:#2a2a2a;"
    "  border:none; font-family:Courier; font-size:11px; }"
    "QTableWidget::item { padding:4px 8px; }"
    "QHeaderView::section { background:#1a1a1a; color:#aaa; border:none;"
    "  border-bottom:1px solid #333; padding:4px; font-family:Courier; font-size:11px; }"
    "QScrollBar:vertical { background:#111; width:8px; }"
    "QScrollBar::handle:vertical { background:#333; border-radius:4px; }";

static const char* TAB_BTN =
    "QPushButton { background:#1a1a1a; color:#888; border:1px solid #333;"
    " border-radius:4px; padding:4px 18px; font-family:Courier; font-size:11px; }"
    "QPushButton:hover  { background:#252525; color:#bbb; }"
    "QPushButton:checked { background:#004400; color:#00ff00;"
    " border-color:#00aa00; font-weight:bold; }";

static const char* CLOSE_BTN =
    "QPushButton { background:#222; color:#ccc; border:1px solid #444;"
    " border-radius:4px; padding:5px 30px; font-family:Courier; font-size:11px; }"
    "QPushButton:hover { background:#333; color:white; }";

// Medal colours
static const QColor GOLD   = QColor(255, 215,   0);
static const QColor SILVER = QColor(192, 192, 192);
static const QColor BRONZE = QColor(205, 127,  50);
static const QColor NORMAL = QColor(200, 200, 200);

// ════════════════════════════════════════════════════════════════════════════
HighScoreDialog::HighScoreDialog(Difficulty initialTab, QWidget* parent)
    : QDialog(parent)
{
    setWindowTitle("High Scores");
    setStyleSheet(DIALOG_STYLE);
    setMinimumWidth(560);
    buildUI();
    setActiveTab(initialTab);
}

void HighScoreDialog::buildUI() {
    auto* vlay = new QVBoxLayout(this);
    vlay->setSpacing(10);
    vlay->setContentsMargins(16, 16, 16, 16);

    // ── Title ─────────────────────────────────────────────────────────────
    auto* title = new QLabel("🏆  HIGH SCORES", this);
    title->setFont(QFont("Courier", 20, QFont::Bold));
    title->setAlignment(Qt::AlignCenter);
    title->setStyleSheet("color:#ffd700; margin-bottom:4px;");
    vlay->addWidget(title);

    // ── Difficulty tab buttons ────────────────────────────────────────────
    auto* tabRow = new QHBoxLayout();
    tabRow->setSpacing(6);

    m_slugBtn   = new QPushButton("🐌  Slug",   this);
    m_wormBtn   = new QPushButton("🪱  Worm",   this);
    m_pythonBtn = new QPushButton("🐍  Python", this);

    for (auto* btn : {m_slugBtn, m_wormBtn, m_pythonBtn}) {
        btn->setCheckable(true);
        btn->setStyleSheet(TAB_BTN);
        tabRow->addWidget(btn);
    }
    vlay->addLayout(tabRow);

    connect(m_slugBtn,   &QPushButton::clicked, this, &HighScoreDialog::showSlug);
    connect(m_wormBtn,   &QPushButton::clicked, this, &HighScoreDialog::showWorm);
    connect(m_pythonBtn, &QPushButton::clicked, this, &HighScoreDialog::showPython);

    // ── Table ─────────────────────────────────────────────────────────────
    m_table = new QTableWidget(this);
    m_table->setColumnCount(5);
    m_table->setHorizontalHeaderLabels({"Rank", "Name", "Score", "Mode", "Date"});
    m_table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    m_table->setSelectionMode(QAbstractItemView::NoSelection);
    m_table->verticalHeader()->setVisible(false);
    m_table->horizontalHeader()->setStretchLastSection(true);
    m_table->setColumnWidth(0, 50);
    m_table->setColumnWidth(1, 130);
    m_table->setColumnWidth(2, 80);
    m_table->setColumnWidth(3, 100);
    m_table->setShowGrid(false);
    m_table->setAlternatingRowColors(false);
    vlay->addWidget(m_table);

    // ── Close button ──────────────────────────────────────────────────────
    auto* closeBtn = new QPushButton("Close", this);
    closeBtn->setStyleSheet(CLOSE_BTN);
    auto* btnRow = new QHBoxLayout();
    btnRow->addStretch();
    btnRow->addWidget(closeBtn);
    btnRow->addStretch();
    vlay->addLayout(btnRow);

    connect(closeBtn, &QPushButton::clicked, this, &QDialog::accept);
}

void HighScoreDialog::setActiveTab(Difficulty diff) {
    m_slugBtn->setChecked(diff == Difficulty::Slug);
    m_wormBtn->setChecked(diff == Difficulty::Worm);
    m_pythonBtn->setChecked(diff == Difficulty::Python);
    populateTable(diff);
}

void HighScoreDialog::showSlug()   { setActiveTab(Difficulty::Slug);   }
void HighScoreDialog::showWorm()   { setActiveTab(Difficulty::Worm);   }
void HighScoreDialog::showPython() { setActiveTab(Difficulty::Python); }

void HighScoreDialog::populateTable(Difficulty diff) {
    const QList<HighScoreEntry> list = HighScoreManager::instance().entries(diff);

    m_table->setRowCount(list.size());

    for (int i = 0; i < list.size(); ++i) {
        const HighScoreEntry& e = list[i];

        QColor rowColor;
        QString rankStr;
        if      (i == 0) { rowColor = GOLD;   rankStr = "🥇  1"; }
        else if (i == 1) { rowColor = SILVER; rankStr = "🥈  2"; }
        else if (i == 2) { rowColor = BRONZE; rankStr = "🥉  3"; }
        else             { rowColor = NORMAL; rankStr = QString("    %1").arg(i + 1); }

        const QString modeStr = (e.mode == GameMode::SinglePlayer) ? "Single" : "Multi";

        auto mkItem = [&](const QString& txt) {
            auto* item = new QTableWidgetItem(txt);
            item->setForeground(rowColor);
            item->setTextAlignment(Qt::AlignCenter);
            return item;
        };

        m_table->setItem(i, 0, mkItem(rankStr));
        m_table->setItem(i, 1, mkItem(e.name));
        m_table->setItem(i, 2, mkItem(QString::number(e.score)));
        m_table->setItem(i, 3, mkItem(modeStr));
        m_table->setItem(i, 4, mkItem(e.date));

        m_table->setRowHeight(i, 26);
    }

    if (list.isEmpty()) {
        m_table->setRowCount(1);
        auto* item = new QTableWidgetItem("No scores yet — be the first!");
        item->setForeground(QColor(80, 80, 80));
        item->setTextAlignment(Qt::AlignCenter);
        m_table->setSpan(0, 0, 1, 5);
        m_table->setItem(0, 0, item);
    } else {
        m_table->clearSpans();
    }
}
