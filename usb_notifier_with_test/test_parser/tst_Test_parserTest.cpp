#include <QString>
#include <QtTest>
#include <QCoreApplication>

#include "../usb_notifier/Core.h"

class Test_parserTest : public QObject
{
  Q_OBJECT

public:
  Test_parserTest();

private Q_SLOTS:
  void testTrashFirst();
  void testUnFinishedLine();
  void testTrashLast();
  void testInsertNoRestriction();
  void testInsertGood();
  void testInsertBad();
  void testRemoveGood();
  void testRemoveBad();
  void testInsertMultiGoodBad();
  void testRemoveMultiGoodBad();
  void testInitialData();
};

Test_parserTest::Test_parserTest()
{
}

void Test_parserTest::testTrashFirst()
{
  QString data = "jasdf0asd09fausfasdf[]{}{}{}{dfa90sd8fa98s-d}{Dfa09s8df}{Dfa-09id{}"
                 "{\"id\":3,\"value\":{\"raw\":{\"device_descriptor\":{\"vendor_id\":\"8564\",\"product_id\":\"1000\",\"dev_class_id\""
      // serial number removed from first line
                 ":\"00\",\"dev_subclass_id\":\"00\",\"dev_protocol_id\":\"00\",:\"DFT5NG1T\"},\"interfaces\":[{\"if_class_id\":\""
                 "08\",\"if_subclass_id\":\"06\",\"if_protocol_id\":\"50\"}]}},\"text\":\"USB устройство присоединено\",\"timestamp\":\"2017-08-06T07:23:04\"},"
                 "{\"id\":3,\"value\":{\"raw\":{\"device_descriptor\":{\"vendor_id\":\"8564\",\"product_id\":\"1000\",\"dev_class_id\""
                 ":\"00\",\"dev_subclass_id\":\"00\",\"dev_protocol_id\":\"00\",\"serial_number\":\"DFT5NG1T\"},\"interfaces\":[{\"if_class_id\":\""
                 "08\",\"if_subclass_id\":\"06\",\"if_protocol_id\":\"50\"}]}},\"text\":\"USB устройство разрешено\",\"timestamp\":\"2017-08-06T07:23:04\"},"
                 "{\"id\":3,\"value\":{\"raw\":{\"device_descriptor\":{\"vendor_id\":\"8564\",\"product_id\":\"1000\",\"dev_class_id\""
                 ":\"00\",\"dev_subclass_id\":\"00\",\"dev_protocol_id\":\"00\",\"serial_number\":\"DFT5NG2T\"},\"interfaces\":[{\"if_class_id\":\""
                 "08\",\"if_subclass_id\":\"06\",\"if_protocol_id\":\"50\"}]}},\"text\":\"USB устройство присоединено\",\"timestamp\":\"2017-08-06T07:23:04\"},"
                 "{\"id\":3,\"value\":{\"raw\":{\"device_descriptor\":{\"vendor_id\":\"8564\",\"product_id\":\"1000\",\"dev_class_id\""
                 ":\"00\",\"dev_subclass_id\":\"00\",\"dev_protocol_id\":\"00\",\"serial_number\":\"DFT5NG2T\"},\"interfaces\":[{\"if_class_id\":\""
                 "08\",\"if_subclass_id\":\"06\",\"if_protocol_id\":\"50\"}]}},\"text\":\"USB устройство заблокировано\",\"timestamp\":\"2017-08-06T07:23:04\"},";

  Core c;
  c.mHWBad << "DFT5NG3T";
  c.mHWGood << "DFT5NG5T";
  c.mHWInstalledRemoved << "NEEDED";

  QVERIFY(c.Parse(data, 0));

  QCOMPARE(c.mHWBad.size(), 2);
  QCOMPARE(c.mHWGood.size(), 2);
  QCOMPARE(c.mHWInstalledRemoved.size(), 1);
}

