#include "BoardItemDelegate.h"

#include <QDateTimeEdit>

BoardItemDelegate::BoardItemDelegate()
{

}

BoardItemDelegate::~BoardItemDelegate()
{

}

QWidget *BoardItemDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    if( index.column() == 0 ) {
        QDateTimeEdit *editor = new QDateTimeEdit(parent);
        editor->setDisplayFormat("yyyy-MM-dd");
        editor->setCalendarPopup(true);
        return editor;
    }

    return QItemDelegate::createEditor(parent, option, index);
}

void BoardItemDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    QDateTimeEdit* dateEditor = qobject_cast<QDateTimeEdit*>(editor);
    if( dateEditor ) {
        dateEditor->setDate(QDate::fromString(index.model()->data(index, Qt::EditRole).toString(), "yyyy-MM-dd"));
    } else {
        QItemDelegate::setEditorData(editor, index);
    }
}

void BoardItemDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    QDateTimeEdit* dateEditor = qobject_cast<QDateTimeEdit*>(editor);
    if( dateEditor ) {
        model->setData(index, dateEditor->date().toString("yyyy-MM-dd"));
    } else {
        QItemDelegate::setModelData(editor, model, index);
    }
}
