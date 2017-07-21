#include "entry.hh"
#include "ui_entry.h"

Entry::Entry(QWidget *parent, const QString &name) :
    QWidget(parent),
    ui(new Ui::Entry),
    name(name)
{
    ui->setupUi(this);
    ui->name->setText(name);
}

Entry::~Entry()
{
    delete ui;
}
