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

INCLUDEPATH += ../..

SOURCES += bigintegerunittests.cpp \
    ../../BigInteger.cc \
    ../../BigIntegerAlgorithms.cc \
    ../../BigIntegerUtils.cc \
    ../../BigUnsigned.cc \
    ../../BigUnsignedInABase.cc
DEFINES += SRCDIR=\\\"$$PWD/\\\"

HEADERS += \
    ../../BigInteger.hh \
    ../../BigIntegerAlgorithms.hh \
    ../../BigIntegerLibrary.hh \
    ../../BigIntegerUtils.hh \
    ../../BigUnsigned.hh \
    ../../BigUnsignedInABase.hh \
    ../../NumberlikeArray.hh
