#include "backendscript.hh"

BackendScript::BackendScript(QObject *parent, QSettings const &settings) :
    QObject(parent),
    settings(settings)
{
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
    std::cout << "UPDATED SEARCH QUERY: " << query.toStdString() << std::endl;
    emit newDataAvailable(query);
}

void BackendScript::handleResults(const QString &results)
{
    std::cout << results.toStdString() << std::endl;
}

