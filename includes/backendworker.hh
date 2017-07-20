#ifndef BACKENDWORKER_HH
#define BACKENDWORKER_HH

#include <QObject>
#include <QThread>
#include <QString>
#include <QStringList>
#include <QProcess>

#include <QDebug>

class BackendWorker : public QThread
{
    Q_OBJECT
public:
    void            setParams(QString const &program, QStringList const &params);

signals:
    void            resultReady(const QString &);

public slots:
    void            newDataInput(const QString &);

private:
    QProcess        *proc;

    QString         program;
    QStringList     params;

    void run();
};

#endif // BACKENDWORKER_HH
