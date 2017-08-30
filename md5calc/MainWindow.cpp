#include <QFileDialog>
#include <QCryptographicHash>
#include <QMessageBox>
#include <QDebug>

#include "MainWindow.h"
#include "ui_MainWindow.h"



MainWindow::MainWindow(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::MainWindow)
{
  ui->setupUi(this);

  ui->comboBox->addItem(QString::fromUtf8("MD5"), QCryptographicHash::Md5);
  ui->comboBox->addItem(QString::fromUtf8("SHA_1"), QCryptographicHash::Sha1);
  ui->comboBox->addItem(QString::fromUtf8("SHA_256"), QCryptographicHash::Sha256);
  ui->comboBox->addItem(QString::fromUtf8("SHA_512"), QCryptographicHash::Sha512);
  ui->comboBox->addItem(QString::fromUtf8("SHA3_256"), QCryptographicHash::Sha3_256);
  ui->comboBox->addItem(QString::fromUtf8("SHA3_512"), QCryptographicHash::Sha3_512);
}

MainWindow::~MainWindow()
{
  delete ui;
}

void MainWindow::on_pushButtonChooseFile_clicked()
{
  mFilePath = QFileDialog::getOpenFileName(this, QString::fromUtf8("Choose file..."));
}

void MainWindow::on_pushButtonCalcMD5_clicked()
{
  QFile f(mFilePath);
  if (!f.open(QIODevice::ReadOnly)) {
    QMessageBox::warning(
          this, QString::fromUtf8("Warning!"),
          QString::fromUtf8("Cannot open file!"));
    return;
  }

  QByteArray md5 = QCryptographicHash::hash(
        f.readAll(),
        (QCryptographicHash::Algorithm)ui->comboBox->currentData().toInt());

  ui->textEdit->append(mFilePath);
//  ui->textEdit->append();

  QString fromUtf8("%0: ");
  fromUtf8 = fromUtf8.arg(ui->comboBox->currentText(), 10, QLatin1Char('>'));
//  fromUtf8 = fromUtf8.arg(md5.toHex());
  fromUtf8.append(md5.toHex());
  ui->textEdit->append(fromUtf8);
}
