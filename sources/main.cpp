#include "mainwindow.hh"
#include <QApplication>
#include <QCoreApplication>
#include <QProcess>
#include <QSettings>

int main(int argc, char *argv[])
{
    QCoreApplication::setOrganizationName("Nozzle");
    QCoreApplication::setOrganizationDomain("nozzle.schlipak.github.io");
    QCoreApplication::setApplicationName("NozzleLauncher");

    QApplication    app(argc, argv);
    QSettings       settings;
    MainWindow      win(NULL, app);

    win.show();
    return app.exec();
}
