#pragma once

#include <QDialog>


namespace Ui {
class DateTimeChooser1;
}

class DateTimeChooser1 : public QDialog
{
  Ui::DateTimeChooser1 *ui;

  Q_OBJECT

public:
  void SetCurrentDateTime();
  QDateTime GetDateTime() const;

public:
  explicit DateTimeChooser1(QWidget *parent = 0);
  ~DateTimeChooser1();

};
