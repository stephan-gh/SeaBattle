QT -= gui

TARGET = core
TEMPLATE = lib
CONFIG += staticlib c++14

SOURCES += \
    game.cpp \
    ship.cpp \
    player.cpp \
    field.cpp

HEADERS += \
    game.h \
    ship.h \
    player.h \
    field.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
