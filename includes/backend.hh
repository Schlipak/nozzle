#ifndef BACKEND_HH
#define BACKEND_HH

#include <QObject>
#include <QSettings>
#include <QProcess>
#include <QThread>
#include <QMutex>

#include <QDebug>

#include "backendworker.hh"

class Backend : public QObject
{
    Q_OBJECT
public:
    explicit        Backend(QObject *parent = 0);
    ~Backend();

    void            start();
    void            updateSearchQuery(QString const &query);

signals:
    void            newDataAvailable(QString const &);
    void            newResultsAvailable(QString const &);

public slots:
    void            handleResults(QString const &);

private:
    BackendWorker   *worker;
    QThread         workerThread;

    QString         program;
    QStringList     params;
};

#endif // BACKEND_HH
