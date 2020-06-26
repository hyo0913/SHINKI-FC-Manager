#ifndef BOARDMODEL_H
#define BOARDMODEL_H

#include <QAbstractTableModel>

#include "BoardHeader.h"

class Matchs;
class Players;
class BoardModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    enum BoardViewType {
        BoardNone,
        BoardGoal,
        BoardAssist,
        BoardTotal
    };
public:
    BoardModel(Matchs *matchs, Players *players, QObject *parent = nullptr);
    ~BoardModel();

    void setHorizontalHeader(BoardHorizontalHeader *header);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;

    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

    Qt::ItemFlags flags(const QModelIndex &index) const override;

    bool addMatch(const QDate &date);
    void removeMatch(const QDate &date);

    bool addPlayer(const QString &name);
    void removePlayer(const QString &name);

private:
    Matchs* m_matchs;
    Players* m_players;

    BoardHorizontalHeader* m_columnHeader;

    QDate getMatchDate(int row) const;
    QString getPlayerName(int column) const;
};

#endif // BOARDMODEL_H
