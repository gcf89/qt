#include <QDir>
#include <QStringList>
#include <QDebug>
#include <QDesktopServices>
#include <QUrl>

#include "MainWindow.h"
#include "ui_MainWindow.h"

MainWindow::MainWindow(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::MainWindow)
{
  ui->setupUi(this);

  ui->lineEditWhere->setText(R"(\\10.0.61.48\builds\Gormost\Windows)");
  mDelimiter = "____"; // 4 underscore
}

MainWindow::~MainWindow()
{
  delete ui;
}

void MainWindow::on_pushButton_clicked()
{
  auto dp = ui->lineEditWhere->text();
  auto tgt = ui->lineEditWhat->text();

  QDir d(dp);
  if (!d.exists()) {
    ui->lineEditResult->setText("No such dir!");
    return;
  }

  auto items = d.entryList();
  bool ok = false;
  for (auto it=items.constBegin(); it!=items.constEnd(); ++it) {
    if (it->section(mDelimiter, 1).contains(tgt)) {
      auto fullp = dp.append(QDir::separator()).append(*it);
      ui->lineEditResult->setText(fullp);
      ok = true;
      break;
    }
  }

  if (!ok) {
    ui->lineEditResult->setText("No match!");
  }
}

void MainWindow::on_pushButtonOpen_clicked()
{
  QDesktopServices::openUrl(QUrl::fromLocalFile(
          QDir::toNativeSeparators(ui->lineEditResult->text())));
}
