#ifndef FORM_H
#define FORM_H

#include <QWidget>
#include <QProgressBar>

namespace Ui {
class Form;
}

class Form : public QWidget
{
  Q_OBJECT

public:
  QProgressBar* PrimaryProgressBar() const;
  QProgressBar* SecondaryProgressBar() const;

public:
  explicit Form(QWidget *parent = 0);
  ~Form();

private:
  Ui::Form *ui;
};

#endif // FORM_H
