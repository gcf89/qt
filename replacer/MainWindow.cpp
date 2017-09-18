#include <QFileDialog>
#include <QFile>
#include <QTextStream>

#include "MainWindow.h"
#include "ui_MainWindow.h"

MainWindow::MainWindow(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::MainWindow)
{
  ui->setupUi(this);
  ui->statusBar->showMessage("Ready");
  ui->lineEditReplacement->setToolTip("%i - integer from 0 to N");
}

MainWindow::~MainWindow()
{
  delete ui;
}

void MainWindow::on_pushButtonOpen_clicked()
{
  mFilePath = QFileDialog::getOpenFileName(this, QString::fromUtf8("Choose file..."));
  QFile f(mFilePath);
  if (f.open(QIODevice::Text | QIODevice::ReadOnly)) {
    ui->textEdit->setPlainText(f.readAll());
  } else {
    ui->statusBar->showMessage(f.errorString());
  }
}

void MainWindow::on_pushButtonReplace_clicked()
{
  mStrTarget = ui->lineEditTarget->text();
  mStrReplacement = ui->lineEditReplacement->text();
  QString source = ui->textEdit->toPlainText();

  bool counterEnabled = mStrReplacement.contains("%i");
  int counter = 0;

  int ind = 0;
  QString replacement;

  while ( (ind = source.indexOf(mStrTarget, ind)) != -1 ) {
    if (counterEnabled) {
      replacement = mStrReplacement;
      replacement = replacement.replace("%i", QString::number(counter++));
    } else {
      replacement = mStrReplacement;
    }
    source = source.replace(ind, mStrTarget.size(), replacement);
    ind += replacement.size();
  }

  ui->textEdit->clear();
  ui->textEdit->setPlainText(source);

  ui->statusBar->showMessage("Done");
}

void MainWindow::on_pushButtonSave_clicked()
{
  QFile f(mFilePath);
  if (f.open(QIODevice::Text | QIODevice::WriteOnly)) {
    QTextStream out(&f);
    out << ui->textEdit->toPlainText();
  } else {
    ui->statusBar->showMessage(f.errorString());
  }
}
