#-------------------------------------------------
#
# Project created by QtCreator 2016-04-06T00:24:58
#
#-------------------------------------------------

QT       += testlib

QT       -= gui

TARGET = bigintegerunittests
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

include(vendor/vendor.pri)
include(../../biginteger.pri)

SOURCES += bigintegerunittests.cpp \
    main.cpp
DEFINES += SRCDIR=\\\"$$PWD/\\\"

HEADERS += \
    bigintegerunittests.h

DISTFILES += \
    qml/tst_calculation.qml
