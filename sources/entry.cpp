#include "entry.hh"
#include "ui_entry.h"

Entry::Entry(
  QWidget       *parent,
  const QUuid   &uid,
  const QString &name,
  const QString &description,
  const QString &iconPath,
  const QString &exec
) :
  QWidget(parent),
  ui(new Ui::Entry),
  mUid(uid),
  mName(name),
  mDescription(description),
  mIconPath(iconPath),
  mExec(exec)
{
  QSettings settings;

  ui->setupUi(this);
  ui->name->setText(name);
  ui->name->setStyleSheet(
    QString(
      "QLabel {"
        "color: %1;"
      "}"
    ).arg(settings.value("style/text-color", "rgba(243, 243, 243, 0.75)").toString())
  );

  ui->description->setText(description);
  ui->description->setStyleSheet(
    QString(
      "QLabel {"
        "color: %1;"
      "}"
    ).arg(settings.value("style/text-secondary-color", "rgba(243, 243, 243, 0.5)").toString())
  );

  QIcon icon = QIcon::fromTheme(iconPath);
  QPixmap pixmap = icon.pixmap(icon.actualSize(QSize(64, 64)));
  ui->icon->setPixmap(pixmap);
}

Entry::~Entry()
{
  delete ui;
}

void Entry::setDescription(const QString &description)
{
  mDescription = description;
  ui->description->setText(description);
}

void Entry::setIconPath(const QString &path)
{
  mIconPath = path;

  QIcon icon = QIcon::fromTheme(path);
  QPixmap pixmap = icon.pixmap(icon.actualSize(QSize(64, 64)));
  ui->icon->setPixmap(pixmap);
}

void Entry::setExec(const QString &exec)
{
  mExec = exec;
}

const QString &Entry::getExec() const
{
  return mExec;
}

const QString &Entry::getName() const
{
  return mName;
}

void Entry::applyBorderRadius()
{
  setAccessibleName("last-item");
}

const QUuid &Entry::uid() const
{
  return mUid;
}
