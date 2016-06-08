#include "mainwindow.h"
#include <QApplication>
#include <QTranslator>
#include <QLibraryInfo>
#include <QDebug>
#include <QDir>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    //auto locale = QLocale::system();
    QLocale locale{QLocale::German};

    QTranslator qtTranslator;
    qtTranslator.load(locale, "qt", "_", QLibraryInfo::location(QLibraryInfo::TranslationsPath));
    app.installTranslator(&qtTranslator);

    QTranslator appTranslator;
    appTranslator.load(locale, "seabattle", "_", ":/translations");
    app.installTranslator(&appTranslator);

    MainWindow window{nullptr, "seabattle.json"};
    window.show();

    return app.exec();
}
