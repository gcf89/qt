#pragma once

#include <QWidget>
#include <QStringList>

namespace Ui {
class FormCustomSearch2;
}

class FormCustomSearch2 : public QWidget
{
  Ui::FormCustomSearch2*          ui;
  QStringList                     mFullDict;
  QList<FormCustomSearch2*>       mSearchWidgets;
  // history container

  Q_OBJECT

public:
  explicit FormCustomSearch2(bool isReadOnly = false,
                             const QString& defaultText = "",
                             QWidget* parent = 0);
  ~FormCustomSearch2();

private slots:
  void on_textEdit_textChanged();
  void on_toolButtonClear_clicked();
  void on_toolButtonSearch_clicked();

private:
};

