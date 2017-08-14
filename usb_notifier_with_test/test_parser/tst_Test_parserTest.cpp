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
  // предполагаемый порядок вставки и удаления уст.
  void testInit1();
  void testInit2();
  void testInit3();
  void testInit4();
  void testInit5();
  // тоже самое но с ошибками
  // порядок работы норм.
  // порядок работы кривой
};

Test_parserTest::Test_parserTest()
{
}

void Test_parserTest::testInit1()
{
  QString data = R"({"id":1484192689468435, "text":"Драйвер гипервизора запущен",
                 "timestamp":"2017-08-06T07:22:33"},{"id":1484192689468439,"value""
                 :{"raw":{"device_descriptor":{"vendor_id":"1","product_id":"1",
                 "dev_class_id":"00","dev_subclass_id":"00","dev_protocol_id":"00",
                 "serial_number":"mass1"},"interfaces":[{"if_class_id":"08",
                 "if_subclass_id":"06","if_protocol_id":"50"}]}},"text":"USB устройство извлечено",
                 "timestamp":"2017-08-06T07:22:51"},{"id":1484192689468430,"value""
                 :{"raw":{"device_descriptor":{"vendor_id":"1","product_id":"1",
                 "dev_class_id":"00","dev_subclass_id":"00","dev_protocol_id":"00",
                 "serial_number":"mass2"},"interfaces":[{"if_class_id":"08",
                 "if_subclass_id":"06","if_protocol_id":"50"}]}},"text":"USB устройство извлечено",
                 "timestamp":"2017-08-06T07:22:52"})";

  Core c;
  c.Parse(data, 0);

  QCOMPARE(c.mHWAccepted.size(), 0);
  QCOMPARE(c.mHWRejected.size(), 0);
  QCOMPARE(c.mHWConnected.size(), 0);
  QCOMPARE(c.mHWMandatoryDisconnected.size(), 0);
}

void Test_parserTest::testInit2()
{
  QString data = R"({"id":1484192689468435,
                 "text":"Драйвер гипервизора запущен","timestamp":"2017-08-06T07:22:33"},{"id":1484192689468439,
                 "value":{"raw":{"device_descriptor":{"vendor_id":"2","product_id":"2""
                 ,"dev_class_id":"03","dev_subclass_id":"00","dev_protocol_id":"00",
                 "serial_number":"keyboard1"},"interfaces":[{"if_class_id":"00",
                 "if_subclass_id":"00","if_protocol_id":"00"}]}},"text":"USB устройство извлечено",
                 "timestamp":"2017-08-06T07:22:51"},{"id":1484192689468439,
                 "value":{"raw":{"device_descriptor":{"vendor_id":"8564",
                 "product_id":"1000","dev_class_id":"01","dev_subclass_id":"00","dev_protocol_id":"00",
                 "serial_number":"audio1"},"interfaces":[{"if_class_id":"08",
                 "if_subclass_id":"00","if_protocol_id":"00"}]}},"text":"USB устройство извлечено",
                 "timestamp":"2017-08-06T07:22:51"},{"id":1484192689468439,
                 "value":{"raw":{"device_descriptor":{"vendor_id":"2","product_id":"2""
                 ,"dev_class_id":"03","dev_subclass_id":"00","dev_protocol_id":"00",
                 "serial_number":"keyboard2"},"interfaces":[{"if_class_id":"00",
                 "if_subclass_id":"00","if_protocol_id":"00"}]}},"text":"USB устройство присоединено",
                 "timestamp":"2017-08-06T07:22:51"},{"id":1484192689468439,
                 "value":{"raw":{"device_descriptor":{"vendor_id":"2","product_id":"2""
                 ,"dev_class_id":"03","dev_subclass_id":"00","dev_protocol_id":"00",
                 "serial_number":"audio2"},"interfaces":[{"if_class_id":"00",
                 "if_subclass_id":"00","if_protocol_id":"00"}]}},"text":"USB устройство присоединено",
                 "timestamp":"2017-08-06T07:22:51"})";

  Core c;
  c.Parse(data, 0);

  QCOMPARE(c.mHWAccepted.size(), 0);
  QCOMPARE(c.mHWRejected.size(), 2);
  QCOMPARE(c.mHWConnected.size(), 0);
  QCOMPARE(c.mHWMandatoryDisconnected.size(), 2);
}

