#ifndef BOARDVERTICALHEADERMODEL_H
#define BOARDVERTICALHEADERMODEL_H

#include <QHeaderView>
#include <QItemDelegate>

class BoardVertialHeaderItemDelegate : public QItemDelegate
{
    Q_OBJECT

public:
    BoardVertialHeaderItemDelegate(QObject *parent = nullptr);
    ~BoardVertialHeaderItemDelegate();

    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
};

class BoardVerticalHeader : public QHeaderView
{
    Q_OBJECT

public:
    BoardVerticalHeader(QWidget *parent = nullptr);
    ~BoardVerticalHeader();
};

#endif // BOARDVERTICALHEADERMODEL_H
