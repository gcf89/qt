#pragma once

#include <QStyledItemDelegate>
#include <QPainter>
#include <QStyleOptionViewItem>
#include <QModelIndex>
#include <QLineEdit>


class CustomSearchDelegate : public QStyledItemDelegate
{
  QLineEdit*              mLineEdit;

  Q_OBJECT

public:
  CustomSearchDelegate(QLineEdit* lineEdit, QWidget *parent = 0)
    : QStyledItemDelegate(parent), mLineEdit(lineEdit) {}

  void paint(QPainter *painter, const QStyleOptionViewItem &option,
             const QModelIndex &index) const override;
  QSize sizeHint(const QStyleOptionViewItem &option,
                 const QModelIndex &index) const override;
};
