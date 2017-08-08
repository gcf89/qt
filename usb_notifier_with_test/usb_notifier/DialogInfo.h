#pragma once

#include <QDialog>
#include <QPixmap>
#include <QColor>

namespace Ui {
class DialogInfo;
}

class DialogInfo : public QDialog
{
  Q_OBJECT

  Ui::DialogInfo *ui;
  QPixmap       mGreenPic;
  QPixmap       mRedPic;
  QColor        mColorGreen;

public slots:
  void ShowInfo(QString message, bool needLock);

private:
  void PrepareGui();

public:
  explicit DialogInfo(QWidget *parent = 0);
  ~DialogInfo();
};
