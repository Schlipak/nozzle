#include "backend.hh"

Backend::Backend(QObject *parent) :
    QObject(parent),
    mWorker(Q_NULLPTR)
{
    QSettings settings;

    mProgram = settings.value("backend/exec", "python").toString();
    mParams << settings.value("backend/params", "backend.py").toString();
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
    mWorker = new BackendWorker(mProgram, mParams);
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

void Backend::handleResults(const QString &results)
{
    emit newResultsAvailable(results);
}

