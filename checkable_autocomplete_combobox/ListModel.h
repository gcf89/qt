#pragma once

#include <QAbstractListModel>
#include <QModelIndex>


#include "ListModelItem.h"

/**
 * @brief The ListModel class Shows checkable items as list,
 * that could contain arbitrary amout of extra data
 */
class ListModel : public QAbstractListModel
{
  QList<ListModelItem*> mItems;

  Q_OBJECT

public:
  void Clear();
  void AddItem(ListModelItem* _Item);

public:
  QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
  QVariant data(const QModelIndex &index, int role) const override;
  int rowCount(const QModelIndex &parent) const override;
  Qt::ItemFlags flags(const QModelIndex &index) const override;
  bool setData(const QModelIndex &index, const QVariant &value, int role) override;

public:
  ListModel(QObject* parent = 0);
  virtual ~ListModel();
};
