#ifndef ENTRY_HH
#define ENTRY_HH

#include <QWidget>
#include <QGraphicsOpacityEffect>
#include <QSettings>
#include <QIcon>
#include <QPixmap>
#include <QSize>
#include <QFontMetrics>

#include <QDebug>

#include "elidedlabel.hh"

namespace Ui {
class Entry;
}

class Entry : public QWidget
{
    Q_OBJECT

public:
    explicit Entry(
        QWidget *parent,
        QString const &name,
        QString const &description = "",
        QString const &iconPath = "",
        QString const &exec = ""
    );
    ~Entry();

    void                    setDescription(QString const &description);
    void                    setIconPath(QString const &path);
    void                    setExec(QString const &exec);
    QString const           &getExec() const;
    void                    applyBorderRadius();

private:
    Ui::Entry               *ui;
    QString const           &mName;

    QString                 mDescription;
    QString                 mIconPath;
    QString                 mExec;

    bool                    mSelected;
};

#endif // ENTRY_HH
