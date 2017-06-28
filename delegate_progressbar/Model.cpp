#include "Model.h"

Model::Model(QObject* parent)
  : QAbstractTableModel(parent)
{
  for (int i=0; i<100; ++i)
    mData << i;
}

QVariant Model::data(const QModelIndex& index, int role) const
{
  if (index.isValid() && role == Qt::DisplayRole) {
    if (-1 < index.row() && index.row() < mData.size()) {
      return mData.at(index.row());
    }
  }

  return QVariant();
}

int Model::columnCount(const QModelIndex& parent) const
{
  return 1;
}

int Model::rowCount(const QModelIndex& parent) const
{
  return mData.size();
}
