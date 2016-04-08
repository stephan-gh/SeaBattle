QT -= gui
QT += websockets

TARGET = core
TEMPLATE = lib
CONFIG += staticlib c++14

SOURCES += \
    game.cpp \
    ship.cpp \
    player.cpp \
    field.cpp \
    server.cpp

HEADERS += \
    game.h \
    ship.h \
    player.h \
    field.h \
    server.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