void Test_parserTest::testUnFinishedLine()
{
  QString data1 = "{\"id\":3,\"value\":{\"raw\":{\"device_descriptor\":{\"vendor_id\":\"8564\",\"product_id\":\"1000\",\"dev_class_id\""
      ":\"00\",\"dev_subclass_id\":\"00\",\"de";

  QString data2 = "{\"id\":3,\"value\":{\"raw\":{\"device_descriptor\":{\"vendor_id\":\"8564\",\"product_id\":\"1000\",\"dev_class_id\""
      ":\"00\",\"dev_subclass_id\":\"00\",\"dev_protocol_id\":\"00\",\"serial_number\":\"DFT5NG1T\"},\"interfaces\":[{\"if_class_id\":\""
      "08\",\"if_subclass_id\":\"06\",\"if_protocol_id\":\"50\"}]}},\"text\":\"USB устройство заблокировано\",\"timestamp\":\"2017-08-06T07:23:04\"},";

  Core c;

  QVERIFY(!c.Parse(data1, 0));

  QCOMPARE(c.mHWBad.size(), 0);
  QCOMPARE(c.mHWGood.size(), 0);

  QVERIFY(c.Parse(data2, 0));

  QCOMPARE(c.mHWBad.size(), 1);
  QCOMPARE(c.mHWGood.size(), 0);
}

void Test_parserTest::testTrashLast()
{
  QString data = "{\"id\":3,\"value\":{\"raw\":{\"device_descriptor\":{\"vendor_id\":\"8564\",\"product_id\":\"1000\",\"dev_class_id\""
      ":\"00\",\"dev_subclass_id\":\"00\",\"dev_protocol_id\":\"00\",\"serial_number\":\"DFT5NG1T\"},\"interfaces\":[{\"if_class_id\":\""
      "08\",\"if_subclass_id\":\"06\",\"if_protocol_id\":\"50\"}]}},\"text\":\"USB устройство заблокировано\",\"timestamp\":\"2017-08-06T07:23:04\"},"
      "},\"}}asdf2f24f}{asdf{}{asdf}\"\"}}\"}{}\"}{\"jasdfoausd0f9ua0sduf0asjdf09ajs09dfjasjdf9ajs";

  Core c;

  QVERIFY(c.Parse(data, 0));

  QCOMPARE(c.mHWBad.size(), 1);
  QCOMPARE(c.mHWGood.size(), 0);
}

void Test_parserTest::testInsertNoRestriction()
{
  QString data = "{\"id\":3,\"value\":{\"raw\":{\"device_descriptor\":{\"vendor_id\":\"8564\",\"product_id\":\"1000\",\"dev_class_id\""
      ":\"00\",\"dev_subclass_id\":\"00\",\"dev_protocol_id\":\"00\",\"serial_number\":\"DFT5NG1T\"},\"interfaces\":[{\"if_class_id\":\""
      "08\",\"if_subclass_id\":\"06\",\"if_protocol_id\":\"50\"}]}},\"text\":\"USB устройство присоединено\",\"timestamp\":\"2017-08-06T07:23:04\"},";

  Core c;

  QVERIFY(c.Parse(data, 0));

  QCOMPARE(c.mHWBad.size(), 0);
  QCOMPARE(c.mHWGood.size(), 0);
}

