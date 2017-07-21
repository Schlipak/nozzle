#include "entry.hh"
#include "ui_entry.h"

Entry::Entry(QWidget *parent, const QString &name) :
    QWidget(parent),
    ui(new Ui::Entry),
    name(name)
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

    opacity = new QGraphicsOpacityEffect(ui->wrapper);
    opacity->setOpacity(0.75);
    ui->wrapper->setGraphicsEffect(opacity);
}

Entry::~Entry()
{
    delete ui;
}
