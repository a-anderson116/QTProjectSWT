#include "menuscreen.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QButtonGroup>
#include <QRadioButton>
#include <QGroupBox>
#include <QFont>

static const char* DARK_STYLE =
    "background-color:#0a0a0a; color:white;";

static const char* GROUP_STYLE =
    "QGroupBox { color:#aaa; border:1px solid #333; border-radius:5px;"
    " padding:8px; margin-top:6px; font-size:11px; }"
    "QGroupBox::title { subcontrol-origin:margin; padding:0 4px; }";

static const char* RADIO_STYLE_BASE = "color:white; font-size:11px;";

MenuScreen::MenuScreen(QWidget* parent) : QWidget(parent) {
    setStyleSheet(DARK_STYLE);
    buildUI();
}

void MenuScreen::buildUI() {
    auto* root = new QVBoxLayout(this);
    root->setAlignment(Qt::AlignCenter);
    root->setSpacing(12);
    root->setContentsMargins(40, 20, 40, 20);

    // ── Title ───────────────────────────────────────────────────────────────
    auto* title = new QLabel("🐍  SNAKE GAME", this);
    title->setFont(QFont("Courier", 30, QFont::Bold));
    title->setAlignment(Qt::AlignCenter);
    title->setStyleSheet("color:#00ff00; letter-spacing:4px;");
    root->addWidget(title);

    auto* sub = new QLabel("Atari-style retro snake – eat apples, avoid yourself!", this);
    sub->setAlignment(Qt::AlignCenter);
    sub->setStyleSheet("color:#555; font-size:11px;");
    root->addWidget(sub);

    root->addSpacing(10);

    // ── Game Mode ────────────────────────────────────────────────────────────
    auto* modeBox = new QGroupBox("Game Mode", this);
    modeBox->setStyleSheet(GROUP_STYLE);
    auto* modeRow = new QHBoxLayout(modeBox);
    m_modeGroup = new QButtonGroup(this);
    m_singleBtn = new QRadioButton("Single Player", modeBox);
    m_multiBtn  = new QRadioButton("Multi Player (WASD vs Arrows)", modeBox);
    m_singleBtn->setChecked(true);
    m_singleBtn->setStyleSheet(RADIO_STYLE_BASE);
    m_multiBtn->setStyleSheet("color:#88ccff; font-size:11px;");
    m_modeGroup->addButton(m_singleBtn, 0);
    m_modeGroup->addButton(m_multiBtn,  1);
    modeRow->addWidget(m_singleBtn);
    modeRow->addWidget(m_multiBtn);
    root->addWidget(modeBox);

    // ── Difficulty ───────────────────────────────────────────────────────────
    auto* diffBox = new QGroupBox("Difficulty", this);
    diffBox->setStyleSheet(GROUP_STYLE);
    auto* diffRow = new QHBoxLayout(diffBox);
    m_diffGroup = new QButtonGroup(this);

    m_slugBtn   = new QRadioButton("🐌  Slug\n(slowest, 200 ms/step)",   diffBox);
    m_wormBtn   = new QRadioButton("🪱  Worm\n(medium, 120 ms/step)",    diffBox);
    m_pythonBtn = new QRadioButton("🐍  Python\n(fastest, 70 ms/step)",  diffBox);
    m_wormBtn->setChecked(true);

    m_slugBtn->setStyleSheet("color:#88ff88; font-size:11px;");
    m_wormBtn->setStyleSheet("color:#ffff66; font-size:11px;");
    m_pythonBtn->setStyleSheet("color:#ff6666; font-size:11px;");

    m_diffGroup->addButton(m_slugBtn,   0);
    m_diffGroup->addButton(m_wormBtn,   1);
    m_diffGroup->addButton(m_pythonBtn, 2);

    diffRow->addWidget(m_slugBtn);
    diffRow->addWidget(m_wormBtn);
    diffRow->addWidget(m_pythonBtn);
    root->addWidget(diffBox);

    // ── Controls ─────────────────────────────────────────────────────────────
    auto* ctrlBox = new QGroupBox("Controls", this);
    ctrlBox->setStyleSheet(GROUP_STYLE);
    auto* ctrlCol = new QVBoxLayout(ctrlBox);

    auto mkLabel = [&](const QString& txt, const char* color) {
        auto* l = new QLabel(txt, ctrlBox);
        l->setAlignment(Qt::AlignCenter);
        l->setStyleSheet(QString("color:%1; font-size:11px;").arg(color));
        ctrlCol->addWidget(l);
    };

    mkLabel("Player 1: W A S D  or  ↑ ← ↓ →  (single player)", "#00dd00");
    mkLabel("Player 2: ↑ ← ↓ →  (multiplayer only)",            "#44aaff");
    mkLabel("Pause / Resume: P or ESC",                           "#aaaaaa");
    mkLabel("Restart: R   •   Back to Menu: M  (after game over)","#777777");
    root->addWidget(ctrlBox);

    // ── Tips ─────────────────────────────────────────────────────────────────
    auto* tip = new QLabel("Tip: Take wide sweeping turns along the edges – it's the safest path!", this);
    tip->setWordWrap(true);
    tip->setAlignment(Qt::AlignCenter);
    tip->setStyleSheet("color:#444; font-size:10px; font-style:italic;");
    root->addWidget(tip);

    root->addSpacing(8);

    // ── Start button ─────────────────────────────────────────────────────────
    m_startBtn = new QPushButton("▶  START GAME", this);
    m_startBtn->setFixedSize(220, 52);
    m_startBtn->setFont(QFont("Courier", 15, QFont::Bold));
    m_startBtn->setStyleSheet(
        "QPushButton { background:#005500; color:white; border:2px solid #00aa00;"
        " border-radius:6px; }"
        "QPushButton:hover   { background:#007700; border-color:#00ff00; }"
        "QPushButton:pressed { background:#003300; }");

    // ── High Scores button ───────────────────────────────────────────────────
    m_highScoreBtn = new QPushButton("🏆  HIGH SCORES", this);
    m_highScoreBtn->setFixedSize(220, 38);
    m_highScoreBtn->setFont(QFont("Courier", 11));
    m_highScoreBtn->setStyleSheet(
        "QPushButton { background:#1a1400; color:#ffd700; border:1px solid #665500;"
        " border-radius:5px; }"
        "QPushButton:hover   { background:#2a2000; border-color:#ffd700; }"
        "QPushButton:pressed { background:#0d0a00; }");

    auto* btnRow = new QHBoxLayout();
    btnRow->addStretch();
    btnRow->addWidget(m_startBtn);
    btnRow->addStretch();
    root->addLayout(btnRow);

    auto* hsRow = new QHBoxLayout();
    hsRow->addStretch();
    hsRow->addWidget(m_highScoreBtn);
    hsRow->addStretch();
    root->addLayout(hsRow);

    connect(m_startBtn,     &QPushButton::clicked, this, &MenuScreen::onStartClicked);
    connect(m_highScoreBtn, &QPushButton::clicked, this, &MenuScreen::showHighScores);
}

void MenuScreen::onStartClicked() {
    GameMode mode = (m_modeGroup->checkedId() == 0)
                    ? GameMode::SinglePlayer : GameMode::MultiPlayer;
    Difficulty diff;
    switch (m_diffGroup->checkedId()) {
        case 0:  diff = Difficulty::Slug;   break;
        case 2:  diff = Difficulty::Python; break;
        default: diff = Difficulty::Worm;   break;
    }
    emit startGame(mode, diff);
}
