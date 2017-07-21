#ifndef ENTRY_HH
#define ENTRY_HH

#include <QWidget>

namespace Ui {
class Entry;
}

class Entry : public QWidget
{
    Q_OBJECT

public:
    explicit Entry(QWidget *parent, QString const &name);
    ~Entry();

private:
    Ui::Entry       *ui;

    QString const   &name;
};

#endif // ENTRY_HH
