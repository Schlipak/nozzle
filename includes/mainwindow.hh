#ifndef MAINWINDOW_HH
#define MAINWINDOW_HH

#include <QMainWindow>
#include <QDesktopWidget>
#include <QGraphicsDropShadowEffect>
#include <QPropertyAnimation>
#include <QEasingCurve>
#include <QRect>
#include <QTimer>
#include <QCloseEvent>
#include <QProcess>
#include <QStringList>
#include <QSettings>

#include <iostream>

#include "backendscript.hh"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent, QApplication const &app, QSettings const &settings);
    ~MainWindow();

protected:
    void closeEvent(QCloseEvent *event);

private:
    Ui::MainWindow      *ui;
    QApplication const  &app;
    QSettings const     &settings;

    QGraphicsDropShadowEffect   *shadow;
    QPropertyAnimation          *anim;
    QTimer                      *animTimer;
    BackendScript               *backend;

    bool                        closed;

    double                      getXOffset();
    void                        setupUi();

private slots:
    void                        animateOut();
    void on_searchInput_textChanged(const QString &string);
};

#endif // MAINWINDOW_HH
