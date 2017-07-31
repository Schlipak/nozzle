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
    BackendWorker(const QString &mProgram, const QStringList &mParams);

signals:
    void            resultReady(const QString &);

public slots:
    void            newDataInput(const QString &);
    void            newDataOutput();

private:
    QProcess        *mProc;
    QThread         mThread;

    QString         mProgram;
    QStringList     mParams;

    void run();
};

#endif // BACKENDWORKER_HH