void Test_parserTest::testInsertGood()
{
  QString data = "({\"id\":3,\"value\":{\"raw\":{\"device_descriptor\":{\"vendor_id\""
                 ":\"8564\",\"product_id\":\"1000\",\"dev_class_id\":\"00\",\"dev_subclass_id\""
                 ":\"00\",\"dev_protocol_id\":\"00\",\"serial_number\":\"DFT5NG1T\"},\"interfaces\""
                 ":[{\"if_class_id\":\"08\",\"if_subclass_id\":\"06\",\"if_protocol_id\":\"50\"}]}},"
                 "\"text\":\"USB устройство присоединено\",\"timestamp\":\"2017-08-06T07:23:04\"},"
                 "{\"id\":3,\"value\":{\"raw\":{\"device_descriptor\":{\"vendor_id\":\"8564\","
                 "\"product_id\":\"1000\",\"dev_class_id\":\"00\",\"dev_subclass_id\":\"00\","
                 "\"dev_protocol_id\":\"00\",\"serial_number\":\"DFT5NG1T\"},\"interfaces\":"
                 "[{\"if_class_id\":\"08\",\"if_subclass_id\":\"06\",\"if_protocol_id\":\"50\"}]}},"
                 "\"text\":\"USB устройство разрешено\",\"timestamp\":\"2017-08-06T07:23:04\"},)";

  Core c;
  QVERIFY(c.Parse(data, 0));

  QVERIFY(c.mHWBad.size() == 0);
  QVERIFY(c.mHWGood.size() == 1);
}

void Test_parserTest::testInsertBad()
{
  QString data = "({\"id\":3,\"value\":{\"raw\":{\"device_descriptor\":{\"vendor_id\""
                 ":\"8564\",\"product_id\":\"1000\",\"dev_class_id\":\"00\","
                 "\"dev_subclass_id\":\"00\",\"dev_protocol_id\":\"00\",\"serial_number\""
                 ":\"DFT5NG1T\"},\"interfaces\":[{\"if_class_id\":\"08\",\"if_subclass_id\""
                 ":\"06\",\"if_protocol_id\":\"50\"}]}},\"text\":\"USB устройство присоединено\","
                 "\"timestamp\":\"2017-08-06T07:23:04\"},{\"id\":3,\"value\":{\"raw\":"
                 "{\"device_descriptor\":{\"vendor_id\":\"8564\",\"product_id\":\"1000\","
                 "\"dev_class_id\":\"00\",\"dev_subclass_id\":\"00\",\"dev_protocol_id\":"
                 "\"00\",\"serial_number\":\"DFT5NG1T\"},\"interfaces\":[{\"if_class_id\":"
                 "\"08\",\"if_subclass_id\":\"06\",\"if_protocol_id\":\"50\"}]}},\"text\":"
                 "\"USB устройство заблокировано\",\"timestamp\":\"2017-08-06T07:23:04\"},)";

  Core c;
  QVERIFY(c.Parse(data, 0));

  QVERIFY(c.mHWBad.size() == 1);
  QVERIFY(c.mHWGood.size() == 0);
}

void Test_parserTest::testRemoveGood()
{
  QString data = "({\"id\":3,\"value\":{\"raw\":{\"device_descriptor\":"
                 "{\"vendor_id\":\"8564\",\"product_id\":\"1000\",\"dev_class_id\":"
                 "\"00\",\"dev_subclass_id\":\"00\",\"dev_protocol_id\":\"00\","
                 "\"serial_number\":\"DFT5NG1T\"},\"interfaces\":[{\"if_class_id\":"
                 "\"08\",\"if_subclass_id\":\"06\",\"if_protocol_id\":\"50\"}]}},"
                 "\"text\":\"USB устройство извлечено\",\"timestamp\":\"2017-08-06T07:23:04\"},)";

  Core c;
  QVERIFY(c.Parse(data, 0));

  QCOMPARE(c.mHWBad.size(), 0);
  QCOMPARE(c.mHWGood.size(), 0);
  QCOMPARE(c.mHWInstalledRemoved.size(), 1);
}

void Test_parserTest::testRemoveBad()
{
  QString data = "({\"id\":3,\"value\":{\"raw\":{\"device_descriptor\":"
                 "{\"vendor_id\":\"8564\",\"product_id\":\"1000\",\"dev_class_id\":"
                 "\"00\",\"dev_subclass_id\":\"00\",\"dev_protocol_id\":\"00\","
                 "\"serial_number\":\"DFT5NG1T\"},\"interfaces\":[{\"if_class_id\":"
                 "\"08\",\"if_subclass_id\":\"06\",\"if_protocol_id\":\"50\"}]}},"
                 "\"text\":\"USB устройство извлечено\",\"timestamp\":\"2017-08-06T07:23:04\"},)";

  Core c;
  c.mHWBad << "DFT5NG1T";
  c.mHWGood << "DFT5NG1T";

  QVERIFY(c.Parse(data, 0));

  QCOMPARE(c.mHWBad.size(), 0);
  QCOMPARE(c.mHWGood.size(), 1);
}

