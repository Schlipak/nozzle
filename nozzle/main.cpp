#include "mainwindow.hh"
#include <QApplication>
#include <QProcess>

int main(int argc, char *argv[])
{
    QApplication    app(argc, argv);
    MainWindow      win(NULL, app);

    win.show();
    return app.exec();
}
