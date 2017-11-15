#pragma once

#include <QWidget>
#include <QStringList>
#include <QStringListModel>

namespace Ui {
class FormCustomSearch;
}

class FormCustomSearch : public QWidget
{
  Ui::FormCustomSearch*         ui;
  QStringListModel*             mDictModel;
  QStringList                   mFullDict;

  Q_OBJECT

public:
  explicit FormCustomSearch(QWidget *parent = 0);
  ~FormCustomSearch();

  void SetDict(const QStringList& dict);
  QStringList GetRelevant() const;

signals:
  void ReturnPressed(const QStringList& selected);

private slots:
//  void on_lineEdit_editingFinished();
  void on_lineEdit_returnPressed();
  void on_listView_clicked(const QModelIndex &index);

  void on_lineEdit_textChanged(const QString &arg1);

private:
};
