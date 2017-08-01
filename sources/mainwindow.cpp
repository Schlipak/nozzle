#include "mainwindow.hh"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent, const QApplication &app) :
  QMainWindow(parent),
  ui(new Ui::MainWindow),
  app(app),
  mUid(QUuid::createUuid()),
  mClosed(false),
  mCount(0)
{
  ui->setupUi(this);
  setWindowFlags(Qt::Popup | Qt::FramelessWindowHint);
  setAttribute(Qt::WA_TranslucentBackground, true);
  setFocusPolicy(Qt::StrongFocus);

  setupUi();

  QSettings settings;

  settings.beginGroup("backends");
  int backendCount = settings.beginReadArray("script");
  for (int i = 0; i < backendCount; i++)
  {
    settings.setArrayIndex(i);
    QString program = settings.value("exec", "python").toString();
    QString params = settings.value("params", "none.py").toString();
    QString name = settings.value("name", "(unknown)").toString();

    Backend *backend = new Backend(program, params);
    backend->setName(name);
    connect(backend, SIGNAL(newResultsAvailable(Backend const &, QString)), this, SLOT(onNewBackendResults(Backend const &, QString)));
    mbackends.append(backend);
  }
  settings.endArray();
  settings.endGroup();

  connect(ui->entryList, SIGNAL(itemActivated(QListWidgetItem*)), this, SLOT(onEntrySelected(QListWidgetItem*)));
  ui->searchInput->installEventFilter(this);
  connect(ui->searchInput, SIGNAL(returnPressed()), this, SLOT(onEnterPressedInInput()));
}

MainWindow::~MainWindow()
{
  delete ui;
  delete mShadow;
  delete mAnimIntro;
  delete mAnimSize;
  delete mAnimIntroTimer;
  foreach (auto backend, mbackends) {
    delete backend;
  }
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
      settings.value("style/selection-background-color", "#454545").toString()
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

void MainWindow::onNewBackendResults(Backend const &backend, const QString &results)
{
  QJsonParseError         err;
  QJsonDocument           json = QJsonDocument::fromJson(results.simplified().toUtf8(), &err);
  QJsonObject             root = json.object();
  QJsonArray              entries = root["results"].toArray();
  EntryListItem           *lastItem = Q_NULLPTR;
  QSize                   finalSize;

  clearEntries(backend.uid());
  if (err.error != QJsonParseError::NoError)
  {
    Entry *errorReport = new Entry(
      this,
      mUid,
      QString("Backend `%1'").arg(backend.name()),
      QString("JSON parse error: `%1'").arg(err.errorString()),
      "error",
      QString(
        "notify-send -t 5000 \"Nozzle - Backend `%1'\" \"JSON parse error: `%2'\" -i \"error"
      ).arg(backend.name(), err.errorString())
    );
    EntryListItem *item = new EntryListItem();

    ui->entryList->addItem(item);
    item->setSizeHint(QSize(0, 50));
    ui->entryList->setItemWidget(item, errorReport);
    lastItem = item;

    finalSize.setWidth(width());
    finalSize.setHeight(70 + 50);
  }
  else
  {
    foreach (const QJsonValue &value, entries)
    {
      QJsonObject entryData = value.toObject();
      Entry *entry = new Entry(
        this,
        backend.uid(),
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
    finalSize.setHeight(70 + 50 * ui->entryList->count());
  }

  if (lastItem) lastItem->applyBorderRadius();

  if (ui->entryList->count() > 0)
  {
    QListWidgetItem *item = ui->entryList->item(0);
    if (item) item->setSelected(true);
  }

  mAnimSize->setStartValue(size());
  mAnimSize->setEndValue(finalSize);
  mAnimSize->start();
}

bool MainWindow::eventFilter(QObject *target, QEvent *event)
{
  if (target == ui->searchInput)
  {
    if (event->type() == QEvent::KeyPress)
    {
      QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
      if (keyEvent->key() == Qt::Key_Down)
      {
        selectIndex(1);
      }
      else if (keyEvent->key() == Qt::Key_Up)
      {
        selectIndex(-1);
      }
    }
    else if (event->type() == QEvent::FocusOut)
    {
      QTimer::singleShot(0, ui->searchInput, SLOT(setFocus()));
      return false;
    }
  }
  return QMainWindow::eventFilter(target, event);
}

void MainWindow::selectIndex(int offset)
{
  QModelIndexList modelIndexList = ui->entryList->selectionModel()->selectedIndexes();
  int             index = 0;

  if (offset == -1)
  {
    index = ui->entryList->count() - 1;
  }
  if (!modelIndexList.empty())
  {
    index = modelIndexList[0].row() + offset;
  }

  if (ui->entryList->item(index))
  {
    ui->entryList->item(index)->setSelected(true);
  }
}

void MainWindow::clearEntries(const QUuid &uid)
{
  mMutex.lock();

  for (int i = 0; i < ui->entryList->count(); i++)
  {
    QListWidgetItem *item = ui->entryList->item(i);
    Entry *entry = static_cast<Entry *>(ui->entryList->itemWidget(item));

    if (entry->uid() == uid || entry->uid() == mUid)
    {
      ui->entryList->removeItemWidget(item);
      delete entry;
      ui->entryList->takeItem(ui->entryList->row(item));
      delete item;
      i--;
    }
  }

  mMutex.unlock();
}

void MainWindow::onEntrySelected(QListWidgetItem *item)
{
  Entry       *entry = static_cast<Entry*>(ui->entryList->itemWidget(item));
  QProcess    proc;

  qDebug() << "Entry selected" << entry->getExec();
  proc.startDetached(entry->getExec());
  close();
}

void MainWindow::onEnterPressedInInput()
{
  QList<QListWidgetItem *> list = ui->entryList->selectedItems();

  if (!list.empty())
  {
    QListWidgetItem *item = list[0];
    onEntrySelected(item);
  }
}

void MainWindow::animateOut()
{
  close();
  app.exit();
}

void MainWindow::on_searchInput_textChanged(const QString &string)
{
  foreach (Backend *backend, mbackends) {
    backend->updateSearchQuery(string);
  }
}
