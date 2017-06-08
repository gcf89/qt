#include <QCompleter>
#include <QAbstractItemModel>
#include <QStringListModel>

#include "MainWindow.h"
#include "ui_MainWindow.h"

#include "ListModel.h"


MainWindow::MainWindow(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::MainWindow)
{
  ui->setupUi(this);

  int cnt = 0;
  ui->comboBox->AddItem(QIcon(), "Desperate", ++cnt);
  ui->comboBox->AddItem(QIcon(), "Monro", ++cnt);
  ui->comboBox->AddItem(QIcon(), "Virgin", ++cnt);
  ui->comboBox->AddItem(QIcon(), "Apple", ++cnt);
  ui->comboBox->AddItem(QIcon(), "Safari", ++cnt);

//  ui->comboBox->setEditable(false);


  // combo 2
  QStringList items;
  items << "California"
        << "New York"
        << "Oregon"
        << "Russia"
        << "Moscow"
        << "Canada";

  QCompleter* cmpl = new QCompleter(items, this);

  ui->comboBox_2->insertItems(0, items);
  ui->comboBox_2->setCompleter(cmpl);
  ui->comboBox_2->setEditable(true);

  // list view
  auto model = new QStringListModel(items, this);
  mSFModel = new QSortFilterProxyModel(this);
  mSFModel->setFilterCaseSensitivity(Qt::CaseInsensitive);
  mSFModel->setSourceModel(model);
  ui->listView->setModel(mSFModel);

  // list view 2
  mSFModel2 = new QSortFilterProxyModel(this);
  mSFModel2->setFilterCaseSensitivity(Qt::CaseInsensitive);
  mSFModel2->setDynamicSortFilter(true);
  auto listmodel = new ListModel(this);
  mSFModel2->setSourceModel(listmodel);
  ui->listView_2->setModel(mSFModel2);
//  ui->listView_2->setModel(listmodel);

  int cnt1 = 0;
  listmodel->AddItem(new ListModelItem("Jack", QVariantList() << ++cnt1));
  listmodel->AddItem(new ListModelItem("Rose", QVariantList() << ++cnt1));
  listmodel->AddItem(new ListModelItem("Peter", QVariantList() << ++cnt1));


}

MainWindow::~MainWindow()
{
  delete ui;
}

void MainWindow::on_lineEdit_textChanged(const QString& _NewText)
{
  mSFModel->setFilterWildcard(_NewText);
  mSFModel2->setFilterWildcard(_NewText);
}
