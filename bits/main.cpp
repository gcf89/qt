#include <limits>


#include <QCoreApplication>
#include <QDebug>
#include <QString>


int main(int argc, char *argv[])
{
  Q_UNUSED(argc)
  Q_UNUSED(argv)
//  QCoreApplication a(argc, argv);
//  return a.exec();

//  qDebug() << std::numeric_limits<unsigned char>::digits
//           << std::numeric_limits<int>::digits;


  // set 'num' bit for 'b' to 1 from left to right
  int v = std::numeric_limits<uchar>::digits;
  int digits = 4;
  uchar b = 0;
  QString binRepr;

  // init value
  int mask = (1 << (v-1) );
  qDebug() << "mask:" << mask << binRepr.setNum(mask, 2);

  while (digits-- > 0) {
    b = b >> 1 | (1 << (v-1) );
    qDebug() << b
             << binRepr.setNum((uint)b, 2);
  }

  qDebug() << 5 % 8 << 8%8 << 9%8 << 0%8;



//  qDebug() << b;

  return 0;
}
