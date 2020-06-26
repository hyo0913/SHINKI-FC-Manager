#include "BoardVerticalHeader.h"

#include <QDebug>

BoardVertialHeaderItemDelegate::BoardVertialHeaderItemDelegate(QObject *parent) :
    QItemDelegate(parent)
{

}

BoardVertialHeaderItemDelegate::~BoardVertialHeaderItemDelegate()
{

}

void BoardVertialHeaderItemDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    qDebug() << index.row() << index.column();

    QItemDelegate::paint(painter, option, index);
}


BoardVerticalHeader::BoardVerticalHeader(QWidget *parent) :
    QHeaderView(Qt::Vertical, parent)
{
    this->setItemDelegate(new BoardVertialHeaderItemDelegate());
}

BoardVerticalHeader::~BoardVerticalHeader()
{

}
