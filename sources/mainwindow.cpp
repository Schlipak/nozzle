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
    setWindowFlags(Qt::Popup | Qt::FramelessWindowHint);
    setAttribute(Qt::WA_TranslucentBackground, true);
    setFocusPolicy(Qt::StrongFocus);

    setupUi();
    backend = new Backend(NULL);
    connect(backend, SIGNAL(newResultsAvailable(QString)), this, SLOT(onNewBackendResults(QString)));
    connect(ui->entryList, SIGNAL(itemActivated(QListWidgetItem*)), this, SLOT(onEntrySelected(QListWidgetItem*)));
}

MainWindow::~MainWindow()
{
    delete ui;
    delete shadow;
    delete animIntro;
    delete animSize;
    delete animIntroTimer;
    delete backend;
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    if (closed) return;

    event->ignore();
    closed = true;

    QSettings settings;

    delete animIntro;
    animIntro = new QPropertyAnimation(this, "geometry");
    animIntro->setDuration(settings.value("style/animation-duration", 400).toInt());
    animIntro->setStartValue(QRect(getXOffset(), 100, settings.value("style/width", 800).toInt(), 0));
    animIntro->setEndValue(QRect(getXOffset(), -100, settings.value("style/width", 800).toInt(), 0));
    animIntro->setEasingCurve(QEasingCurve::OutBack);
    animIntro->start();
    animIntroTimer->start(settings.value("style/animation-duration", 400).toInt() + 100);
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
            "QWidget#wrapper {"
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
            "QLineEdit#searchInput {"
                "background: transparent;"
                "border: none;"
                "color: %1;"
            "}"
        ).arg(
            settings.value("style/text-color", "rgb(243, 243, 243)").toString()
        )
    );
    ui->entryList->setStyleSheet(
        QString(
            "QWidget#entryList {"
                "background: transparent;"
                "border-radius: %1;"
            "}"
        ).arg(
            settings.value("style/border-radius", "3px").toString()
        )
    );

    shadow = new QGraphicsDropShadowEffect();
    shadow->setBlurRadius(settings.value("style/blur-radius", 15).toInt());
    shadow->setOffset(0, 3);
    ui->centralWidget->setGraphicsEffect(shadow);
    ui->searchInput->setFocus();

    animIntro = new QPropertyAnimation(this, "geometry");
    animIntro->setDuration(settings.value("style/animation-duration", 400).toInt());
    animIntro->setStartValue(QRect(getXOffset(), -100, settings.value("style/width", 800).toInt(), 0));
    animIntro->setEndValue(QRect(getXOffset(), 100, settings.value("style/width", 800).toInt(), 0));
    animIntro->setEasingCurve(QEasingCurve::OutBack);
    animIntro->start();

    animSize = new QPropertyAnimation(this, "size");
    animSize->setDuration(200);
    animSize->setEasingCurve(QEasingCurve::InOutQuad);

    animIntroTimer = new QTimer(this);
    animIntroTimer->setSingleShot(true);
    connect(animIntroTimer, SIGNAL(timeout()), SLOT(animateOut()));

    ui->entryList->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
}

void MainWindow::onNewBackendResults(const QString &results)
{
    QJsonParseError         err;
    QJsonDocument           json = QJsonDocument::fromJson(results.simplified().toUtf8(), &err);
    QJsonObject             root = json.object();
    QJsonArray              entries = root["results"].toArray();

    ui->entryList->clear();
    foreach (const QJsonValue &value, entries)
    {
        QJsonObject entryData = value.toObject();
        Entry *entry = new Entry(
            this,
            entryData["name"].toString(),
            " " + entryData["description"].toString(),
            entryData["icon"].toString(),
            entryData["exec"].toString()
        );
        QListWidgetItem *item = new QListWidgetItem();

        ui->entryList->addItem(item);
        item->setSizeHint(QSize(0, 50));
        ui->entryList->setItemWidget(item, entry);
    }

    QSize s;
    s.setWidth(width());
    s.setHeight(70 + 50 * entries.size());

    animSize->setStartValue(size());
    animSize->setEndValue(s);
    animSize->start();
}

void MainWindow::onEntrySelected(QListWidgetItem *item)
{
    Entry       *entry = static_cast<Entry*>(ui->entryList->itemWidget(item));
    QProcess    proc;

    qDebug() << "Entry selected" << entry->getExec();
    proc.startDetached(entry->getExec());
    close();
}

void MainWindow::animateOut()
{
    close();
    app.exit();
}

void MainWindow::on_searchInput_textChanged(const QString &string)
{
    backend->updateSearchQuery(string);
}
