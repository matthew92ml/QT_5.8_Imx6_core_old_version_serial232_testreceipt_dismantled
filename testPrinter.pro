#-------------------------------------------------
#
# Project created by QtCreator 2017-07-28T12:23:57
#
#-------------------------------------------------

QT       += core

QT       -= gui

TARGET = testPrinter
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += main.cpp

include(qextserialport/src/qextserialport.pri)

