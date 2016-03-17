#include <QFile>
#include <QMessageBox>
#include <QInputDialog>
#include <QJsonArray>
#include <QJsonDocument>
#include <QDebug>

#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "gameconfigdialog.h"

MainWindow::MainWindow(QWidget *parent, const QString &configPath) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    configPath(configPath)
{
    ui->setupUi(this);
    loadConfig();

    connect(ui->actionNewGame, &QAction::triggered, [this] () {
        QStringList options;
        for (GameConfig config : this->configs) {
            options << config.name();
        }

        options << tr("Create new...");

        bool ok;
        auto selection = QInputDialog::getItem(this, tr("Select game configuration"), tr("Game configuration:"), options, 0, false, &ok);

        if (ok) {
            if (selection != options.last()) {
                qDebug() << selection;
                auto i = options.indexOf(selection);
                qDebug() << i;
            } else {
                // TODO: Create new
            }
        }
    });

    connect(ui->actionGames, &QAction::triggered, [this] () {
        if (GameConfigDialog{this, configs}.exec()) {
            saveConfig();
        }
    });

    connect(ui->actionAboutQt, &QAction::triggered, &QApplication::aboutQt);
    connect(ui->actionQuit, &QAction::triggered, this, &QMainWindow::close);
}

MainWindow::~MainWindow()
{
    delete ui;
}

bool MainWindow::loadConfig()
{
    QFile file{configPath};
    if (!file.exists()) {
        return false;
    }

    if (!file.open(QIODevice::ReadOnly)) {
        QMessageBox::critical(this, tr("Failed to open configuration file"), tr("Failed to open configuration file from %1").arg(configPath));
        return false;
    }

    auto doc = QJsonDocument::fromJson(file.readAll());
    auto json = doc.object();

    QJsonArray configs = json["configs"].toArray();
    this->configs.clear();
    for (auto config : configs) {
        this->configs.emplace_back(config);
    }
    return true;
}

bool MainWindow::saveConfig()
{
    QFile file{configPath};
    if (!file.open(QIODevice::WriteOnly)) {
        QMessageBox::critical(this, tr("Failed to write configuration file"), tr("Failed to write configuration to %1").arg(configPath));
        return false;
    }

    QJsonObject json;

    QJsonArray configs;
    for (auto config : this->configs) {
        configs.append(config);
    }
    json["configs"] = configs;

    QJsonDocument doc{json};
    file.write(doc.toJson());
    return true;
}
