#include "mainwindow.hh"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent, const QApplication &app, const QSettings &settings) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    app(app),
    settings(settings),
    closed(false)
{
    ui->setupUi(this);
    setWindowFlags(Qt::Popup);
    setAttribute(Qt::WA_TranslucentBackground, true);
    setFocusPolicy(Qt::StrongFocus);

    setupUi();
    backend = new BackendScript(NULL, settings);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete shadow;
    delete anim;
    delete backend;
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    if (closed) return;

    event->ignore();
    closed = true;

    delete anim;
    anim = new QPropertyAnimation(this, "geometry");
    anim->setDuration(settings.value("panel/animation-duration", 400).toInt());
    anim->setStartValue(QRect(getXOffset(), 100, 0, 0));
    anim->setEndValue(QRect(getXOffset(), -100, 0, 0));
    anim->setEasingCurve(QEasingCurve::OutBack);
    anim->start();
    animTimer->start(settings.value("panel/animation-duration", 400).toInt() + 100);
}

double MainWindow::getXOffset()
{
    QRect const screen = app.desktop()->availableGeometry();
    return screen.width() / 2.f - size().width() / 2.f;
}

void MainWindow::setupUi()
{
    shadow = new QGraphicsDropShadowEffect();
    shadow->setBlurRadius(15);
    shadow->setOffset(0, 3);
    ui->centralWidget->setGraphicsEffect(shadow);
    ui->searchInput->setFocus();

    anim = new QPropertyAnimation(this, "geometry");
    anim->setDuration(settings.value("panel/animation-duration", 400).toInt());
    anim->setStartValue(QRect(getXOffset(), -100, 0, 0));
    anim->setEndValue(QRect(getXOffset(), 100, 0, 0));
    anim->setEasingCurve(QEasingCurve::OutBack);
    anim->start();

    animTimer = new QTimer(this);
    animTimer->setSingleShot(true);
    connect(animTimer, SIGNAL(timeout()), SLOT(animateOut()));
}

void MainWindow::animateOut()
{
    close();
    app.exit();
}

void MainWindow::on_searchInput_textChanged(const QString &string)
{
    qDebug() << "TEXT CHANGED: " << string;
    backend->updateSearchQuery(string);
}