void Test_parserTest::testInsertMultiGoodBad()
{
  QString data = "{\"id\":3,\"value\":{\"raw\":{\"device_descriptor\":"
                 "{\"vendor_id\":\"8564\",\"product_id\":\"1000\",\"dev_class_id\""
                 ":\"00\",\"dev_subclass_id\":\"00\",\"dev_protocol_id\":\"00\","
                 "\"serial_number\":\"DFT5NG1T\"},\"interfaces\":[{\"if_class_id\":\""
                 "08\",\"if_subclass_id\":\"06\",\"if_protocol_id\":\"50\"}]}},"
                 "\"text\":\"USB устройство присоединено\",\"timestamp\":\"2017-08-06T07:23:04\"},"
                 "{\"id\":3,\"value\":{\"raw\":{\"device_descriptor\":{\"vendor_id\":\"8564\","
                 "\"product_id\":\"1000\",\"dev_class_id\""
                 ":\"00\",\"dev_subclass_id\":\"00\",\"dev_protocol_id\":\"00\",\"serial_number\":"
                 "\"DFT5NG1T\"},\"interfaces\":[{\"if_class_id\":\""
                 "08\",\"if_subclass_id\":\"06\",\"if_protocol_id\":\"50\"}]}},\"text\":"
                 "\"USB устройство разрешено\",\"timestamp\":\"2017-08-06T07:23:04\"},"
                 "{\"id\":3,\"value\":{\"raw\":{\"device_descriptor\":{\"vendor_id\":"
                 "\"8564\",\"product_id\":\"1000\",\"dev_class_id\""
                 ":\"00\",\"dev_subclass_id\":\"00\",\"dev_protocol_id\":\"00\",\"serial_number\":"
                 "\"DFT5NG2T\"},\"interfaces\":[{\"if_class_id\":\""
                 "08\",\"if_subclass_id\":\"06\",\"if_protocol_id\":\"50\"}]}},\"text\":"
                 "\"USB устройство присоединено\",\"timestamp\":\"2017-08-06T07:23:04\"},"
                 "{\"id\":3,\"value\":{\"raw\":{\"device_descriptor\":{\"vendor_id\":\"8564\","
                 "\"product_id\":\"1000\",\"dev_class_id\""
                 ":\"00\",\"dev_subclass_id\":\"00\",\"dev_protocol_id\":\"00\",\"serial_number\":"
                 "\"DFT5NG2T\"},\"interfaces\":[{\"if_class_id\":\""
                 "08\",\"if_subclass_id\":\"06\",\"if_protocol_id\":\"50\"}]}},\"text\":"
                 "\"USB устройство заблокировано\",\"timestamp\":\"2017-08-06T07:23:04\"},";

  Core c;
  QVERIFY(c.Parse(data, 0));

  QCOMPARE(c.mHWBad.size(), 1);
  QCOMPARE(c.mHWGood.size(), 1);
}

