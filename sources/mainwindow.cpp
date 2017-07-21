#include "mainwindow.hh"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent, const QApplication &app) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    app(app),
    closed(false),
    count(0)
{
    ui->setupUi(this);
    setWindowFlags(Qt::Popup);
    setAttribute(Qt::WA_TranslucentBackground, true);
    setFocusPolicy(Qt::StrongFocus);

    setupUi();
    backend = new BackendScript(NULL);
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

    QSettings settings;

    delete anim;
    anim = new QPropertyAnimation(this, "geometry");
    anim->setDuration(settings.value("style/animation-duration", 400).toInt());
    anim->setStartValue(QRect(getXOffset(), 100, settings.value("style/width", 800).toInt(), 0));
    anim->setEndValue(QRect(getXOffset(), -100, settings.value("style/width", 800).toInt(), 0));
    anim->setEasingCurve(QEasingCurve::OutBack);
    anim->start();
    animTimer->start(settings.value("style/animation-duration", 400).toInt() + 100);
}

double MainWindow::getXOffset()
{
    QRect const screen = app.desktop()->availableGeometry();
    return screen.width() / 2.f - size().width() / 2.f;
}

void MainWindow::setupUi()
{
    QSettings settings;

    resize(settings.value("style/width", 800).toInt(), height());
    setMaximumWidth(app.desktop()->availableGeometry().width() - 20);
    ui->wrapper->setStyleSheet(
        QString(
            "QWidget {"
                "border-radius: %1;"
                "background: %2;"
            "}"
        ).arg(
            settings.value("style/border-radius", "3px").toString(),
            settings.value("style/background-color", "#373737").toString()
        )
    );
    ui->searchInput->setStyleSheet(
        QString(
            "QLineEdit {"
                "color: %1;"
            "}"
        ).arg(
            settings.value("style/text-color", "rgb(243, 243, 243)").toString()
        )
    );

    shadow = new QGraphicsDropShadowEffect();
    shadow->setBlurRadius(settings.value("style/blur-radius", 15).toInt());
    shadow->setOffset(0, 3);
    ui->centralWidget->setGraphicsEffect(shadow);
    ui->searchInput->setFocus();

    anim = new QPropertyAnimation(this, "geometry");
    anim->setDuration(settings.value("style/animation-duration", 400).toInt());
    anim->setStartValue(QRect(getXOffset(), -100, settings.value("style/width", 800).toInt(), 0));
    anim->setEndValue(QRect(getXOffset(), 100, settings.value("style/width", 800).toInt(), 0));
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

void MainWindow::on_searchInput_returnPressed()
{
    Entry *entry = new Entry(this, QString("Entry %1").arg(count));
    ui->entryList->layout()->addWidget(entry);
    count++;

    QSize s;
    s.setWidth(width());
    s.setHeight(70 + 50 * count);
    resize(s);
}
