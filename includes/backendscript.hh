#ifndef BACKENDSCRIPT_HH
#define BACKENDSCRIPT_HH

#include <QObject>
#include <QSettings>
#include <QProcess>
#include <QThread>
#include <QMutex>

#include <QDebug>

#include "backendworker.hh"

class BackendScript : public QObject
{
    Q_OBJECT
public:
    explicit        BackendScript(QObject *parent = 0);

    void            start();
    void            updateSearchQuery(QString const &query);

signals:
    void            newDataAvailable(QString const &);

public slots:
    void            handleResults(QString const &);

private:
    BackendWorker   *worker;
    QString         program;
    QStringList     params;
};

#endif // BACKENDSCRIPT_HH
