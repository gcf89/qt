#pragma once

#include <QWidget>
#include <QStringList>
#include <QIcon>
#include <QMouseEvent>

#include "FormCustomSearchResult.h"


namespace Ui {
class FormCustomSearch2;
}

class FormCustomSearch2 : public QWidget
{
  Ui::FormCustomSearch2*          ui;
  int                             mShowLimit;

  QStringList                     mFullDict;
  QList<FormCustomSearchResult*>  mSearchWidgets;
  QIcon                           mMagnifierGrayIcon;
  QIcon                           mMagnifierBlueIcon;
  QStringList                     mSearchResults;

  Q_OBJECT

public:
  explicit FormCustomSearch2(QWidget* parent = 0, int showLimit = 3);
  ~FormCustomSearch2();

  void SetShowLimit(int showLimit);
  void InitDict(const QStringList& strings);
  QStringList GetSearchResults() const { return mSearchResults; }

private slots:
  void on_lineEdit_textChanged(const QString &st);
  void on_toolButtonClear_clicked();
  void on_toolButtonSearch_clicked();
  void OnSearchResultMouseReleased(QMouseEvent* event);

private:
  void ClearResults();
};

