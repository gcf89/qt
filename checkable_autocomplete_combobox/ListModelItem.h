#pragma once

#include <QVariant>
#include <QVariantList>
#include <QString>
#include <QModelIndex>

/**
 * @brief The ListModelItem class Checkable item, that could
 * contain arbitrary number of extra data
 */
class ListModelItem
{
  Qt::CheckState mCheckState;
  QString mRepr;
  QVariantList mData;


public:
  void SetRepr(const QString& _Repr);
  void SetCheckState(Qt::CheckState _CheckState);
  void AddData(const QVariant& _Data);
  void InsertData(int _Pos, const QVariant& _Data);
  QVariant GetDataCopy(int _Pos) const;
  QString GetRepr() const;
  Qt::CheckState GetCheckState() const;

public:
  QVariant data(const QModelIndex &index, int role) const;
  Qt::ItemFlags flags(const QModelIndex &index) const;
  bool setData(const QModelIndex &index, const QVariant &value, int role);

public:
  ListModelItem();
  ListModelItem(const QString& _Repr, const QVariantList& _Data, Qt::CheckState _CheckState = Qt::Unchecked);
  ~ListModelItem();
};
