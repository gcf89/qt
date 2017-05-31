#include "DateTimeChooser1.h"
#include "ui_DateTimeChooser1.h"


void DateTimeChooser1::SetCurrentDateTime()
{
//  ui->calendarWidget->setCurrentPage(QDate::currentDate().year(), QDate::currentDate().month());
  ui->calendarWidget->setSelectedDate(QDate::currentDate());
  ui->spinboxHour->setValue(QTime::currentTime().hour());
  ui->spinboxMinute->setValue(QTime::currentTime().minute());
}

QDateTime DateTimeChooser1::GetDateTime() const
{
  QDateTime res;
  res.setDate(ui->calendarWidget->selectedDate());
  QTime time;
  time.setHMS(ui->spinboxHour->value(), ui->spinboxMinute->value(), 0);
  res.setTime(time);
  return res;
}


DateTimeChooser1::DateTimeChooser1(QWidget *parent) :
  QDialog(parent),
  ui(new Ui::DateTimeChooser1)
{
  ui->setupUi(this);
  setWindowFlags(Qt::FramelessWindowHint);
}

DateTimeChooser1::~DateTimeChooser1()
{
  delete ui;
}
