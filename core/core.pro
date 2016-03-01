QT -= gui

TARGET = core
TEMPLATE = lib
CONFIG += staticlib c++14

SOURCES += \
    seabattle.cpp

HEADERS += \
    seabattle.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
