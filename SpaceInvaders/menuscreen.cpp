#include "menuscreen.h"

#include <QPainter>
#include <QKeyEvent>
#include <QFont>
#include <QFontMetrics>
#include <QPushButton>
#include <QVBoxLayout>

// Same 8×8 sprite helper (copy of the one in gamewidget.cpp)
static void drawSprite8M(QPainter& p, int x, int y,
                          const uint8_t* rows, int nRows,
                          int ps, QColor color)
{
    p.setPen(Qt::NoPen);
    p.setBrush(color);
    for (int r = 0; r < nRows; r++)
        for (int c = 0; c < 8; c++)
            if (rows[r] & (0x80 >> c))
                p.drawRect(x + c * ps, y + r * ps, ps, ps);
}

static const uint8_t SPR_SMALL_M[8]  = {0x18,0x3C,0xFF,0xDB,0xFF,0x24,0x5A,0xA5};
static const uint8_t SPR_MEDIUM_M[8] = {0x81,0x42,0xFF,0xBD,0xFF,0xFF,0x24,0x42};
static const uint8_t SPR_LARGE_M[8]  = {0x3C,0x7E,0xFF,0xE7,0xFF,0x7E,0x42,0xA5};
static const uint8_t SPR_MYSTERY_M[5]= {0x3C,0xFF,0xDB,0xFF,0x3C};

MenuScreen::MenuScreen(QWidget* parent)
    : QWidget(parent), m_blinkState(true)
{
    setStyleSheet("background-color: #00000A;");
    setFocusPolicy(Qt::StrongFocus);

    // Buttons
    auto btnStyle = [](QString fg, QString bg) {
        return QString(
            "QPushButton { color:%1; background:%2; border:2px solid %1;"
            "font:bold 15px 'Courier New'; padding:8px 32px; }"
            "QPushButton:hover { background:%1; color:#000010; }").arg(fg, bg);
    };

    auto* playBtn = new QPushButton("PLAY", this);
    playBtn->setStyleSheet(btnStyle("#00FF00", "transparent"));
    connect(playBtn, &QPushButton::clicked, this, &MenuScreen::playRequested);

    auto* hsBtn = new QPushButton("HIGH SCORES", this);
    hsBtn->setStyleSheet(btnStyle("#FFD700", "transparent"));
    connect(hsBtn, &QPushButton::clicked, this, &MenuScreen::highScoresRequested);

    auto* quitBtn = new QPushButton("QUIT", this);
    quitBtn->setStyleSheet(btnStyle("#FF4040", "transparent"));
    connect(quitBtn, &QPushButton::clicked, this, &MenuScreen::quitRequested);

    auto* vbox = new QVBoxLayout;
    vbox->addStretch(4);
    vbox->addWidget(playBtn);
    vbox->addSpacing(10);
    vbox->addWidget(hsBtn);
    vbox->addSpacing(10);
    vbox->addWidget(quitBtn);
    vbox->addStretch(1);
    vbox->setAlignment(Qt::AlignHCenter);

    // Wrap in outer layout that pushes buttons to lower half
    auto* outer = new QVBoxLayout(this);
    outer->addStretch(3);
    outer->addLayout(vbox);
    outer->addStretch(1);

    m_blinkTimer.setInterval(500);
    connect(&m_blinkTimer, &QTimer::timeout, this, &MenuScreen::onBlink);
    m_blinkTimer.start();
}

void MenuScreen::onBlink()
{
    m_blinkState = !m_blinkState;
    update();
}

void MenuScreen::keyPressEvent(QKeyEvent* e)
{
    if (e->key() == Qt::Key_Return || e->key() == Qt::Key_Space)
        emit playRequested();
    else
        QWidget::keyPressEvent(e);
}

void MenuScreen::paintEvent(QPaintEvent*)
{
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing, false);
    int W = width(), H = height();

    // Starfield
    quint64 seed = 7;
    auto rng = [&](int mod) -> int {
        seed = seed * 6364136223846793005ULL + 1442695040888963407ULL;
        return static_cast<int>((seed >> 33) % mod);
    };
    p.setPen(QColor(200, 200, 255, 140));
    for (int i = 0; i < 70; i++) p.drawPoint(rng(W), rng(H));

    // Title
    QFont titleFont("Courier New", 38, QFont::Bold);
    p.setFont(titleFont);

    auto centreX = [&](const QString& txt, const QFont& f) {
        return W/2 - QFontMetrics(f).horizontalAdvance(txt)/2;
    };

    p.setPen(QColor(255, 40, 40));
    p.drawText(centreX("SPACE", titleFont), 90, "SPACE");
    p.setPen(QColor(255, 255, 255));
    p.drawText(centreX("INVADERS", titleFont), 135, "INVADERS");

    // Score table (sprite + points)
    struct Row { const uint8_t* spr; int nRows; QColor col; int pts; };
    Row rows[] = {
        {SPR_MYSTERY_M,  5, {255, 60,  60},  300},
        {SPR_SMALL_M,    8, {255, 80, 255},   30},
        {SPR_MEDIUM_M,   8, {  0,255, 200},   20},
        {SPR_LARGE_M,    8, {  0,255,  80},   10},
    };

    QFont infoFont("Courier New", 12);
    p.setFont(infoFont);

    int tableX = W/2 - 80, tableY = 165;
    p.setPen(QColor(180, 180, 180));
    p.drawText(tableX - 10, tableY, "= SCORE GUIDE =");
    tableY += 18;

    for (auto& row : rows) {
        int ps = (row.nRows == 5) ? 3 : 2;
        drawSprite8M(p, tableX, tableY, row.spr, row.nRows, ps, row.col);
        p.setPen(QColor(255, 215, 0));
        p.setFont(infoFont);
        p.drawText(tableX + 30, tableY + (row.nRows * ps)/2 + 5,
                   QString("= %1 pts").arg(row.pts == 300 ? "???" : QString::number(row.pts)));
        tableY += row.nRows * ps + 10;
    }

    // Blink prompt
    if (m_blinkState) {
        QFont promptFont("Courier New", 14, QFont::Bold);
        p.setFont(promptFont);
        p.setPen(QColor(0, 255, 0));
        QString prompt = "-- PRESS SPACE TO PLAY --";
        p.drawText(centreX(prompt, promptFont), H - 75, prompt);
    }

    // Controls hint
    QFont hintFont("Courier New", 10);
    p.setFont(hintFont);
    p.setPen(QColor(120, 120, 120));
    p.drawText(centreX("<- -> to move   SPACE to fire   P to pause", hintFont),
               H - 50, "<- -> to move   SPACE to fire   P to pause");
}
