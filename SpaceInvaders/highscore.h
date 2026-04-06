#ifndef HIGHSCORE_H
#define HIGHSCORE_H

#include <QString>
#include <QVector>

struct ScoreEntry {
    QString name;
    int     score;
};

class HighScore {
public:
    static const int MAX_ENTRIES = 10;

    HighScore();
    void load();
    void save() const;

    bool isHighScore(int score) const;
    void addScore(const QString& name, int score);

    const QVector<ScoreEntry>& entries() const { return m_entries; }

private:
    QVector<ScoreEntry> m_entries;
    QString filePath() const;
};

#endif // HIGHSCORE_H