void Test_parserTest::testInit3()
{
  QString data = R"({"id":1,
                 "text":"Драйвер гипервизора запущен","timestamp":"2017-08-06T07:22:33"},{"id":2,
                 "value":{"raw":{"device_descriptor":{"vendor_id":"2","product_id":"2""
                 ,"dev_class_id":"03","dev_subclass_id":"00","dev_protocol_id":"00",
                 "serial_number":"keyboard1"},"interfaces":[{"if_class_id":"00",
                 "if_subclass_id":"00","if_protocol_id":"00"}]}},"text":"USB устройство извлечено",
                 "timestamp":"2017-08-06T07:22:51"},{"id":3,
                 "value":{"raw":{"device_descriptor":{"vendor_id":"3",
                 "product_id":"2","dev_class_id":"01","dev_subclass_id":"00","dev_protocol_id":"00",
                 "serial_number":"audio1"},"interfaces":[{"if_class_id":"00",
                 "if_subclass_id":"00","if_protocol_id":"00"}]}},"text":"USB устройство извлечено",
                 "timestamp":"2017-08-06T07:22:51"},{"id":4,
                 "value":{"raw":{"device_descriptor":{"vendor_id":"2","product_id":"2""
                 ,"dev_class_id":"03","dev_subclass_id":"00","dev_protocol_id":"00",
                 "serial_number":"keyboard2"},"interfaces":[{"if_class_id":"00",
                 "if_subclass_id":"00","if_protocol_id":"00"}]}},"text":"USB устройство присоединено",
                 "timestamp":"2017-08-06T07:22:51"},{"id":5,
                 "value":{"raw":{"device_descriptor":{"vendor_id":"3","product_id":"2""
                 ,"dev_class_id":"01","dev_subclass_id":"00","dev_protocol_id":"00",
                 "serial_number":"audio2"},"interfaces":[{"if_class_id":"00",
                 "if_subclass_id":"00","if_protocol_id":"00"}]}},"text":"USB устройство присоединено",
                 "timestamp":"2017-08-06T07:22:51"},{"id":6,
                 "value":{"raw":{"device_descriptor":{"vendor_id":"2","product_id":"2""
                 ,"dev_class_id":"03","dev_subclass_id":"00","dev_protocol_id":"00",
                 "serial_number":"keyboard2"},"interfaces":[{"if_class_id":"00",
                 "if_subclass_id":"00","if_protocol_id":"00"}]}},"text":"USB устройство извлечено",
                 "timestamp":"2017-08-06T07:22:51"},{"id":7,
                 "value":{"raw":{"device_descriptor":{"vendor_id":"3","product_id":"2""
                 ,"dev_class_id":"01","dev_subclass_id":"00","dev_protocol_id":"00",
                 "serial_number":"audio2"},"interfaces":[{"if_class_id":"00",
                 "if_subclass_id":"00","if_protocol_id":"00"}]}},"text":"USB устройство извлечено",
                 "timestamp":"2017-08-06T07:22:51"},{"id":8,
                 "value":{"raw":{"device_descriptor":{"vendor_id":"2","product_id":"2""
                 ,"dev_class_id":"03","dev_subclass_id":"00","dev_protocol_id":"00",
                 "serial_number":"keyboard1"},"interfaces":[{"if_class_id":"00",
                 "if_subclass_id":"00","if_protocol_id":"00"}]}},"text":"USB устройство присоединено",
                 "timestamp":"2017-08-06T07:22:51"},{"id":9,
                 "value":{"raw":{"device_descriptor":{"vendor_id":"3","product_id":"2""
                 ,"dev_class_id":"01","dev_subclass_id":"00","dev_protocol_id":"00",
                 "serial_number":"audio1"},"interfaces":[{"if_class_id":"00",
                 "if_subclass_id":"00","if_protocol_id":"00"}]}},"text":"USB устройство присоединено",
                 "timestamp":"2017-08-06T07:22:51"},{"id":10,
                 "value":{"raw":{"device_descriptor":{"vendor_id":"4","product_id":"2""
                 ,"dev_class_id":"00","dev_subclass_id":"00","dev_protocol_id":"00",
                 "serial_number":"mass1"},"interfaces":[{"if_class_id":"08",
                 "if_subclass_id":"08","if_protocol_id":"00"}]}},"text":"USB устройство присоединено",
                 "timestamp":"2017-08-06T07:22:51"},{"id":11,
                 "value":{"raw":{"device_descriptor":{"vendor_id":"4","product_id":"2""
                 ,"dev_class_id":"00","dev_subclass_id":"00","dev_protocol_id":"00",
                 "serial_number":"mass2"},"interfaces":[{"if_class_id":"08",
                 "if_subclass_id":"08","if_protocol_id":"00"}]}},"text":"USB устройство присоединено",
                 "timestamp":"2017-08-06T07:22:51"},{"id":12,
                 "value":{"raw":{"device_descriptor":{"vendor_id":"5","product_id":"2""
                 ,"dev_class_id":"00","dev_subclass_id":"00","dev_protocol_id":"00",
                 "serial_number":"mass3"},"interfaces":[{"if_class_id":"08",
                 "if_subclass_id":"08","if_protocol_id":"00"}]}},"text":"USB устройство присоединено",
                 "timestamp":"2017-08-06T07:22:51"},{"id":13,
                 "value":{"raw":{"device_descriptor":{"vendor_id":"4","product_id":"2""
                 ,"dev_class_id":"00","dev_subclass_id":"00","dev_protocol_id":"00",
                 "serial_number":"mass1"},"interfaces":[{"if_class_id":"08",
                 "if_subclass_id":"08","if_protocol_id":"00"}]}},"text":"USB устройство разрешено",
                 "timestamp":"2017-08-06T07:22:51"},{"id":14,
                 "value":{"raw":{"device_descriptor":{"vendor_id":"5","product_id":"2""
                 ,"dev_class_id":"00","dev_subclass_id":"00","dev_protocol_id":"00",
                 "serial_number":"mass3"},"interfaces":[{"if_class_id":"08",
                 "if_subclass_id":"08","if_protocol_id":"00"}]}},"text":"USB устройство заблокировано",
                 "timestamp":"2017-08-06T07:22:51"})";

  Core c;
  c.Parse(data, 0);

  QCOMPARE(c.mHWAccepted.size(), 1);
  QCOMPARE(c.mHWRejected.size(), 1);
  QCOMPARE(c.mHWConnected.size(), 1);
  QCOMPARE(c.mHWMandatoryDisconnected.size(), 0);
}

