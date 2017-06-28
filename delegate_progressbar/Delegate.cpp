#include <QApplication>

#include "Delegate.h"

Delegate::Delegate(QWidget* parent)
  : QStyledItemDelegate(parent)
{

}

void Delegate::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
//  QItemDelegate::paint(painter, viewOption,index);

  int progress = index.model()->data(index,Qt::DisplayRole).toInt();
  QStyleOptionProgressBar progressBarOption;
  progressBarOption.rect = QRect(option.rect.x(),
                                 option.rect.y()/*+(SETHEIGHT - PROGRESSBARHEIGHT)/2*/,
                                 option.rect.width(),
                                 option.rect.height()
//                                 PROGRESSBARHEIGHT
                                 );

  //qDebug()<<progressBarOption.rect.x()<<progressBarOption.rect.y()<<progressBarOption.rect.height()<<progressBarOption.rect.width();
  //qDebug()<<option.rect.x()<<option.rect.y()<<option.rect.height()<<option.rect.width();

  progressBarOption.state |= QStyle::State_Enabled;
  progressBarOption.direction = QApplication::layoutDirection();
  progressBarOption.fontMetrics = QApplication::fontMetrics();
  progressBarOption.minimum = 0;
  progressBarOption.maximum = 100;
  progressBarOption.textAlignment = Qt::AlignCenter;
  progressBarOption.textVisible = true;
  progressBarOption.progress = progress < 0 ? 0 : progress;
  progressBarOption.text = QString().sprintf("%d%%", progressBarOption.progress);
  QApplication::style()->drawControl(QStyle::CE_ProgressBar, &progressBarOption, painter);
}

QSize Delegate::sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const
{
  return 5 * QSize(5, 1);
}

QWidget*Delegate::createEditor(QWidget* parent, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
  return QStyledItemDelegate::createEditor(parent, option, index);
}

void Delegate::setEditorData(QWidget* editor, const QModelIndex& index) const
{
  QStyledItemDelegate::setEditorData(editor, index);
}

void Delegate::setModelData(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const
{
  QStyledItemDelegate::setModelData(editor, model, index);
}
