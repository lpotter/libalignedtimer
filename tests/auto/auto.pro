TARGET = tst_qalignedtimer
CONFIG+=testcase

QT += testlib network
QT -= gui

SOURCES  += tst_qalignedtimer.cpp
INCLUDEPATH += ../../lib
LIBS += -L../../lib -lalignedtimer

target.path = $$INSTALL_ROOT$$PREFIX/lib/libalignedtimer/tests
INSTALLS += target

