#include "BoardModel.h"

#include "Match.h"
#include "Player.h"

BoardModel::BoardModel(Matchs *matchs, Players *players, QObject *parent) :
    QAbstractTableModel(parent),
    m_matchs(matchs),
    m_players(players)
{
}

BoardModel::~BoardModel()
{
}

void BoardModel::setHorizontalHeader(BoardHorizontalHeader *header)
{
    m_columnHeader = header;
}

int BoardModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);

    return m_matchs->count();
}

int BoardModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);

    return m_players->count()*2;
}

QVariant BoardModel::data(const QModelIndex &index, int role) const
{
    QVariant result;

    if( !index.isValid() ) {
        return result;
    }

    if( role == Qt::DisplayRole ) {
        int column = index.column();
        if( column == 0 ) { column = 1; }
        const Match* match = m_matchs->matchAt(index.row());
        const Player* player = m_players->playerAt(column/2);

        if( match != NULL && player != NULL ) {
            if( player->hasMatch(match->Date) ) {
                if( index.column() % 2 == 0 ) {
                    result.setValue(player->playData(match->Date)->data("Goal"));
                } else {
                    result.setValue(player->playData(match->Date)->data("Assist"));
                }
            } else {
                // empty variant
            }
        }
    } else if( role == Qt::TextAlignmentRole ) {
        result.setValue(uint(Qt::AlignCenter));
    }

    return result;
}

bool BoardModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    return true;
}

QVariant BoardModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    QVariant result;

    if( role == Qt::DisplayRole ) {
        if( orientation == Qt::Horizontal ) {
            if( section % 2 == 0 ) {
                result.setValue(tr("Goal"));
            } else {
                result.setValue(tr("Assist"));
            }
        } else if( orientation == Qt::Vertical ) {
            if( section < m_matchs->count() ) {
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

    return QAbstractTableModel::flags(index) | Qt::ItemIsEditable;
}

bool BoardModel::addMatch(const QDate &date)
{
    if( m_matchs->exist(date) ) { return false; }

    int row = this->rowCount();
    beginInsertRows(QModelIndex(), row, row);

    m_matchs->makeMatch(date);

    this->insertRow(row);

    endInsertRows();

    return true;
}

void BoardModel::removeMatch(const QDate &date)
{

}

bool BoardModel::addPlayer(const QString &name)
{
    if( m_players->exist(name) ) { return false; }

    int column = this->columnCount();
    beginInsertColumns(QModelIndex(), column, column+1);

    m_players->makePalyer(name);

    this->insertColumn(column);

    endInsertColumns();

    return true;
}

void BoardModel::removePlayer(const QString &name)
{

}
