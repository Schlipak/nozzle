#include "backend.hh"

Backend::Backend(const QString &exec, const QString params) :
  QObject(NULL),
  mUid(QUuid::createUuid()),
  mWorker(Q_NULLPTR),
  mProgram(exec),
  mParams(params)
{
  start();
}

Backend::~Backend()
{
  mWorkerThread.quit();
  mWorkerThread.wait();
  delete mWorker;
}

void Backend::start()
{
  mWorker = new BackendWorker(mProgram, mParams, mName);
  //    mWorker->moveToThread(&mWorkerThread);
  //    connect(&mWorkerThread, SIGNAL(finished()), mWorker, SLOT(deleteLater()));
  connect(this, SIGNAL(newDataAvailable(QString)), mWorker, SLOT(newDataInput(QString)));
  connect(mWorker, SIGNAL(resultReady(QString)), this, SLOT(handleResults(QString)));
  mWorkerThread.start();
}

void Backend::updateSearchQuery(const QString &query)
{
  qDebug() << "[BackendController] Updated search query -> " << query;
  emit newDataAvailable(query);
}

const QUuid &Backend::uid() const
{
  return mUid;
}

void Backend::setName(const QString &name)
{
  mName = name;
}

const QString &Backend::name() const
{
  return mName;
}

void Backend::handleResults(const QString &results)
{
  emit newResultsAvailable(*this, results);
}
