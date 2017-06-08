
#include "ListModel.h"


void ListModel::Clear()
{
  qDeleteAll(mItems);
  mItems.clear();
}

void ListModel::AddItem(ListModelItem* _Item)
{
  beginInsertRows(QModelIndex(), mItems.size(), mItems.size());
  mItems << _Item;
  endInsertRows();
}

QVariant ListModel::headerData(int section, Qt::Orientation orientation, int role) const
{
  Q_UNUSED(section)
  Q_UNUSED(orientation)
  Q_UNUSED(role)

  return "Abrakadabra";
}

QVariant ListModel::data(const QModelIndex &index, int role) const
{
  if (index.isValid() && 0 <= index.row() && index.row() < mItems.size()) {
    return mItems.at(index.row())->data(index, role);
  }
  return QVariant();
}

int ListModel::rowCount(const QModelIndex &parent) const
{
  Q_UNUSED(parent)

  return mItems.size();
}

Qt::ItemFlags ListModel::flags(const QModelIndex &index) const
{
  if (index.isValid() && 0 <= index.row() && index.row() < mItems.size()) {
    return mItems.at(index.row())->flags(index);
  }
  return QAbstractListModel::flags(index)/* | Qt::ItemIsDragEnabled | Qt::ItemIsDropEnabled*/;
}

bool ListModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
  if (index.isValid()) {
    return mItems.at(index.row())->setData(index, value, role);
  }
  return QAbstractListModel::setData(index, value, role);
}


ListModel::ListModel(QObject *parent)
  : QAbstractListModel(parent)
{
}

ListModel::~ListModel()
{
  Clear();
}
