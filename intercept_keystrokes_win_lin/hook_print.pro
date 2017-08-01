#-------------------------------------------------
#
# Project created by QtCreator 2011-12-21T17:32:37
#
#-------------------------------------------------

QT       += core

QT       += gui

TARGET = hook_print
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

LIBS += -lX11

SOURCES += main.cpp \
    hookkeybord.cpp

HEADERS += \
    hookkeybord.h
