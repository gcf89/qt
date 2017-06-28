#ifndef MODEL_H
#define MODEL_H

#include <QAbstractTableModel>
#include <QVariant>
#include <QList>

class Model : public QAbstractTableModel
{
  Q_OBJECT

  QList<int> mData;

public:
  Model(QObject* parent = 0);

  QVariant data(const QModelIndex &index, int role) const override;
  int columnCount(const QModelIndex &parent) const override;
  int rowCount(const QModelIndex &parent) const override;
};

#endif // MODEL_H
