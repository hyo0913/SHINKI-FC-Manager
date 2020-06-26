#ifndef PLAYER_H
#define PLAYER_H

#include <QString>
#include <QVariant>
#include <QDate>

namespace PlayDataItem {
const QString itemGoal = "Goal";
const QString itemAssist = "Assist";
const QString itemTotal = "Total";
}

class PlayData
{
public:
    explicit PlayData(const QDate &date = QDate::currentDate());
    ~PlayData();

    inline const QDate &date() const { return m_date; }
    inline QVariant data(const QString &itemName) const { return m_data.value(itemName); }
    inline void setData(const QString &itemName, const QVariant &data) { m_data.insert(itemName, data); }

private:
    QDate m_date;
    QHash<QString, QVariant> m_data;
};

Q_DECLARE_METATYPE(PlayData)

class Player
{
public:
    Player(const QString &name);
    ~Player();

    inline const QString &name() const { return m_name; }
    bool hasMatch(const QDate &date) const;
    PlayData* addMatch(const QDate &date);
    void removeMatch(const QDate &date);
    PlayData* playData(const QDate &date);
    const PlayData* playData(const QDate &date) const;
    QVariant data(const QDate &date, const QString &itemName) const;

private:
    QString m_name;
    QHash<QString, PlayData*> m_data;
};

class Players
{
public:
    Players();
    ~Players();

    inline QStringList playerNameList() const;
    inline int count() const { return m_players.count(); }
    bool exist(const QString &name) const;
    Player* makePalyer(const QString &name);
    void removePlayer(const QString &name);
    Player* playerAt(int idx);
    const Player* playerAt(int idx) const;
    Player* player(const QString &name);
    const Player* player(const QString &name) const;
    int index(const QString &name) const;

private:
    QList<Player*> m_players;
};

#endif // PLAYER_H
