#include <QString>
#include <QtTest>

#include "Duration.h"

class DurationTest : public QObject
{
  Q_OBJECT

  Duration d;

public:
  DurationTest();

private Q_SLOTS:
  void initTestCase();
  void cleanupTestCase();
  void testCase1();
};

DurationTest::DurationTest()
{
  d.SetMax(0,0,0,0,0,0);
}

void DurationTest::initTestCase()
{
}

void DurationTest::cleanupTestCase()
{
}

void DurationTest::testCase1()
{
  QDateTime cur = QDateTime::currentDateTime();

  QCOMPARE(d.Get(QDateTime(), QDateTime()), QString(""));
  QCOMPARE(d.Get(cur, cur.addSecs(-1)), QString(""));
  QCOMPARE(d.Get(cur, cur.addSecs(1)), QString("1s"));
  QCOMPARE(d.Get(cur, cur.addSecs(60)), QString("1m 0s"));
  QCOMPARE(d.Get(cur, cur.addSecs(60*60)), QString("1h 0m 0s"));
  QCOMPARE(d.Get(cur, cur.addSecs(60*60*24)), QString("1d 0h 0m 0s"));
  QCOMPARE(d.Get(cur, cur.addSecs(60*60*24*364)), QString("364d 0h 0m 0s"));
  QCOMPARE(d.Get(cur, cur.addSecs(60*60*24*365)), QString("1y 0d 0h 0m 0s"));
  qint64 bigYear = 60L*60L*24L*365L*33L;
  QCOMPARE(d.Get(cur, cur.addSecs(bigYear)), QString("33y 0d 0h 0m 0s"));
  QCOMPARE(d.Get(cur, cur.addSecs(59)), QString("59s"));
  QCOMPARE(d.Get(cur, cur.addSecs(60*60*24 + 1)), QString("1d 0h 0m 1s"));
}

QTEST_APPLESS_MAIN(DurationTest)

#include "tst_DurationTest.moc"
