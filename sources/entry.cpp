#include "entry.hh"
#include "ui_entry.h"

Entry::Entry(QWidget *parent, const QString &name, const QString &description, const QString &iconPath, const QString &exec) :
    QWidget(parent),
    ui(new Ui::Entry),
    name(name),
    description(description),
    iconPath(iconPath),
    exec(exec)
{
    QSettings settings;

    ui->setupUi(this);
    ui->name->setText(name);
    ui->name->setStyleSheet(
        QString(
            "QLabel {"
                "color: %1;"
            "}"
        ).arg(settings.value("style/text-color", "rgb(243, 243, 243)").toString())
    );

    ui->description->setText(this->description);
    ui->description->setStyleSheet(
        QString(
            "QLabel {"
                "color: %1;"
            "}"
        ).arg(settings.value("style/text-color", "rgb(243, 243, 243)").toString())
    );

    QIcon icon = QIcon::fromTheme(iconPath);
    QPixmap pixmap = icon.pixmap(icon.actualSize(QSize(64, 64)));
    ui->icon->setPixmap(pixmap);

    generalOpacity = new QGraphicsOpacityEffect(ui->wrapper);
    generalOpacity->setOpacity(0.75);
    ui->wrapper->setGraphicsEffect(generalOpacity);

    descriptionOpacity = new QGraphicsOpacityEffect(ui->description);
    descriptionOpacity->setOpacity(0.75);
    ui->description->setGraphicsEffect(descriptionOpacity);
}

Entry::~Entry()
{
    delete ui;
    delete generalOpacity;
    delete descriptionOpacity;
}

void Entry::setDescription(const QString &description)
{
    this->description = description;
    ui->description->setText(description);
}

void Entry::setIconPath(const QString &path)
{
    this->iconPath = path;

    QIcon icon = QIcon::fromTheme(path);
    QPixmap pixmap = icon.pixmap(icon.actualSize(QSize(64, 64)));
    ui->icon->setPixmap(pixmap);
}

void Entry::setExec(const QString &exec)
{
    this->exec = exec;
}

