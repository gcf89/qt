#include <QFile>
#include <QDebug>

#include "FormCustomSearch.h"
#include "ui_FormCustomSearch.h"

#include "CustomSearchDelegate.h"


FormCustomSearch::FormCustomSearch(QWidget *parent)
  : QWidget(parent)
  , ui(new Ui::FormCustomSearch)
  , mDictModel(new QStringListModel(this))
{
  ui->setupUi(this);
  ui->listView->setModel(mDictModel);
  ui->listView->setVisible(false);
  ui->listView->setItemDelegate(new CustomSearchDelegate(ui->lineEdit));

//  QFile f(":/data/word_rus.txt");
//  if (f.open(QIODevice::Text | QIODevice::ReadOnly)) {
//    while (!f.atEnd()) {
//      mFullDict << f.readLine().trimmed();
//    }
//  } else {
    mFullDict << "i'm gonna write this app" << "take some cake" << "weather" << "don't be shy";
//    qDebug() << "ERR: cannot open dict";
//  }

  mFullDict << QString::fromUtf8("Целое большое предложение из слов");
}

FormCustomSearch::~FormCustomSearch()
{
  delete ui;
}

void FormCustomSearch::SetDict(const QStringList& dict)
{
  if (mDictModel) {
    mDictModel->setStringList(dict);
  }
}

QStringList FormCustomSearch::GetRelevant() const
{
  // get from model
  return QStringList();
}


//void FormCustomSearch::on_lineEdit_editingFinished()
//{
//  mDictModel->removeRows(0, mDictModel->rowCount());
//  QString st = ui->lineEdit->text();
//  ui->listView->setVisible(!st.isEmpty());

//  QStringList matches;
//  if (!st.isEmpty()) {
//    foreach (const QString& str, mDictModel->stringList()) {
//      if (str.contains(st)) {
//        matches << str;
//      }
//    }
//  }
//  mDictModel->setStringList(matches);
//}

void FormCustomSearch::on_lineEdit_returnPressed()
{
  emit ReturnPressed(QStringList());
}

void FormCustomSearch::on_listView_clicked(const QModelIndex &index)
{
  // complete text, mark object, center map, max zoom

  if (index.isValid()) {
    ui->lineEdit->setText(index.data().toString());
  }
}

void FormCustomSearch::on_lineEdit_textChanged(const QString &st)
{
  mDictModel->removeRows(0, mDictModel->rowCount());
//  QString st = ui->lineEdit->text();
  ui->listView->setVisible(!st.isEmpty());

  QStringList matches;
  if (!st.isEmpty()) {
    foreach (const QString& str, mFullDict) {
      if (str.contains(st)) {
        matches << str;
      }
    }
  }
  mDictModel->setStringList(matches);
}
