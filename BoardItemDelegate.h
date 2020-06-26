#ifndef BOARDITEMDELEGATE_H
#define BOARDITEMDELEGATE_H

#include <QItemDelegate>

class BoardItemDelegate : public QItemDelegate
{
public:
    BoardItemDelegate(QObject *parent = nullptr);
    ~BoardItemDelegate();

    //void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    QWidget * createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    void setEditorData(QWidget *editor, const QModelIndex &index) const override;
    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const override;
};

#endif // BOARDITEMDELEGATE_H
