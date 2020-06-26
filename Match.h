#ifndef MATCH_H
#define MATCH_H

#include <QDate>
#include <QStringList>

class Match
{
public:
    Match();
    ~Match();

    QDate Date;
    QStringList Players;

    bool operator <(const Match &other) const;
};

class Matchs
{
public:
    Matchs();
    ~Matchs();

    inline int count() const { return m_matchs.count(); }
    bool exist(const QDate &date) const;
    Match* makeMatch(const QDate &date);
    void removeMatch(const QDate &date);
    Match* matchAt(int idx);
    const Match* matchAt(int idx) const;
    Match* match(const QDate &date);
    const Match* match(const QDate &date) const;
    int index(const QDate &date) const;

private:
    QList<Match*> m_matchs;
};

bool MatchLessThan(const Match *m1, const Match *m2);

#endif // MATCH_H
