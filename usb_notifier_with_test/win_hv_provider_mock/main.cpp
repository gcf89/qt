#include <iostream>

#include <QCoreApplication>
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <QTextCodec>

int main(int argc, char *argv[])
{
  QString sourcePath;
  if (argc == 2) { // try read 'source path' from args
    sourcePath = QString(argv[1]);
  } else {
    qDebug() << "ERR: no source file passed";
    exit(-1);
  }

  if (sourcePath.startsWith("./")) {
    sourcePath.remove(0, 1);
    sourcePath.prepend(qApp->applicationDirPath());
  }

  QFile f(sourcePath);
  if (!f.open(QIODevice::Text | QIODevice::ReadOnly)) {
    qDebug() << "ERR: cannot open source file";
    exit(-2);
  }

//  QTextStream in(&f);
//  in.setCodec(QTextCodec::codecForName("UTF8"));

//  std::cout << in.readAll().toStdString();
#ifdef Q_OS_WIN
  std::cout << f.readAll().toStdString();
#endif
  return 0;
}
