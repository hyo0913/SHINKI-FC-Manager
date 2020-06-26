#include "Player.h"

PlayData::PlayData(const QDate &date) :
    m_date(date)
{
}

PlayData::~PlayData()
{
}

/********************************************************************************/
Player::Player(const QString &name) :
    m_name(name)
{
}

Player::~Player()
{
    qDeleteAll(m_data);
    m_data.clear();
}

bool Player::hasMatch(const QDate &date) const
{
    return m_data.contains(date.toString());
}

PlayData *Player::addMatch(const QDate &date)
{
    PlayData* result = playData(date);

    if( result == NULL) {
        const QString key = date.toString();
        result = new PlayData(date);
        m_data.insert(key, result);
    }

    return result;
}

void Player::removeMatch(const QDate &date)
{
    const QString key = date.toString();

    if( m_data.contains(key) ) {
        delete m_data.take(key);
    }
}

PlayData *Player::playData(const QDate &date)
{
    return m_data.value(date.toString(), NULL);
}

const PlayData *Player::playData(const QDate &date) const
{
    return m_data.value(date.toString(), NULL);
}

QVariant Player::data(const QDate &date, const QString &itemName) const
{
    QVariant result;
    const QString key = date.toString();

    if( m_data.contains(key) ) {
        PlayData* playData = m_data.value(key);
        result = playData->data(itemName);
    }

    return result;
}

/********************************************************************************/
Players::Players()
{
}

Players::~Players()
{
    qDeleteAll(m_players);
    m_players.clear();
}

bool Players::exist(const QString &name) const
{
    bool result = false;

    for( int i = 0; i < m_players.count(); i++ ) {
        if( m_players.at(i)->name() == name ) {
            result = true;
            break;
        }
    }

    return result;
}

Player *Players::makePalyer(const QString &name)
{
    Player* result = NULL;

    if( !this->exist(name) ) {
        result = new Player(name);
        m_players << result;
    }

    return result;
}

void Players::removePlayer(const QString &name)
{
    Player* player = this->player(name);
    if( player != NULL ) {
        m_players.removeOne(player);
        delete player;
    }
}

Player *Players::playerAt(int idx)
{
    Player* result = NULL;

    if( idx < m_players.count() ) {
        result = m_players.at(idx);
    }

    return result;
}

const Player *Players::playerAt(int idx) const
{
    Player* result = NULL;

    if( idx < m_players.count() ) {
        result = m_players.at(idx);
    }

    return result;
}

Player *Players::player(const QString &name)
{
    Player* result = NULL;

    for( int i = 0; i < m_players.count(); i++ ) {
        if( m_players.at(i)->name() == name ) {
            result = m_players.at(i);
            break;
        }
    }

    return result;
}

const Player *Players::player(const QString &name) const
{
    Player* result = NULL;

    for( int i = 0; i < m_players.count(); i++ ) {
        if( m_players.at(i)->name() == name ) {
            result = m_players.at(i);
            break;
        }
    }

    return result;
}

int Players::index(const QString &name) const
{
    int result = -1;

    for( int i = 0; i < m_players.count(); i++ ) {
        if( m_players.at(i)->name() == name ) {
            result = i;
            break;
        }
    }

    return result;
}
