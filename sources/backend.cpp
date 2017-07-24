#include "backend.hh"

Backend::Backend(QObject *parent) :
    QObject(parent)
{
    QSettings settings;

    program = settings.value("backend/exec", "python").toString();
    params << settings.value("backend/params", "backend.py").toString();
    start();
}

Backend::~Backend()
{
    workerThread.quit();
    workerThread.wait();
}

void Backend::start()
{
    worker = new BackendWorker(program, params);
//    worker->moveToThread(&workerThread);
//    connect(&workerThread, SIGNAL(finished()), worker, SLOT(deleteLater()));
    connect(this, SIGNAL(newDataAvailable(QString)), worker, SLOT(newDataInput(QString)));
    connect(worker, SIGNAL(resultReady(QString)), this, SLOT(handleResults(QString)));
    workerThread.start();
}

void Backend::updateSearchQuery(const QString &query)
{
    qDebug() << "Updated search query -> " << query;
    emit newDataAvailable(query);
}

void Backend::handleResults(const QString &results)
{
    emit newResultsAvailable(results);
}