void Test_parserTest::testRemoveMultiGoodBad()
{
  QString data = "{\"id\":3,\"value\":{\"raw\":{\"device_descriptor\":{\"vendor_id\":"
                 "\"8564\",\"product_id\":\"1000\",\"dev_class_id\""
                 ":\"00\",\"dev_subclass_id\":\"00\",\"dev_protocol_id\":\"00\",\"serial_number\":"
                 "\"DFT5NG1T\"},\"interfaces\":[{\"if_class_id\":\""
                 "08\",\"if_subclass_id\":\"06\",\"if_protocol_id\":\"50\"}]}},\"text\":"
                 "\"USB устройство извлечено\",\"timestamp\":\"2017-08-06T07:23:04\"},"
                 "{\"id\":3,\"value\":{\"raw\":{\"device_descriptor\":{\"vendor_id\":"
                 "\"8564\",\"product_id\":\"1000\",\"dev_class_id\""
                 ":\"00\",\"dev_subclass_id\":\"00\",\"dev_protocol_id\":\"00\",\"serial_number\":"
                 "\"DFT5NG2T\"},\"interfaces\":[{\"if_class_id\":\""
                 "08\",\"if_subclass_id\":\"06\",\"if_protocol_id\":\"50\"}]}},\"text\":"
                 "\"USB устройство извлечено\",\"timestamp\":\"2017-08-06T07:23:04\"},"
                 "{\"id\":3,\"value\":{\"raw\":{\"device_descriptor\":{\"vendor_id\":\"8564\","
                 "\"product_id\":\"1000\",\"dev_class_id\""
                 ":\"00\",\"dev_subclass_id\":\"00\",\"dev_protocol_id\":\"00\",\"serial_number\":"
                 "\"DFT5NG3T\"},\"interfaces\":[{\"if_class_id\":\""
                 "08\",\"if_subclass_id\":\"06\",\"if_protocol_id\":\"50\"}]}},\"text\":"
                 "\"USB устройство извлечено\",\"timestamp\":\"2017-08-06T07:23:04\"},"
                 "{\"id\":3,\"value\":{\"raw\":{\"device_descriptor\":{\"vendor_id\":\"8564\","
                 "\"product_id\":\"1000\",\"dev_class_id\""
                 ":\"00\",\"dev_subclass_id\":\"00\",\"dev_protocol_id\":\"00\",\"serial_number\":"
                 "\"DFT5NG4T\"},\"interfaces\":[{\"if_class_id\":\""
                 "08\",\"if_subclass_id\":\"06\",\"if_protocol_id\":\"50\"}]}},\"text\":"
                 "\"USB устройство извлечено\",\"timestamp\":\"2017-08-06T07:23:04\"},";

  Core c;
  c.mHWBad << "DFT5NG3T";

  QVERIFY(c.Parse(data, 0));

  QCOMPARE(c.mHWBad.size(), 0);
  QCOMPARE(c.mHWGood.size(), 0);
  QCOMPARE(c.mHWInstalledRemoved.size(), 3);
}

