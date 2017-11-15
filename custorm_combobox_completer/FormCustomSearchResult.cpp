#include <QDebug>

#include "FormCustomSearchResult.h"
#include "ui_FormCustomSearchResult.h"

FormCustomSearchResult::FormCustomSearchResult(const QString& html,
                                               const QString& txt,
                                               QWidget *parent)
  : QWidget(parent)
  , ui(new Ui::FormCustomSearchResult)
  , mResultTxt(txt)
{
  ui->setupUi(this);
  ui->label_2->setText(html);

  connect(ui->label, SIGNAL(MouseReleased(QMouseEvent*)),
          this, SIGNAL(MouseReleased(QMouseEvent*)));
  connect(ui->label_2, SIGNAL(MouseReleased(QMouseEvent*)),
          this, SIGNAL(MouseReleased(QMouseEvent*)));
}

FormCustomSearchResult::~FormCustomSearchResult()
{
  delete ui;
}
