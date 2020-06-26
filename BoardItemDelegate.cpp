#include "BoardItemDelegate.h"

#include <QDebug>

#include <QPainter>
#include <QSpinBox>

#include "Player.h"

BoardItemDelegate::BoardItemDelegate(QObject *parent) :
    QItemDelegate(parent)
{
}

BoardItemDelegate::~BoardItemDelegate()
{
}

QWidget *BoardItemDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    if( index.isValid() ) {
        return new QSpinBox(parent);
    }

    return QItemDelegate::createEditor(parent, option, index);
}

void BoardItemDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    QSpinBox* spinEditor = qobject_cast<QSpinBox*>(editor);
    if( spinEditor ) {
        QVariant value = index.model()->data(index, Qt::DisplayRole);
        spinEditor->setValue(value.toInt());
    } else {
        QItemDelegate::setEditorData(editor, index);
    }
}

void BoardItemDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    QSpinBox* spinEditor = qobject_cast<QSpinBox*>(editor);
    if( spinEditor ) {
        model->setData(index, spinEditor->value());
    } else {
        QItemDelegate::setEditorData(editor, index);
    }
}
