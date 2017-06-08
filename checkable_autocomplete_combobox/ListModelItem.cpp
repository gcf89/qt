#include "ListModelItem.h"


void ListModelItem::SetRepr(const QString &_Repr)
{
  mRepr = _Repr;
}

void ListModelItem::SetCheckState(Qt::CheckState _CheckState)
{
  mCheckState = _CheckState;
}

void ListModelItem::AddData(const QVariant &_Data)
{
  mData.append(_Data);
}

void ListModelItem::InsertData(int _Pos, const QVariant &_Data)
{
  Q_ASSERT(_Pos < mData.size());
  mData.insert(_Pos, _Data);
}

QVariant ListModelItem::GetDataCopy(int _Pos) const
{
  Q_ASSERT(_Pos < mData.size());
  return mData.at(_Pos);
}

QString ListModelItem::GetRepr() const
{
  return mRepr;
}

Qt::CheckState ListModelItem::GetCheckState() const
{
  return mCheckState;
}

QVariant ListModelItem::data(const QModelIndex &index, int role) const
{
  Q_UNUSED(index)

  switch (role) {
  case Qt::DisplayRole:
    return mRepr;
  case Qt::CheckStateRole:
    return mCheckState;
  default:
    return QVariant();
  }
}

Qt::ItemFlags ListModelItem::flags(const QModelIndex &index) const
{
  Q_UNUSED(index)

  return Qt::ItemIsSelectable | Qt::ItemIsUserCheckable | Qt::ItemIsEnabled;
}

bool ListModelItem::setData(const QModelIndex &index, const QVariant &value, int role)
{
  Q_UNUSED(index)

  switch (role) {
  case Qt::CheckStateRole:
    mCheckState = value.toBool() ? Qt::Checked : Qt::Unchecked;
    return true;
  default:
    return false;
  }
}


ListModelItem::ListModelItem()
{
}

ListModelItem::ListModelItem(const QString &_Repr, const QVariantList &_Data, Qt::CheckState _CheckState)
{
  mRepr = _Repr;
  mData = _Data;
  mCheckState = _CheckState;
}

ListModelItem::~ListModelItem()
{
}
