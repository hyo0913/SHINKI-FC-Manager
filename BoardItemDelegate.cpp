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

void BoardItemDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    if( index.isValid() ) {
        if( index.column() == 0 ) {
            painter->save();

            QPen pen;
            pen.setWidth(1);
            pen.setColor(Qt::black);
            pen.setStyle(Qt::SolidLine);
            painter->setPen(pen);

            painter->drawText(option.rect, 0, tr("Goal\nAssist"));

            painter->restore();
        } else {
            if( index.data().canConvert<PlayData>() ) {
                PlayData data = qvariant_cast<PlayData>(index.data());

                QVariant goal = data.data("Goal");
                QVariant assist = data.data("Assist");

                painter->save();

                QPen pen;
                pen.setWidth(1);
                pen.setColor(Qt::black);
                pen.setStyle(Qt::SolidLine);
                painter->setPen(pen);

                painter->drawText(option.rect, option.index.flags(), goal.toString());

                painter->restore();
            } else {
                QItemDelegate::paint(painter, option, index);
            }
        }
    } else {
        QItemDelegate::paint(painter, option, index);
    }
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
