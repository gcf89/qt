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
  void testParse_1();
};

Test_parserTest::Test_parserTest()
{
}

void Test_parserTest::testParse_1()
{
  QString data = R"({"id":1484192689468435,
                 "text":"Драйвер гипервизора запущен","timestamp":"2017-08-06T07:22:33"},{"id":1484192689468439,
                 "value":{"raw":{"device_descriptor":{"vendor_id":"2","product_id":"2""
                 ,"dev_class_id":"03","dev_subclass_id":"00","dev_protocol_id":"00",
                 "serial_number":"m_kb"},"interfaces":[{"if_class_id":"03",
                 "if_subclass_id":"01","if_protocol_id":"01"}]}},"text":"USB устройство присоединено",
                 "timestamp":"2017-08-06T07:22:51"},{"id":1484192689468439,
                 "value":{"raw":{"device_descriptor":{"vendor_id":"2",
                 "product_id":"2","dev_class_id":"03","dev_subclass_id":"00","dev_protocol_id":"00",
                 "serial_number":"m_mo"},"interfaces":[{"if_class_id":"03",
                 "if_subclass_id":"01","if_protocol_id":"02"}]}},"text":"USB устройство присоединено",
                 "timestamp":"2017-08-06T07:22:51"},{"id":1484192689468439,
                 "value":{"raw":{"device_descriptor":{"vendor_id":"2","product_id":"2""
                 ,"dev_class_id":"03","dev_subclass_id":"00","dev_protocol_id":"00",
                 "serial_number":"m_kb"},"interfaces":[{"if_class_id":"03",
                 "if_subclass_id":"01","if_protocol_id":"01"}]}},"text":"USB устройство разрешено",
                 "timestamp":"2017-08-06T07:22:51"},{"id":1484192689468439,
                 "value":{"raw":{"device_descriptor":{"vendor_id":"2","product_id":"2""
                 ,"dev_class_id":"03","dev_subclass_id":"00","dev_protocol_id":"00",
                 "serial_number":"m_mo"},"interfaces":[{"if_class_id":"03",
                 "if_subclass_id":"01","if_protocol_id":"02"}]}},"text":"USB устройство разрешено",
                 "timestamp":"2017-08-06T07:22:51"},{"id":1484192689468439,
                 "value":{"raw":{"device_descriptor":{"vendor_id":"2","product_id":"2""
                 ,"dev_class_id":"03","dev_subclass_id":"00","dev_protocol_id":"00",
                 "serial_number":"m_kb1"},"interfaces":[{"if_class_id":"03",
                 "if_subclass_id":"01","if_protocol_id":"01"}]}},"text":"USB устройство присоединено",
                 "timestamp":"2017-08-06T07:22:51"},{"id":1484192689468439,
                 "value":{"raw":{"device_descriptor":{"vendor_id":"2","product_id":"2""
                 ,"dev_class_id":"03","dev_subclass_id":"00","dev_protocol_id":"00",
                 "serial_number":"m_kb1"},"interfaces":[{"if_class_id":"03",
                 "if_subclass_id":"01","if_protocol_id":"01"}]}},"text":"USB устройство разрешено",
                 "timestamp":"2017-08-06T07:22:51"},{"id":1484192689468439,
                 "value":{"raw":{"device_descriptor":{"vendor_id":"2",
                 "product_id":"2","dev_class_id":"03","dev_subclass_id":"00","dev_protocol_id":"00",
                 "serial_number":"hub1"},"interfaces":[{"if_class_id":"09",
                 "if_subclass_id":"00","if_protocol_id":"02"}]}},"text":"USB устройство присоединено",
                 "timestamp":"2017-08-06T07:22:51"},{"id":1484192689468439,
                 "value":{"raw":{"device_descriptor":{"vendor_id":"2","product_id":"2""
                 ,"dev_class_id":"03","dev_subclass_id":"00","dev_protocol_id":"00",
                 "serial_number":"hub1"},"interfaces":[{"if_class_id":"09",
                 "if_subclass_id":"00","if_protocol_id":"01"}]}},"text":"USB устройство разрешено",
                 "timestamp":"2017-08-06T07:22:51"},{"id":1484192689468439,
                 "value":{"raw":{"device_descriptor":{"vendor_id":"2",
                 "product_id":"2","dev_class_id":"03","dev_subclass_id":"00","dev_protocol_id":"00",
                 "serial_number":"flash1"},"interfaces":[{"if_class_id":"08",
                 "if_subclass_id":"06","if_protocol_id":"00"}]}},"text":"USB устройство присоединено",
                 "timestamp":"2017-08-06T07:22:51"},{"id":1484192689468439,
                 "value":{"raw":{"device_descriptor":{"vendor_id":"2","product_id":"2""
                 ,"dev_class_id":"03","dev_subclass_id":"00","dev_protocol_id":"00",
                 "serial_number":"flash1"},"interfaces":[{"if_class_id":"08",
                 "if_subclass_id":"06","if_protocol_id":"00"}]}},"text":"USB устройство разрешено",
                 "timestamp":"2017-08-06T07:22:51"},{"id":1484192689468439,
                 "value":{"raw":{"device_descriptor":{"vendor_id":"2",
                 "product_id":"2","dev_class_id":"03","dev_subclass_id":"00","dev_protocol_id":"00",
                 "serial_number":"flash2"},"interfaces":[{"if_class_id":"08",
                 "if_subclass_id":"06","if_protocol_id":"00"}]}},"text":"USB устройство присоединено",
                 "timestamp":"2017-08-06T07:22:51"},{"id":1484192689468439,
                 "value":{"raw":{"device_descriptor":{"vendor_id":"2","product_id":"2""
                 ,"dev_class_id":"03","dev_subclass_id":"00","dev_protocol_id":"00",
                 "serial_number":"flash2"},"interfaces":[{"if_class_id":"08",
                 "if_subclass_id":"06","if_protocol_id":"00"}]}},"text":"USB устройство заблокировано",
                 "timestamp":"2017-08-06T07:22:51"},{"id":1484192689468439,
                 "value":{"raw":{"device_descriptor":{"vendor_id":"2",
                 "product_id":"2","dev_class_id":"03","dev_subclass_id":"00","dev_protocol_id":"00",
                 "serial_number":"flash1"},"interfaces":[{"if_class_id":"08",
                 "if_subclass_id":"06","if_protocol_id":"00"}]}},"text":"USB устройство извлечено",
                 "timestamp":"2017-08-06T07:22:51"},{"id":1484192689468439,
                 "value":{"raw":{"device_descriptor":{"vendor_id":"2",
                 "product_id":"2","dev_class_id":"03","dev_subclass_id":"00","dev_protocol_id":"00",
                 "serial_number":"flash2"},"interfaces":[{"if_class_id":"08",
                 "if_subclass_id":"06","if_protocol_id":"00"}]}},"text":"USB устройство извлечено",
                 "timestamp":"2017-08-06T07:22:51"},{"id":1484192689468439,
                 "value":{"raw":{"device_descriptor":{"vendor_id":"2",
                 "product_id":"2","dev_class_id":"03","dev_subclass_id":"00","dev_protocol_id":"00",
                 "serial_number":"hub1"},"interfaces":[{"if_class_id":"09",
                 "if_subclass_id":"00","if_protocol_id":"02"}]}},"text":"USB устройство извлечено",
                 "timestamp":"2017-08-06T07:22:51"},{"id":1484192689468439,
                 "value":{"raw":{"device_descriptor":{"vendor_id":"2","product_id":"2""
                 ,"dev_class_id":"03","dev_subclass_id":"00","dev_protocol_id":"00",
                 "serial_number":"m_kb1"},"interfaces":[{"if_class_id":"03",
                 "if_subclass_id":"01","if_protocol_id":"01"}]}},"text":"USB устройство извлечено",
                 "timestamp":"2017-08-06T07:22:51"},{"id":1484192689468439,
                 "value":{"raw":{"device_descriptor":{"vendor_id":"2","product_id":"2""
                 ,"dev_class_id":"03","dev_subclass_id":"00","dev_protocol_id":"00",
                 "serial_number":"m_kb"},"interfaces":[{"if_class_id":"03",
                 "if_subclass_id":"01","if_protocol_id":"01"}]}},"text":"USB устройство извлечено",
                 "timestamp":"2017-08-06T07:22:51"},{"id":1484192689468439,
                 "value":{"raw":{"device_descriptor":{"vendor_id":"2","product_id":"2""
                 ,"dev_class_id":"03","dev_subclass_id":"00","dev_protocol_id":"00",
                 "serial_number":"m_kb"},"interfaces":[{"if_class_id":"03",
                 "if_subclass_id":"01","if_protocol_id":"01"}]}},"text":"USB устройство присоединено",
                 "timestamp":"2017-08-06T07:22:51"})";

  Core c;
  c.Parse(data, 0);

  QCOMPARE(c.mHWAccepted.size(), 0);
  QCOMPARE(c.mHWRejected.size(), 0);
  QCOMPARE(c.mHWConnected.size(), 0);
  QCOMPARE(c.mHWMandatoryDisconnected.size(), 0);
  QCOMPARE(c.mHWMandatoryConnected.size(), 2);
}

QTEST_MAIN(Test_parserTest)

#include "tst_Test_parserTest.moc"
