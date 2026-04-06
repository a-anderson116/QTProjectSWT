#include "highscore.h"
#include <QStandardPaths>
#include <QDir>
#include <QFile>
#include <QTextStream>
#include <algorithm>

HighScore::HighScore()
{
    load();
}

QString HighScore::filePath() const
{
    return QStandardPaths::writableLocation(QStandardPaths::AppDataLocation)
           + "/spaceinvaders_scores.txt";
}

void HighScore::load()
{
    m_entries.clear();
    QFile file(filePath());
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return;

    QTextStream in(&file);
    while (!in.atEnd()) {
        QString line = in.readLine().trimmed();
        if (line.isEmpty()) continue;
        int sep = line.lastIndexOf(',');
        if (sep < 0) continue;
        ScoreEntry e;
        e.name  = line.left(sep).trimmed();
        e.score = line.mid(sep + 1).trimmed().toInt();
        m_entries.append(e);
    }
}

void HighScore::save() const
{
    QString dir = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    QFile::remove(filePath());

    // Ensure directory exists
    QDir().mkpath(dir);

    QFile file(filePath());
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
        return;

    QTextStream out(&file);
    for (const auto& e : m_entries)
        out << e.name << "," << e.score << "\n";
}

bool HighScore::isHighScore(int score) const
{
    if (m_entries.size() < MAX_ENTRIES) return score > 0;
    return score > m_entries.last().score;
}

void HighScore::addScore(const QString& name, int score)
{
    ScoreEntry e{name, score};
    m_entries.append(e);
    std::sort(m_entries.begin(), m_entries.end(),
              [](const ScoreEntry& a, const ScoreEntry& b){ return a.score > b.score; });
    if (m_entries.size() > MAX_ENTRIES)
        m_entries.resize(MAX_ENTRIES);
    save();
}
