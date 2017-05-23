#include <QTime>
#include <QDebug>

class A
{
public:
  A() {}

  void init_rand()
  {
    qsrand(QTime::currentTime().msec());
  }

  void print_n_rand_numbers(int n)
  {
    for (int i=0; i<n; ++i)
      qDebug() << qrand()%n;
  }
};

int main(int argc, char *argv[])
{
  A a;
  a.init_rand();
  a.print_n_rand_numbers(5);

  return 0;
}
