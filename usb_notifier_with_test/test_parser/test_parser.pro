#-------------------------------------------------
#
# Project created by QtCreator 2017-08-07T17:16:40
#
#-------------------------------------------------

QT       += testlib gui widgets

TARGET = tst_Test_parserTest
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

QMAKE_CXXFLAGS += -std=c++11


SOURCES += \
    tst_Test_parserTest.cpp \
    ../usb_notifier/Core.cpp \
    ../usb_notifier/WidgetSplash.cpp \
    ../usb_notifier/DialogInfo.cpp

HEADERS += \
    ../usb_notifier/Core.h \
    ../usb_notifier/WidgetSplash.h \
    ../usb_notifier/DialogInfo.h

FORMS    += \
    ../usb_notifier/DialogInfo.ui \
    ../usb_notifier/WidgetSplash.ui

DEFINES += SRCDIR=\\\"$$PWD/\\\"
