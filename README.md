libalignedtimer
===============

Alignedtimer is a Qt 5 port of the iphb based QAlignedTimer class in Qt Mobility.
It has a slightly different API, so it can be a drop-in replacement for QTimer.


It includes a declarative plugin.

It currently uses libiphb (which depends on dsme) for it's heartbeat backend.

In the future, it may encompass it's own heartbeat/wakeup backend.

