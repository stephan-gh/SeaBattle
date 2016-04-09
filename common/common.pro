QT -= gui
QT += websockets

TARGET = common
TEMPLATE = lib
CONFIG += staticlib c++14

HEADERS += \
    game.h \
    gameconfig.h \
    ship.h \
    player.h \
    field.h \
    network/server.h \
    network/packet.h \
    network/client.h \
    network/network.h

SOURCES += \
    game.cpp \
    gameconfig.cpp \
    ship.cpp \
    player.cpp \
    field.cpp \
    network/server.cpp \
    network/packet.cpp \
    network/client.cpp \
    network/network.cpp

unix {
    target.path = /usr/lib
    INSTALLS += target
}
