#include <QDebug>

#include "FormCustomSearch2.h"
#include "ui_FormCustomSearch2.h"


FormCustomSearch2::FormCustomSearch2(QWidget *parent,
                                     int showLimit)
  : QWidget(parent)
  , ui(new Ui::FormCustomSearch2)
  , mShowLimit(showLimit)
{
  ui->setupUi(this);

  mMagnifierGrayIcon = QIcon(":/icons/magnifier_gray_32.png");
  mMagnifierBlueIcon = QIcon(":/icons/magnifier_blue_32.png");

  mFullDict << "i'm gonna write this app" << "take some cake" << "weather" << "don't be shy";
  mFullDict << QString::fromUtf8("Целое большое предложение из слов");
}

FormCustomSearch2::~FormCustomSearch2()
{
  delete ui;
}

void FormCustomSearch2::SetShowLimit(int showLimit)
{
  mShowLimit = showLimit;
}

void FormCustomSearch2::InitDict(const QStringList& strings)
{
  mFullDict = strings;
}


void FormCustomSearch2::on_lineEdit_textChanged(const QString &st)
{
  ClearResults();

  if (st.isEmpty()) {
    ui->toolButtonSearch->setIcon(mMagnifierGrayIcon);
  } else {
    ui->toolButtonSearch->setIcon(mMagnifierBlueIcon);

    QStringList matches;
    if (!st.isEmpty()) {
      foreach (const QString& str, mFullDict) {
        if (str.contains(st)) {
          matches << str;
        }
      }
    }

    // todo: add limit
    for (int i=0; i<matches.size() && i<mShowLimit; ++i) {
      const QString& match = matches.at(i);

      int ind = match.indexOf(st);
      QString firstPart = match.mid(0, ind);
      QString boldPart = match.mid(ind, st.size());
      QString lastPart = match.mid(ind + st.size());

      QString txt = QString("<p><span style=\"color:#A9A9A9\">%1</span>"
                            "<strong>%2</strong>"
                            "<span style=\"color:#A9A9A9\">%3</span></p>");

      auto w = new FormCustomSearchResult(txt.arg(firstPart, boldPart, lastPart),
                                          match, this);
      connect(w, SIGNAL(MouseReleased(QMouseEvent*)),
              this, SLOT(OnSearchResultMouseReleased(QMouseEvent*)));

      mSearchWidgets << w;
      ui->layoutMain->insertWidget(1, w);
      mSearchResults << match;
    }
  }
}

void FormCustomSearch2::on_toolButtonClear_clicked()
{
  ui->lineEdit->clear();
}

void FormCustomSearch2::on_toolButtonSearch_clicked()
{
  // TODO: take all matching string and select approp. objects
}

void FormCustomSearch2::OnSearchResultMouseReleased(QMouseEvent* event)
{
  Q_UNUSED(event)

  if (auto w = qobject_cast<FormCustomSearchResult*>(sender())) {
    ui->lineEdit->setText(w->GetResultText());
  }
}

void FormCustomSearch2::ClearResults()
{
  foreach (auto w, mSearchWidgets) {
    w->setVisible(false);
    w->deleteLater();
  }
  mSearchWidgets.clear();
  mSearchResults.clear();
}
