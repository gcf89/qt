#include "FormSplash.h"
#include <QApplication>

int main(int argc, char *argv[])
{
  QApplication a(argc, argv);
  FormSplash w;
  w.show();

  return a.exec();
}
