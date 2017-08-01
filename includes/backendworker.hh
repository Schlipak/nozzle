#ifndef BACKENDWORKER_HH
#define BACKENDWORKER_HH

#include <QObject>
#include <QThread>
#include <QString>
#include <QStringList>
#include <QProcess>
#include <QSettings>
#include <QTimer>

#include <QDebug>

class BackendWorker : public QObject
{
  Q_OBJECT
public:
  BackendWorker(const QString &mProgram, const QStringList &mParams, const QString &name);
  ~BackendWorker();

signals:
  void            resultReady(const QString &);

public slots:
  void            newDataInput(const QString &);
  void            newDataOutput();
  void            onStateChanged(QProcess::ProcessState newState);

private:
  QProcess        *mProc;
  QThread         mThread;

  QString const     &mProgram;
  QStringList const &mParams;
  QString const     &mName;

  void run();

private slots:
  void          emitBackendCrash();
};

#endif // BACKENDWORKER_HH
