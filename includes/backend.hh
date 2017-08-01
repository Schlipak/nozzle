#ifndef BACKEND_HH
#define BACKEND_HH

#include <QObject>
#include <QProcess>
#include <QThread>
#include <QMutex>
#include <QUuid>

#include <QDebug>

#include "backendworker.hh"

class Backend : public QObject
{
  Q_OBJECT
public:
  explicit        Backend(QString const &exec, QString const params);
  ~Backend();

  void            start();
  void            updateSearchQuery(QString const &query);
  QUuid const     &uid() const;
  void            setName(QString const &name);
  QString const   &name() const;

signals:
  void            newDataAvailable(QString const &);
  void            newResultsAvailable(Backend const &, QString const &);

public slots:
  void            handleResults(QString const &);

private:
  QUuid           mUid;
  BackendWorker   *mWorker;
  QThread         mWorkerThread;

  QString         mProgram;
  QStringList     mParams;
  QString         mName;
};

#endif // BACKEND_HH