void Test_parserTest::testInit4()
{
  QString data = R"({"id":1,
                 "text":"Драйвер гипервизора запущен","timestamp":"2017-08-06T07:22:33"},{"id":2,
                 "value":{"raw":{"device_descriptor":{"vendor_id":"2","product_id":"2""
                 ,"dev_class_id":"03","dev_subclass_id":"00","dev_protocol_id":"00",
                 "serial_number":"keyboard1"},"interfaces":[{"if_class_id":"00",
                 "if_subclass_id":"00","if_protocol_id":"00"}]}},"text":"USB устройство извлечено",
                 "timestamp":"2017-08-06T07:22:51"},{"id":3,
                 "value":{"raw":{"device_descriptor":{"vendor_id":"3",
                 "product_id":"2","dev_class_id":"01","dev_subclass_id":"00","dev_protocol_id":"00",
                 "serial_number":"audio1"},"interfaces":[{"if_class_id":"00",
                 "if_subclass_id":"00","if_protocol_id":"00"}]}},"text":"USB устройство извлечено",
                 "timestamp":"2017-08-06T07:22:51"},{"id":4,
                 "value":{"raw":{"device_descriptor":{"vendor_id":"2","product_id":"2""
                 ,"dev_class_id":"03","dev_subclass_id":"00","dev_protocol_id":"00",
                 "serial_number":"keyboard2"},"interfaces":[{"if_class_id":"00",
                 "if_subclass_id":"00","if_protocol_id":"00"}]}},"text":"USB устройство присоединено",
                 "timestamp":"2017-08-06T07:22:51"},{"id":5,
                 "value":{"raw":{"device_descriptor":{"vendor_id":"3","product_id":"2""
                 ,"dev_class_id":"01","dev_subclass_id":"00","dev_protocol_id":"00",
                 "serial_number":"audio2"},"interfaces":[{"if_class_id":"00",
                 "if_subclass_id":"00","if_protocol_id":"00"}]}},"text":"USB устройство присоединено",
                 "timestamp":"2017-08-06T07:22:51"},{"id":6,
                 "value":{"raw":{"device_descriptor":{"vendor_id":"2","product_id":"2""
                 ,"dev_class_id":"03","dev_subclass_id":"00","dev_protocol_id":"00",
                 "serial_number":"keyboard2"},"interfaces":[{"if_class_id":"00",
                 "if_subclass_id":"00","if_protocol_id":"00"}]}},"text":"USB устройство извлечено",
                 "timestamp":"2017-08-06T07:22:51"},{"id":7,
                 "value":{"raw":{"device_descriptor":{"vendor_id":"3","product_id":"2""
                 ,"dev_class_id":"01","dev_subclass_id":"00","dev_protocol_id":"00",
                 "serial_number":"audio2"},"interfaces":[{"if_class_id":"00",
                 "if_subclass_id":"00","if_protocol_id":"00"}]}},"text":"USB устройство извлечено",
                 "timestamp":"2017-08-06T07:22:51"},{"id":8,
                 "value":{"raw":{"device_descriptor":{"vendor_id":"2","product_id":"2""
                 ,"dev_class_id":"03","dev_subclass_id":"00","dev_protocol_id":"00",
                 "serial_number":"keyboard1"},"interfaces":[{"if_class_id":"00",
                 "if_subclass_id":"00","if_protocol_id":"00"}]}},"text":"USB устройство присоединено",
                 "timestamp":"2017-08-06T07:22:51"},{"id":9,
                 "value":{"raw":{"device_descriptor":{"vendor_id":"3","product_id":"2""
                 ,"dev_class_id":"01","dev_subclass_id":"00","dev_protocol_id":"00",
                 "serial_number":"audio1"},"interfaces":[{"if_class_id":"00",
                 "if_subclass_id":"00","if_protocol_id":"00"}]}},"text":"USB устройство присоединено",
                 "timestamp":"2017-08-06T07:22:51"},{"id":10,
                 "value":{"raw":{"device_descriptor":{"vendor_id":"4","product_id":"2""
                 ,"dev_class_id":"00","dev_subclass_id":"00","dev_protocol_id":"00",
                 "serial_number":"mass1"},"interfaces":[{"if_class_id":"08",
                 "if_subclass_id":"08","if_protocol_id":"00"}]}},"text":"USB устройство присоединено",
                 "timestamp":"2017-08-06T07:22:51"},{"id":11,
                 "value":{"raw":{"device_descriptor":{"vendor_id":"4","product_id":"2""
                 ,"dev_class_id":"00","dev_subclass_id":"00","dev_protocol_id":"00",
                 "serial_number":"mass2"},"interfaces":[{"if_class_id":"08",
                 "if_subclass_id":"08","if_protocol_id":"00"}]}},"text":"USB устройство присоединено",
                 "timestamp":"2017-08-06T07:22:51"},{"id":12,
                 "value":{"raw":{"device_descriptor":{"vendor_id":"5","product_id":"2""
                 ,"dev_class_id":"00","dev_subclass_id":"00","dev_protocol_id":"00",
                 "serial_number":"mass3"},"interfaces":[{"if_class_id":"08",
                 "if_subclass_id":"08","if_protocol_id":"00"}]}},"text":"USB устройство присоединено",
                 "timestamp":"2017-08-06T07:22:51"},{"id":13,
                 "value":{"raw":{"device_descriptor":{"vendor_id":"4","product_id":"2""
                 ,"dev_class_id":"00","dev_subclass_id":"00","dev_protocol_id":"00",
                 "serial_number":"mass1"},"interfaces":[{"if_class_id":"08",
                 "if_subclass_id":"08","if_protocol_id":"00"}]}},"text":"USB устройство разрешено",
                 "timestamp":"2017-08-06T07:22:51"},{"id":14,
                 "value":{"raw":{"device_descriptor":{"vendor_id":"5","product_id":"2""
                 ,"dev_class_id":"00","dev_subclass_id":"00","dev_protocol_id":"00",
                 "serial_number":"mass3"},"interfaces":[{"if_class_id":"08",
                 "if_subclass_id":"08","if_protocol_id":"00"}]}},"text":"USB устройство заблокировано",
                 "timestamp":"2017-08-06T07:22:51"},{"id":2,
                 "value":{"raw":{"device_descriptor":{"vendor_id":"2","product_id":"2""
                 ,"dev_class_id":"03","dev_subclass_id":"00","dev_protocol_id":"00",
                 "serial_number":"keyboard1"},"interfaces":[{"if_class_id":"00",
                 "if_subclass_id":"00","if_protocol_id":"00"}]}},"text":"USB устройство заблокировано",
                 "timestamp":"2017-08-06T07:22:51"},{"id":3,
                 "value":{"raw":{"device_descriptor":{"vendor_id":"3",
                 "product_id":"2","dev_class_id":"01","dev_subclass_id":"00","dev_protocol_id":"00",
                 "serial_number":"audio1"},"interfaces":[{"if_class_id":"00",
                 "if_subclass_id":"00","if_protocol_id":"00"}]}},"text":"USB устройство извлечено",
                 "timestamp":"2017-08-06T07:22:51"},{"id":3,
                 "value":{"raw":{"device_descriptor":{"vendor_id":"3",
                 "product_id":"2","dev_class_id":"01","dev_subclass_id":"00","dev_protocol_id":"00",
                 "serial_number":"audio1"},"interfaces":[{"if_class_id":"00",
                 "if_subclass_id":"00","if_protocol_id":"00"}]}},"text":"USB устройство заблокировано",
                 "timestamp":"2017-08-06T07:22:51"},{"id":14,
                 "value":{"raw":{"device_descriptor":{"vendor_id":"5","product_id":"2""
                 ,"dev_class_id":"00","dev_subclass_id":"00","dev_protocol_id":"00",
                 "serial_number":"mass3"},"interfaces":[{"if_class_id":"08",
                 "if_subclass_id":"08","if_protocol_id":"00"}]}},"text":"USB устройство разрешено",
                 "timestamp":"2017-08-06T07:22:51"},{"id":13,
                 "value":{"raw":{"device_descriptor":{"vendor_id":"4","product_id":"2""
                 ,"dev_class_id":"00","dev_subclass_id":"00","dev_protocol_id":"00",
                 "serial_number":"mass1"},"interfaces":[{"if_class_id":"08",
                 "if_subclass_id":"08","if_protocol_id":"00"}]}},"text":"USB устройство заблокировано",
                 "timestamp":"2017-08-06T07:22:51"})";

  Core c;
  c.Parse(data, 0);

  QCOMPARE(c.mHWAccepted.size(), 1);
  QCOMPARE(c.mHWRejected.size(), 2);
  QCOMPARE(c.mHWConnected.size(), 1);
  QCOMPARE(c.mHWMandatoryDisconnected.size(), 0);
}

