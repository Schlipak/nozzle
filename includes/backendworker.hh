#ifndef BACKENDWORKER_HH
#define BACKENDWORKER_HH

#include <QObject>
#include <QThread>
#include <QString>
#include <QStringList>
#include <QProcess>

#include <QDebug>

class BackendWorker : public QObject
{
    Q_OBJECT
public:
    BackendWorker(const QString &program, const QStringList &params);

signals:
    void            resultReady(const QString &);

public slots:
    void            newDataInput(const QString &);
    void            newDataOutput();

private:
    QProcess        *proc;
    QThread         thread;

    QString         program;
    QStringList     params;

    void run();
};

#endif // BACKENDWORKER_HH
