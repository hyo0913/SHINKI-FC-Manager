#include "Match.h"

Match::Match()
{

}

Match::~Match()
{

}

bool Match::operator <(const Match &other) const
{
    return this->Date < other.Date;
}

/********************************************************************************/
Matchs::Matchs()
{

}

Matchs::~Matchs()
{
    qDeleteAll(m_matchs);
    m_matchs.clear();
}

bool Matchs::exist(const QDate &date) const
{
    bool result = false;

    for( int i = 0; i < m_matchs.count(); i++ ) {
        if( m_matchs.at(i)->Date == date ) {
            result = true;
            break;
        }
    }

    return result;
}

Match *Matchs::makeMatch(const QDate &date)
{
    Match* result = NULL;

    if( !this->exist(date) ) {
        result = new Match();
        result->Date = date;
        m_matchs << result;

        qSort(m_matchs.begin(), m_matchs.end(), MatchLessThan);
    }

    return result;
}

Match *Matchs::matchAt(int idx)
{
    Match* result = NULL;

    if( idx < m_matchs.count() ) {
        result = m_matchs.at(idx);
    }

    return result;
}

const Match *Matchs::matchAt(int idx) const
{
    Match* result = NULL;

    if( idx < m_matchs.count() ) {
        result = m_matchs.at(idx);
    }

    return result;
}

Match *Matchs::match(const QDate &date)
{
    Match* result = NULL;

    for( int i = 0; i < m_matchs.count(); i++ ) {
        if( m_matchs.at(i)->Date == date ) {
            result = m_matchs.at(i);
            break;
        }
    }

    return result;
}

const Match *Matchs::match(const QDate &date) const
{
    Match* result = NULL;

    for( int i = 0; i < m_matchs.count(); i++ ) {
        if( m_matchs.at(i)->Date == date ) {
            result = m_matchs.at(i);
            break;
        }
    }

    return result;
}

bool MatchLessThan(const Match *m1, const Match *m2)
{
    return m1->Date < m2->Date;
}
