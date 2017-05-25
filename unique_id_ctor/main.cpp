#include <QDebug>
#include <QSet>

class A
{
  int mId;

public:
  A() : mId(GetUniqueId()) {}
  ~A() { FreeUniqueId(mId); }

  static int GetUniqueId()
  {
    int i=0;
    while (UniqueIds.contains(++i));
    UniqueIds.insert(i);
    return i;
  }

  static void FreeUniqueId(int id)
  {
    UniqueIds.remove(id);
  }

  static void PrintUsedIds()
  {
    qDebug() << "Used unique ids:";
    foreach (int id, UniqueIds)
      qDebug() << id;
  }

private:
  static QSet<int> UniqueIds;
};

/**
 * @brief A::UniqueIds Important! Static variables should be
 * defined outside class where they are declared! That's the rule!
 */
QSet<int> A::UniqueIds;



int main(int /*argc*/, char **/*argv*/)
{
  A::PrintUsedIds();

  A *a = new A;
  A *a1 = new A;
  A *a2 = new A;

  A::PrintUsedIds();

  delete a;
  delete a1;
  delete a2;

  A::PrintUsedIds();

  return 0;
}