void Test_parserTest::testInitialData()
{
  QString data = "{\"id\":1, \"text\" : \"Драйвер гипервизора запущен\", "
                 "\"timestamp\" : \"2017-08-06T07:22:33\"},"
                 "{\"id\":2,\"value\":{\"raw\":{\"device_descriptor\":{\"vendor_id\":"
                 "\"8564\",\"product_id\":\"1000\""
                 ",\"dev_class_id\":\"00\",\"dev_subclass_id\":\"00\",\"dev_protocol_id\":"
                 "\"00\",\"serial_number\":\"DFT5NG1T\"},\""
                 "interfaces\":[{\"if_class_id\":\"08\",\"if_subclass_id\":\"06\","
                 "\"if_protocol_id\":\"50\"}]}},\"text\":"
                 "\"USB устройство извлечено\",\"timestamp\":\"2017-08-06T07:22:51\"},"
                 "{\"id\":3,\"value\":{\"raw\":{\"device_descriptor\""
                 ":{\"vendor_id\":\"8564\",\"product_id\":\"1000\",\"dev_class_id\":"
                 "\"00\",\"dev_subclass_id\":\"00\",\"dev_protocol_id\":"
                 "\"00\",\"serial_number\":\"DFT5NG1T\"},\"interfaces\":[{\"if_class_id\":"
                 "\"08\",\"if_subclass_id\":\"06\",\"if_protocol_id\""
                 ":\"50\"}]}},\"text\":\"USB устройство присоединено\",\"timestamp\":"
                 "\"2017-08-06T07:23:04\"},{\"id\":4,\"value\":{\"raw\":"
                 "{\"device_descriptor\":{\"vendor_id\":\"8564\",\"product_id\":\"1000\","
                 "\"dev_class_id\":\"00\",\"dev_subclass_id\":\"00\","
                 "\"dev_protocol_id\":\"00\",\"serial_number\":\"DFT5NG1T\"},\"interfaces\":"
                 "[{\"if_class_id\":\"08\",\"if_subclass_id\":"
                 "\"06\",\"if_protocol_id\":\"50\"}]}},\"text\":\"USB устройство разрешено\","
                 "\"timestamp\":\"2017-08-06T07:23:04\"},"
                 "{\"id\":3,\"value\":{\"raw\":{\"device_descriptor\":{\"vendor_id\":\"0951\","
                 "\"product_id\":\"1665\",\"dev_class_id\":"
                 "\"00\",\"dev_subclass_id\":\"00\",\"dev_protocol_id\":\"00\",\"serial_number\":"
                 "\"1C6F65A2E9D5BE51A93A20A8\"},"
                 "\"interfaces\":[{\"if_class_id\":\"08\",\"if_subclass_id\":\"06\",\"if_protocol_id\":"
                 "\"50\"}]}},\"text\":"
                 "\"USB устройство присоединено\",\"timestamp\":\"2017-08-06T07:23:22\"},"
                 "{\"id\":5,\"value\":{\"raw\":{\"device_descriptor\":"
                 "{\"vendor_id\":\"0951\",\"product_id\":\"1665\",\"dev_class_id\":\"00\","
                 "\"dev_subclass_id\":\"00\",\"dev_protocol_id\":\"00\","
                 "\"serial_number\":\"1C6F65A2E9D5BE51A93A20A8\"},\"interfaces\":[{\"if_class_id\":"
                 "\"08\",\"if_subclass_id\":"
                 "\"06\",\"if_protocol_id\":\"50\"}]}},\"text\":\"USB устройство заблокировано\","
                 "\"timestamp\":\"2017-08-06T07:23:22\"},"
                 "{\"id\":2,\"value\":{\"raw\":{\"device_descriptor\":{\"vendor_id\":\"0951\","
                 "\"product_id\":\"1665\",\"dev_class_id\":\"00\","
                 "\"dev_subclass_id\":\"00\",\"dev_protocol_id\":\"00\",\"serial_number\":"
                 "\"1C6F65A2E9D5BE51A93A20A8\"},\"interfaces\":"
                 "[{\"if_class_id\":\"08\",\"if_subclass_id\":\"06\",\"if_protocol_id\":"
                 "\"50\"}]}},\"text\":\"USB устройство извлечено\","
                 "\"timestamp\":\"2017-08-06T07:23:32\"},{\"id\":2,\"value\":{\"raw\":"
                 "{\"device_descriptor\":{\"vendor_id\":\"8564\","
                 "\"product_id\":\"1000\",\"dev_class_id\":\"00\",\"dev_subclass_id\":"
                 "\"00\",\"dev_protocol_id\":\"00\",\"serial_number\":"
                 "\"DFT5NG1T\"},\"interfaces\":[{\"if_class_id\":\"08\",\"if_subclass_id\":"
                 "\"06\",\"if_protocol_id\":\"50\"}]}},\"text\":"
                 "\"USB устройство извлечено\",\"timestamp\":\"2017-08-06T07:23:33\"},";

  Core c;

  QVERIFY(c.Parse(data, 0));

  QCOMPARE(c.mHWBad.size(), 0);
  QCOMPARE(c.mHWGood.size(), 0);
  QCOMPARE(c.mHWInstalledRemoved.size(), 1);
}

QTEST_MAIN(Test_parserTest)

#include "tst_Test_parserTest.moc"
