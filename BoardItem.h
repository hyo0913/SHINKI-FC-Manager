#ifndef BOARDITEM_H
#define BOARDITEM_H

#include <QTableWidgetItem>

class BoardItem : public QTableWidgetItem
{
public:
    BoardItem();
    BoardItem(const QString &text);
    ~BoardItem();
};

#endif // BOARDITEM_H
