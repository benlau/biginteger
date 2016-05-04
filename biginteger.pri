INCLUDEPATH += $$PWD

HEADERS += \
    $$PWD/BigInteger.hh \
    $$PWD/BigIntegerAlgorithms.hh \
    $$PWD/BigIntegerLibrary.hh \
    $$PWD/BigIntegerUtils.hh \
    $$PWD/BigUnsigned.hh \
    $$PWD/BigUnsignedInABase.hh \
    $$PWD/NumberlikeArray.hh \
    $$PWD/bigintegerobject.h \
    $$PWD/bigintegermath.h \
    $$PWD/bigintegerobject2.h

SOURCES += \
    $$PWD/BigInteger.cc \
    $$PWD/BigIntegerAlgorithms.cc \
    $$PWD/BigIntegerUtils.cc \
    $$PWD/BigUnsigned.cc \
    $$PWD/BigUnsignedInABase.cc \
    $$PWD/bigintegerobject.cpp \
    $$PWD/bigintegermath.cpp \
    $$PWD/bigintegerobject2.cpp \
    $$PWD/bigintegerqmltypes.cpp

RESOURCES += \
    $$PWD/biginteger.qrc
