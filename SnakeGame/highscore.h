#ifndef HIGHSCORE_H
#define HIGHSCORE_H

#include <QString>
#include <QList>
#include <QMap>
#include "gamemode.h"

struct HighScoreEntry {
    QString    name;
    int        score;
    GameMode   mode;
    QString    date;
};

class HighScoreManager {
public:
    static HighScoreManager& instance();

    bool isHighScore(int score, Difficulty diff) const;
    void addEntry(const HighScoreEntry& entry, Difficulty diff);

    QList<HighScoreEntry> entries(Difficulty diff) const;
    int topScore(Difficulty diff) const;

private:
    HighScoreManager();
    HighScoreManager(const HighScoreManager&)            = delete;
    HighScoreManager& operator=(const HighScoreManager&) = delete;

    void load();
    void save() const;

    static const int MAX_ENTRIES = 10;

    QMap<Difficulty, QList<HighScoreEntry>> m_entries;
    QString m_filePath;
};

#endif // HIGHSCORE_H
