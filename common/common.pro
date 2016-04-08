QT -= gui
QT += websockets

TARGET = common
TEMPLATE = lib
CONFIG += staticlib c++14

HEADERS += \
    game.h \
    ship.h \
    player.h \
    field.h \
    server.h

SOURCES += \
    game.cpp \
    ship.cpp \
    player.cpp \
    field.cpp \
    server.cpp

unix {
    target.path = /usr/lib
    INSTALLS += target
}
