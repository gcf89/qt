#include "FormCustomSearch2.h"
#include "ui_FormCustomSearch2.h"


FormCustomSearch2::FormCustomSearch2(bool isReadOnly,
                                     const QString& defaultText,
                                     QWidget* parent) :
  QWidget(parent),
  ui(new Ui::FormCustomSearch2)
{
  ui->setupUi(this);
  ui->textEdit->setReadOnly(!isReadOnly);
//  ui->textEdit->setPlainText(defaultText);
  ui->textEdit->setHtml(defaultText);

  mFullDict << "i'm gonna write this app" << "take some cake" << "weather" << "don't be shy";
  mFullDict << QString::fromUtf8("Целое большое предложение из слов");
}

FormCustomSearch2::~FormCustomSearch2()
{
  delete ui;
}


void FormCustomSearch2::on_textEdit_textChanged()
{
  foreach (auto w, mSearchWidgets) {
    w->setVisible(false);
    w->deleteLater();
  }
  mSearchWidgets.clear();


  if (ui->textEdit->toPlainText().isEmpty()) {
    ui->toolButtonSearch->setIcon(QIcon(":/icons/magnifier_gray_32.png"));
  } else {
    ui->toolButtonSearch->setIcon(QIcon(":/icons/magnifier_blue_32.png"));

    QString st = ui->textEdit->toPlainText();
    QStringList matches;
    if (!st.isEmpty()) {
      foreach (const QString& str, mFullDict) {
        if (str.contains(st)) {
          matches << str;
        }
      }
    }
    // todo: search in history

    // todo: add limit
    foreach (QString match, matches) {

      int ind = match.indexOf(st);
      QString firstPart = match.mid(0, ind);
      QString boldPart = match.mid(ind, st.size());
      QString lastPart = match.mid(ind + st.size());

      QString txt = QString("<p><span style=\"color:#A9A9A9\">%1</span>"
                            "<strong>%2</strong>"
                            "<span style=\"color:#A9A9A9\">%3</span></p>");

      auto w = new FormCustomSearch2(false, txt.arg(firstPart, boldPart, lastPart), this);
      mSearchWidgets << w;
      ui->layoutMain->addWidget(w);
    }
  }
}

void FormCustomSearch2::on_toolButtonClear_clicked()
{
  ui->textEdit->clear();
}

void FormCustomSearch2::on_toolButtonSearch_clicked()
{

}
