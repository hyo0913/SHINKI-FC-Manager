#include "BoardItem.h"

BoardItem::BoardItem()
{

}

BoardItem::BoardItem(const QString &text) :
    QTableWidgetItem(text)
{
}

BoardItem::~BoardItem()
{
}