void Test_parserTest::testInit5()
{
  QString data = R"({"id":1484192689468435, "text":"Драйвер гипервизора запущен",
                 "timestamp":"2017-08-06T07:22:33"},{"id":1,
                 "value":{"raw":{"device_descriptor":{
                 "vendor_id":"1","product_id":"1","dev_class_id":"00","dev_subclass_id":"00",
                 "dev_protocol_id":"00","serial_number":"kb_good"},"interfaces":[{"if_class_id":"03",
                 "if_subclass_id":"01","if_protocol_id":"01"}]}},"text":"USB устройство извлечено",
                 "timestamp":"2017-08-06T07:22:51"},{"id":2,
                 "value":{"raw":{"device_descriptor":{"vendor_id":"2","product_id":"2"
                 ,"dev_class_id":"03","dev_subclass_id":"01","dev_protocol_id":"02",
                 "serial_number":"mouse_good"},"interfaces":[{"if_class_id":"00",
                 "if_subclass_id":"00","if_protocol_id":"00"}]}},"text":"USB устройство извлечено",
                 "timestamp":"2017-08-06T07:22:51"},{"id":3,
                 "value":{"raw":{"device_descriptor":{"vendor_id":"8564","product_id":"1000"
                 ,"dev_class_id":"03","dev_subclass_id":"01","dev_protocol_id":"01",
                 "serial_number":"kb_bad"},"interfaces":[{"if_class_id":"08",
                 "if_subclass_id":"00","if_protocol_id":"00"}]}},"text":"USB устройство присоединено",
                 "timestamp":"2017-08-06T07:22:51"},{"id":4,
                 "value":{"raw":{"device_descriptor":{"vendor_id":"8564","product_id":"1000"
                 ,"dev_class_id":"03","dev_subclass_id":"01","dev_protocol_id":"01",
                 "serial_number":"kb_bad"},"interfaces":[{"if_class_id":"08",
                 "if_subclass_id":"06","if_protocol_id":"50"}]}},"text":"USB устройство разрешено",
                 "timestamp":"2017-08-06T07:22:51"},{"id":5,
                 "value":{"raw":{"device_descriptor":{"vendor_id":"8564","product_id":"1000"
                 ,"dev_class_id":"03","dev_subclass_id":"01","dev_protocol_id":"02",
                 "serial_number":"mouse_bad"},"interfaces":[{"if_class_id":"08",
                 "if_subclass_id":"06","if_protocol_id":"50"}]}},"text":"USB устройство присоединено",
                 "timestamp":"2017-08-06T07:22:51"},{"id":6,
                 "value":{"raw":{"device_descriptor":{"vendor_id":"8564","product_id":"1000"
                 ,"dev_class_id":"03","dev_subclass_id":"01","dev_protocol_id":"02",
                 "serial_number":"mouse_bad"},"interfaces":[{"if_class_id":"08",
                 "if_subclass_id":"06","if_protocol_id":"50"}]}},"text":"USB устройство разрешено",
                 "timestamp":"2017-08-06T07:22:51"})";

  Core c;
  c.Parse(data, 0);

  QCOMPARE(c.mHWAccepted.size(), 0);
  QCOMPARE(c.mHWRejected.size(), 2);
  QCOMPARE(c.mHWConnected.size(), 0);
  QCOMPARE(c.mHWMandatoryDisconnected.size(), 2);
}


QTEST_MAIN(Test_parserTest)

#include "tst_Test_parserTest.moc"
