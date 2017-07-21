#include "backendscript.hh"

BackendScript::BackendScript(QObject *parent) :
    QObject(parent)
{
    QSettings settings;

    program = settings.value("backend/exec", "python").toString();
    params << settings.value("backend/params", "backend.py").toString();
}

void BackendScript::start()
{
    worker = new BackendWorker();
    worker->setParams(program, params);
    connect(worker, SIGNAL(resultReady(QString)), this, SLOT(handleResults(QString)));
    connect(worker, SIGNAL(finished()), worker, SLOT(deleteLater()));
    connect(this, SIGNAL(newDataAvailable(QString)), worker, SLOT(newDataInput(QString)));
    worker->start();
}

void BackendScript::updateSearchQuery(const QString &query)
{
    qDebug() << "UPDATED SEARCH QUERY: " << query;
    emit newDataAvailable(query);
}

void BackendScript::handleResults(const QString &results)
{
    qDebug() << results;
}

