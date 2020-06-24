#ifndef BOARDITEMDELEGATE_H
#define BOARDITEMDELEGATE_H

#include <QItemDelegate>

class BoardItemDelegate : public QItemDelegate
{
public:
    BoardItemDelegate();
    ~BoardItemDelegate();

    QWidget * createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    void setEditorData(QWidget *editor, const QModelIndex &index) const override;
    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const override;
};

#endif // BOARDITEMDELEGATE_H
