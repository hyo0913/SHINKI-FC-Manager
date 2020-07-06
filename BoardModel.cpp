#include "BoardModel.h"

#include "Match.h"
#include "Player.h"

BoardModel::BoardModel(Matchs *matchs, Players *players, QObject *parent) :
    QAbstractTableModel(parent),
    m_matchs(matchs),
    m_players(players),
    m_boardItemType(BoardGoal)
{
}

BoardModel::~BoardModel()
{
}

int BoardModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);

    return m_matchs->count()+1;
}

int BoardModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);

    return m_players->count();
}

QVariant BoardModel::data(const QModelIndex &index, int role) const
{
    QVariant result;

    if( !index.isValid() ) {
        return result;
    }

    if( role == Qt::DisplayRole ) {
        if( index.row() == m_matchs->count() ) {    // total
            int i = 0;
            quint64 value = 0;
            switch( m_boardItemType )
            {
            case BoardGoal:
                for( ; i < m_matchs->count(); i++ ) {
                    const Match* match = m_matchs->matchAt(i);
                    const Player* player = m_players->playerAt(index.column());
                    if( match == NULL || player == NULL ) { continue; }

                    if( player->hasMatch(match->Date) ) {
                        value += player->playData(match->Date)->data("Goal").toULongLong();
                    }
                }
                break;
            case BoardAssist:
                for( ; i < m_matchs->count(); i++ ) {
                    const Match* match = m_matchs->matchAt(i);
                    const Player* player = m_players->playerAt(index.column());
                    if( match == NULL || player == NULL ) { continue; }

                    if( player->hasMatch(match->Date) ) {
                        value += player->playData(match->Date)->data("Assist").toULongLong();
                    }
                }
                break;
            case BoardTotal:
                for( ; i < m_matchs->count(); i++ ) {
                    const Match* match = m_matchs->matchAt(i);
                    const Player* player = m_players->playerAt(index.column());
                    if( match == NULL || player == NULL ) { continue; }

                    if( player->hasMatch(match->Date) ) {
                        value += player->playData(match->Date)->data("Goal").toULongLong();
                        value += player->playData(match->Date)->data("Assist").toULongLong();
                    }
                }
                break;
            default:
                ;
            }
            result.setValue(value);
        } else {    // each match
            const Match* match = m_matchs->matchAt(index.row());
            const Player* player = m_players->playerAt(index.column());

            if( match != NULL && player != NULL ) {
                if( player->hasMatch(match->Date) ) {

                    switch( m_boardItemType )
                    {
                    case BoardGoal:
                        result.setValue(player->playData(match->Date)->data(PlayDataItem::itemGoal).toUInt());
                        break;
                    case BoardAssist:
                        result.setValue(player->playData(match->Date)->data(PlayDataItem::itemAssist).toUInt());
                        break;
                    case BoardTotal:
                        result.setValue(player->playData(match->Date)->data(PlayDataItem::itemGoal).toUInt() + player->playData(match->Date)->data("Assist").toUInt());
                        break;
                    default:
                        ;
                    }
                } else {
                    result.setValue(tr("no play"));
                }
            }
        }
    } else if( role == Qt::TextAlignmentRole ) {
        result.setValue(uint(Qt::AlignCenter));
    }

    return result;
}

bool BoardModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if( !index.isValid() ) {
        return false;
    }

    bool result = false;

    if( role == Qt::EditRole ) {
        Match* match = m_matchs->matchAt(index.row());
        Player* player = m_players->playerAt(index.column());

        if( match != NULL && player != NULL ) {
            if( player->hasMatch(match->Date) ) {
                result = true;
                switch( m_boardItemType )
                {
                case BoardGoal:
                    player->playData(match->Date)->setData(PlayDataItem::itemGoal, value);
                    break;
                case BoardAssist:
                    player->playData(match->Date)->setData(PlayDataItem::itemAssist, value);
                    break;
                default:
                    result = false;
                }
            }
        }
    }

    return result;
}

QVariant BoardModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    QVariant result;

    if( role == Qt::DisplayRole ) {
        if( orientation == Qt::Horizontal ) {
            if( section < m_players->count() ) {
                result.setValue(m_players->playerAt(section)->name());
            }
        } else if( orientation == Qt::Vertical ) {
            if( section == m_matchs->count() ) {
                result.setValue(tr("Total"));
            } else {
                result.setValue(m_matchs->matchAt(section)->Date.toString("yyyy-MM-dd"));
            }
        }
    }

    return result;
}

Qt::ItemFlags BoardModel::flags(const QModelIndex &index) const
{
    if( !index.isValid() ) {
        return Qt::ItemIsEnabled;
    }

    const Match* match = m_matchs->matchAt(index.row());
    const Player* player = m_players->playerAt(index.column());

    if( match == NULL || player == NULL || !player->hasMatch(match->Date) ) {
        return QAbstractTableModel::flags(index) & (~Qt::ItemIsEditable);
    }

    return QAbstractTableModel::flags(index) | Qt::ItemIsEditable;
}

bool BoardModel::addMatch(const QDate &date)
{
    if( m_matchs->exist(date) ) { return false; }

    int row = this->rowCount();
    this->beginInsertRows(QModelIndex(), row, row);

    m_matchs->makeMatch(date);

    this->insertRow(row);

    this->endInsertRows();

    return true;
}

bool BoardModel::removeMatch(const QDate &date)
{
    if( !m_matchs->exist(date) ) { return false; }

    int idx = m_matchs->index(date);
    if( idx < 0 ) { return false; }

    this->beginRemoveRows(QModelIndex(), idx, idx);

    Match* match = m_matchs->match(date);
    QStringList playerNames = match->Players;
    while( playerNames.count() > 0 )
    {
        Player* player = m_players->player(playerNames.takeFirst());
        if( player == NULL ) { continue; }

        player->removeMatch(match->Date);
    }

    m_matchs->removeMatch(date);

    this->removeRow(idx, QModelIndex());

    this->endRemoveRows();

    return true;
}

void BoardModel::clearMatchs()
{
    while( m_matchs->count() > 0 )
    {
        Match* match = m_matchs->matchAt(0);
        this->removeMatch(match->Date);
    }
}

bool BoardModel::addPlayer(const QString &name)
{
    if( m_players->exist(name) ) { return false; }

    int column = this->columnCount();
    this->beginInsertColumns(QModelIndex(), column, column);

    m_players->makePalyer(name);

    this->insertColumn(column);

    this->endInsertColumns();

    return true;
}

bool BoardModel::removePlayer(const QString &name)
{
    if( !m_players->exist(name) ) { return false; }

    int idx = m_players->index(name);
    if( idx < 0 ) { return false; }

    this->beginRemoveColumns(QModelIndex(), idx, idx);

    m_players->removePlayer(name);

    this->removeColumn(idx, QModelIndex());

    this->endRemoveColumns();

    return true;
}

void BoardModel::setViewItem(BoardModel::BoardItemType type)
{
    bool reset = (m_boardItemType != type) ? true : false;

    if( reset ) { this->beginResetModel(); }

    m_boardItemType = type;

    if( reset ) { this->endResetModel(); }
}

void BoardModel::changeModelData(Matchs *matchs, Players *players)
{
    if( matchs == NULL || players == NULL ) { return; }

    this->beginResetModel();

    m_matchs = matchs;
    m_players = players;

    this->endResetModel();
}
