#include "mainwindow.hh"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent, const QApplication &app) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    app(app),
    closed(false)
{
    ui->setupUi(this);
    setWindowFlags(Qt::Popup);
    setAttribute(Qt::WA_TranslucentBackground, true);
    setFocusPolicy(Qt::StrongFocus);

    shadow = new QGraphicsDropShadowEffect();
    shadow->setBlurRadius(15);
    shadow->setOffset(0, 3);
    ui->centralWidget->setGraphicsEffect(shadow);
    ui->searchInput->setFocus();

    anim = new QPropertyAnimation(this, "geometry");
    anim->setDuration(400);
    anim->setStartValue(QRect(getXOffset(), -100, 0, 0));
    anim->setEndValue(QRect(getXOffset(), 100, 0, 0));
    anim->setEasingCurve(QEasingCurve::OutBack);
    anim->start();

    animTimer = new QTimer(this);
    animTimer->setSingleShot(true);
    connect(animTimer, SIGNAL(timeout()), SLOT(animateOut()));
}

MainWindow::~MainWindow()
{
    delete ui;
    delete shadow;
    delete anim;
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    if (closed) return;

    event->ignore();
    closed = true;

    delete anim;
    anim = new QPropertyAnimation(this, "geometry");
    anim->setDuration(400);
    anim->setStartValue(QRect(getXOffset(), 100, 0, 0));
    anim->setEndValue(QRect(getXOffset(), -100, 0, 0));
    anim->setEasingCurve(QEasingCurve::OutBack);
    anim->start();
    animTimer->start(500);
}

double MainWindow::getXOffset()
{
    QRect const screen = app.desktop()->availableGeometry();
    return screen.width() / 2.f - size().width() / 2.f;
}

void MainWindow::animateOut()
{
    close();
    app.exit();
}
