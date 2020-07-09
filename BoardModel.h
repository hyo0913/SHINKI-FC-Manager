#ifndef BOARDMODEL_H
#define BOARDMODEL_H

#include <QAbstractTableModel>

class Matchs;
class Players;
class BoardModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    enum BoardItemType {
        BoardNone,
        BoardGoal,
        BoardAssist,
        BoardTotal
    };

public:
    BoardModel(Matchs *matchs, Players *players, QObject *parent = nullptr);
    ~BoardModel();

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;

    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

    Qt::ItemFlags flags(const QModelIndex &index) const override;

    bool addMatch(const QDate &date);
    bool removeMatch(const QDate &date);
    void clearMatchs();

    bool addPlayer(const QString &name);
    bool removePlayer(const QString &name);
    bool editPlayerName(const QString &before, const QString &after);
    void movePlayerToLeft(const QString &name);
    void movePlayerToRight(const QString &name);

    void setViewItem(BoardItemType type);

    void changeModelData(Matchs *matchs, Players *players);

private:
    Matchs* m_matchs;
    Players* m_players;

    BoardItemType m_boardItemType;

    QDate getMatchDate(int row) const;
    QString getPlayerName(int column) const;
};

#endif // BOARDMODEL_H
