TARGET = alignedtimer
TEMPLATE = lib

DEFINES += LIBALIGNEDTIMER_LIBRARY

SOURCES += alignedtimer.cpp \
    qalignedtimer_linux.cpp

PUBLIC_HEADERS += alignedtimer.h\
        libalignedtimer_global.h

PRIVATE_HEADERS +=  \
        qalignedtimer_linux_p.h

target.path = /usr/lib
INSTALLS += target

PKGCONFIGFILES.files = libalignedtimer.pc
PKGCONFIGFILES.path = /usr/lib/pkgconfig
CONFIG += link_pkgconfig

PKGCONFIG += libiphb

OTHER_FILES += libalignedtimer.pc

headers.path = $$INSTALL_ROOT$$PREFIX/include/alignedtimer

headers.files = $$PUBLIC_HEADERS
HEADERS += $$PUBLIC_HEADERS $$PRIVATE_HEADERS

INSTALLS += headers PKGCONFIGFILES
