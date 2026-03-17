#include "highscore.h"
#include <QFile>
#include <QDir>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QStandardPaths>
#include <algorithm>

// ── helpers ──────────────────────────────────────────────────────────────────
static QString diffKey(Difficulty d) {
    switch (d) {
        case Difficulty::Slug:   return "Slug";
        case Difficulty::Worm:   return "Worm";
        case Difficulty::Python: return "Python";
    }
    return "Worm";
}

static Difficulty keyToDiff(const QString& k) {
    if (k == "Slug")   return Difficulty::Slug;
    if (k == "Python") return Difficulty::Python;
    return Difficulty::Worm;
}

static QString modeStr(GameMode m) {
    return m == GameMode::SinglePlayer ? "Single" : "Multi";
}

static GameMode strMode(const QString& s) {
    return s == "Single" ? GameMode::SinglePlayer : GameMode::MultiPlayer;
}

// ════════════════════════════════════════════════════════════════════════════
HighScoreManager& HighScoreManager::instance() {
    static HighScoreManager mgr;
    return mgr;
}

HighScoreManager::HighScoreManager() {
    // C:\ProgramData\SnakeGame\ — writable by all Windows users without elevation
    QString dir = QStandardPaths::writableLocation(QStandardPaths::GenericDataLocation)
                  + "/SnakeGame";
    QDir().mkpath(dir);
    m_filePath = dir + "/highscores.json";
    load();
}

// ── public ───────────────────────────────────────────────────────────────────
bool HighScoreManager::isHighScore(int score, Difficulty diff) const {
    if (score <= 0) return false;
    const auto& list = m_entries.value(diff);
    if (list.size() < MAX_ENTRIES) return true;
    return score > list.last().score;
}

void HighScoreManager::addEntry(const HighScoreEntry& entry, Difficulty diff) {
    auto& list = m_entries[diff];
    list.append(entry);
    std::stable_sort(list.begin(), list.end(),
                     [](const HighScoreEntry& a, const HighScoreEntry& b) {
                         return a.score > b.score;
                     });
    if (list.size() > MAX_ENTRIES)
        list.resize(MAX_ENTRIES);
    save();
}

QList<HighScoreEntry> HighScoreManager::entries(Difficulty diff) const {
    return m_entries.value(diff);
}

int HighScoreManager::topScore(Difficulty diff) const {
    const auto& list = m_entries.value(diff);
    return list.isEmpty() ? 0 : list.first().score;
}

// ── persistence ───────────────────────────────────────────────────────────────
void HighScoreManager::load() {
    QFile f(m_filePath);
    if (!f.open(QIODevice::ReadOnly)) return;

    QJsonDocument doc = QJsonDocument::fromJson(f.readAll());
    if (!doc.isObject()) return;

    const QJsonObject root = doc.object();
    for (const QString& key : root.keys()) {
        Difficulty diff = keyToDiff(key);
        QList<HighScoreEntry> list;
        for (const QJsonValue& v : root[key].toArray()) {
            QJsonObject o = v.toObject();
            HighScoreEntry e;
            e.name  = o["name"].toString();
            e.score = o["score"].toInt();
            e.mode  = strMode(o["mode"].toString());
            e.date  = o["date"].toString();
            list.append(e);
        }
        m_entries[diff] = list;
    }
}

void HighScoreManager::save() const {
    QJsonObject root;
    for (auto it = m_entries.cbegin(); it != m_entries.cend(); ++it) {
        QJsonArray arr;
        for (const HighScoreEntry& e : it.value()) {
            QJsonObject o;
            o["name"]  = e.name;
            o["score"] = e.score;
            o["mode"]  = modeStr(e.mode);
            o["date"]  = e.date;
            arr.append(o);
        }
        root[diffKey(it.key())] = arr;
    }
    QFile f(m_filePath);
    if (f.open(QIODevice::WriteOnly))
        f.write(QJsonDocument(root).toJson());
}
