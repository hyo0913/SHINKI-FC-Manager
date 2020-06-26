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

int BoardModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);

    return m_matchs->count();
}

int BoardModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);

    return m_players->count()+1;
}

QVariant BoardModel::data(const QModelIndex &index, int role) const
{
    QVariant result;

    if( !index.isValid() ) {
        return result;
    }

    if( role == Qt::DisplayRole ) {
        const Match* match = m_matchs->matchAt(index.row());
        const Player* player = m_players->playerAt(index.column()-1);

        if( match != NULL && player != NULL ) {
            if( player->hasMatch(match->Date) ) {
                result.setValue(*player->playData(match->Date));
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
            if( 0 < section && section <= m_players->count() ) {
                result.setValue(m_players->playerAt(section-1)->name());
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
    if( !index.isValid() || index.column() == 0 ) {
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
    beginInsertColumns(QModelIndex(), column, column);

    m_players->makePalyer(name);

    this->insertColumn(column);

    endInsertColumns();

    return true;
}

void BoardModel::removePlayer(const QString &name)
{

}

void BoardModel::changeViewItem(BoardModel::BoardViewType type)
{
    this->beginResetModel();

    m_viewType = type;

    this->endResetModel();
}
