QT += widgets websockets

TARGET = gui
TEMPLATE = app
CONFIG += c++14

HEADERS += \
    mainwindow.h \
    gameconfig/gameconfig.h \
    gameconfig/gameconfigdialog.h \
    gameconfig/gameconfigmodel.h \
    game/gamewidget.h

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    gameconfig/gameconfig.cpp \
    gameconfig/gameconfigdialog.cpp \
    gameconfig/gameconfigmodel.cpp \
    game/gamewidget.cpp

FORMS += \
    mainwindow.ui \
    gameconfig/gameconfigdialog.ui \
    gameconfig/gameconfigeditdialog.ui \
    game/gamewidget.ui \
    game/gamepreparewidget.ui

RESOURCES += seabattle.qrc
TRANSLATIONS = seabattle_de.ts

# Automatically build translations
isEmpty(QMAKE_LRELEASE) {
    win32:QMAKE_LRELEASE = $$[QT_INSTALL_BINS]\\lrelease.exe
    else:QMAKE_LRELEASE = $$[QT_INSTALL_BINS]/lrelease
}

TSQM.name = lrelease ${QMAKE_FILE_IN}
TSQM.input = TRANSLATIONS
TSQM.output = ${QMAKE_FILE_PATH}/${QMAKE_FILE_BASE}.qm
TSQM.commands = $$QMAKE_LRELEASE ${QMAKE_FILE_IN}
TSQM.CONFIG = no_link
QMAKE_EXTRA_COMPILERS += TSQM
PRE_TARGETDEPS += compiler_TSQM_make_all

# Core project dependency
win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../core/release/ -lcore
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../core/debug/ -lcore
else:unix: LIBS += -L$$OUT_PWD/../core/ -lcore

INCLUDEPATH += $$PWD/../core
DEPENDPATH += $$PWD/../core

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../core/release/libcore.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../core/debug/libcore.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../core/release/core.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../core/debug/core.lib
else:unix: PRE_TARGETDEPS += $$OUT_PWD/../core/libcore.a
