#ifndef BOARDVERTICALHEADERMODEL_H
#define BOARDVERTICALHEADERMODEL_H

#include <QHeaderView>

class BoardModel;

class BoardVerticalHeader : public QHeaderView
{
    Q_OBJECT

public:
    BoardVerticalHeader(QWidget *parent = nullptr);
    ~BoardVerticalHeader();
};


class BoardHorizontalHeaderModel : public QAbstractItemModel
{
    Q_OBJECT

public:
    BoardHorizontalHeaderModel(QObject *parent = nullptr);
    ~BoardHorizontalHeaderModel();

    QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const override;
    QModelIndex parent(const QModelIndex &child) const override;
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
};

class BoardHorizontalHeader : public QHeaderView
{
    Q_OBJECT

public:
    BoardHorizontalHeader(QWidget *parent = nullptr);
    ~BoardHorizontalHeader();

public slots:
    void sectionResize(int logicalIndex, int oldSize, int newSize);

private:
    BoardHorizontalHeaderModel* m_model;
};

#endif // BOARDVERTICALHEADERMODEL_H
