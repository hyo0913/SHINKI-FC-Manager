#include "BoardHeader.h"

#include "BoardModel.h"

/**********************************************************************************************************************************************/
BoardVerticalHeader::BoardVerticalHeader(QWidget *parent) :
    QHeaderView(Qt::Vertical, parent)
{
}

BoardVerticalHeader::~BoardVerticalHeader()
{
}

/**********************************************************************************************************************************************/
BoardHorizontalHeaderModel::BoardHorizontalHeaderModel(QObject *parent)
{

}

BoardHorizontalHeaderModel::~BoardHorizontalHeaderModel()
{

}

QModelIndex BoardHorizontalHeaderModel::index(int row, int column, const QModelIndex &parent) const
{
    return createIndex(row, column);
}

QModelIndex BoardHorizontalHeaderModel::parent(const QModelIndex &child) const
{
    return createIndex(-1, -1);
}

int BoardHorizontalHeaderModel::rowCount(const QModelIndex &parent) const
{
    return 0;
}

int BoardHorizontalHeaderModel::columnCount(const QModelIndex &parent) const
{
    return 3;
}

QVariant BoardHorizontalHeaderModel::data(const QModelIndex &index, int role) const
{
    QVariant result;

    return result;
}

QVariant BoardHorizontalHeaderModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    QVariant result;

    if( role == Qt::DisplayRole ) {
        switch( section )
        {
        case 0: result.setValue(QString("a")); break;
        case 1: result.setValue(QString("b")); break;
        case 2: result.setValue(QString("c")); break;
        }
    }

    return result;
}

/**********************************************************************************************************************************************/
BoardHorizontalHeader::BoardHorizontalHeader(QWidget *parent) :
    QHeaderView(Qt::Horizontal, parent),
    m_model(new BoardHorizontalHeaderModel())
{
    this->setModel(m_model);
    this->setFixedHeight(30);

    this->setSectionResizeMode(QHeaderView::Fixed);
}

BoardHorizontalHeader::~BoardHorizontalHeader()
{

}

void BoardHorizontalHeader::sectionResize(int logicalIndex, int oldSize, int newSize)
{
    this->setFixedWidth(newSize*2);
}
