TEMPLATE = subdirs
CONFIG += ordered
SUBDIRS += lib \
    plugin  \
    tests/auto

OTHER_FILES = \
    rpm/libalignedtimer.yaml \
    rpm/libalignedtimer.spec \
    README.md
