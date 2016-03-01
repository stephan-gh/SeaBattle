#include "mainwindow.h"
#include <QApplication>
#include <QTranslator>
#include <QLibraryInfo>
#include <QDebug>
#include <QDir>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    auto locale = QLocale::system().name();
    //auto locale = QStringLiteral("de_DE");

    QTranslator qtTranslator;
    qtTranslator.load("qt_" + locale, QLibraryInfo::location(QLibraryInfo::TranslationsPath));
    app.installTranslator(&qtTranslator);

    QTranslator appTranslator;
    appTranslator.load("seabattle_" + locale, ":/translations");
    app.installTranslator(&appTranslator);

    MainWindow window;
    window.show();

    return app.exec();
}
