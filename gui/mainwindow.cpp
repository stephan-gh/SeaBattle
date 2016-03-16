#include <QMessageBox>

#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "gameconfigdialog.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->actionGames, &QAction::triggered, [this] () {
        GameConfigDialog{this, configs}.exec();
    });

    connect(ui->actionAboutQt, &QAction::triggered, &QApplication::aboutQt);
    connect(ui->actionQuit, &QAction::triggered, this, &QMainWindow::close);
}

MainWindow::~MainWindow()
{
    delete ui;
}
