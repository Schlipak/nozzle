#include "mainwindow.hh"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent, const QApplication &app) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    app(app),
    mClosed(false),
    mCount(0)
{
    ui->setupUi(this);
    setWindowFlags(Qt::Popup | Qt::FramelessWindowHint);
    setAttribute(Qt::WA_TranslucentBackground, true);
    setFocusPolicy(Qt::StrongFocus);

    setupUi();
    mBackend = new Backend(NULL);
    connect(mBackend, SIGNAL(newResultsAvailable(QString)), this, SLOT(onNewBackendResults(QString)));
    connect(ui->entryList, SIGNAL(itemActivated(QListWidgetItem*)), this, SLOT(onEntrySelected(QListWidgetItem*)));
}

MainWindow::~MainWindow()
{
    delete ui;
    delete mShadow;
    delete mAnimIntro;
    delete mAnimSize;
    delete mAnimIntroTimer;
    delete mBackend;
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    if (mClosed) return;

    event->ignore();
    mClosed = true;

    QSettings settings;

    delete mAnimIntro;
    mAnimIntro = new QPropertyAnimation(this, "geometry");
    mAnimIntro->setDuration(settings.value("style/animation-duration", 400).toInt());
    mAnimIntro->setStartValue(QRect(getXOffset(), 100, settings.value("style/width", 800).toInt(), 0));
    mAnimIntro->setEndValue(QRect(getXOffset(), -100, settings.value("style/width", 800).toInt(), 0));
    mAnimIntro->setEasingCurve(QEasingCurve::OutBack);
    mAnimIntro->start();
    mAnimIntroTimer->start(settings.value("style/animation-duration", 400).toInt() + 100);
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
                "outline: 0;"
            "}"
            "QWidget#entryList::item {"
                "background: transparent;"
            "}"
            "QWidget#entryList::item:selected {"
                "background: %2;"
            "}"
            "QWidget#entryList::item[data~=\"last\"] {"
                "background: red;"
                "border-top-left-radius: 0px;"
                "border-top-right-radius: 0px;"
                "border-bottom-left-radius: %1;"
                "border-bottom-right-radius: %1;"
            "}"
        ).arg(
            settings.value("style/border-radius", "3px").toString(),
            settings.value("style/selection-background-color", "#B3E5FC").toString()
        )
    );

    mShadow = new QGraphicsDropShadowEffect();
    mShadow->setBlurRadius(settings.value("style/blur-radius", 15).toInt());
    mShadow->setOffset(0, 3);
    ui->centralWidget->setGraphicsEffect(mShadow);
    ui->searchInput->setFocus();

    mAnimIntro = new QPropertyAnimation(this, "geometry");
    mAnimIntro->setDuration(settings.value("style/animation-duration", 400).toInt());
    mAnimIntro->setStartValue(QRect(getXOffset(), -100, settings.value("style/width", 800).toInt(), 0));
    mAnimIntro->setEndValue(QRect(getXOffset(), 100, settings.value("style/width", 800).toInt(), 0));
    mAnimIntro->setEasingCurve(QEasingCurve::OutBack);
    mAnimIntro->start();

    mAnimSize = new QPropertyAnimation(this, "size");
    mAnimSize->setDuration(200);
    mAnimSize->setEasingCurve(QEasingCurve::InOutQuad);

    mAnimIntroTimer = new QTimer(this);
    mAnimIntroTimer->setSingleShot(true);
    connect(mAnimIntroTimer, SIGNAL(timeout()), SLOT(animateOut()));

    ui->entryList->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
}

void MainWindow::onNewBackendResults(const QString &results)
{
    QJsonParseError         err;
    QJsonDocument           json = QJsonDocument::fromJson(results.simplified().toUtf8(), &err);
    QJsonObject             root = json.object();
    QJsonArray              entries = root["results"].toArray();
    EntryListItem           *lastItem = Q_NULLPTR;
    QSize                   finalSize;

    ui->entryList->clear();
    if (err.error != QJsonParseError::NoError)
    {
        Entry *errorReport = new Entry(
            this,
            "Backend error",
            err.errorString(),
            "error",
            "notify-send -t 5000 \"Nozzle\" \"Backend error\" -i \"error"
        );
        EntryListItem *item = new EntryListItem();

        ui->entryList->addItem(item);
        item->setSizeHint(QSize(0, 50));
        ui->entryList->setItemWidget(item, errorReport);
        lastItem = item;

        finalSize.setWidth(width());
        finalSize.setHeight(70 + 50);
    } else {
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
            EntryListItem *item = new EntryListItem();

            ui->entryList->addItem(item);
            item->setSizeHint(QSize(0, 50));
            ui->entryList->setItemWidget(item, entry);
            lastItem = item;
        }

        finalSize.setWidth(width());
        finalSize.setHeight(70 + 50 * entries.size());
    }

    if (lastItem) lastItem->applyBorderRadius();

    mAnimSize->setStartValue(size());
    mAnimSize->setEndValue(finalSize);
    mAnimSize->start();
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
    mBackend->updateSearchQuery(string);
}
