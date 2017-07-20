#include "backendworker.hh"


void BackendWorker::run()
{
    proc = new QProcess(NULL);
    qDebug() << "STARTING " << program;
    proc->start(program, params);
}

void BackendWorker::setParams(const QString &program, const QStringList &params)
{
    this->program = program;
    this->params = params;
}

void BackendWorker::newDataInput(const QString &input)
{
    qDebug() << "NEW DATA INPUT: " << input;
    if (proc && proc->waitForStarted()) {
        proc->write((input + QString("\n")).toStdString().c_str());
        QString line = proc->readLine();
        emit resultReady(line);
    }
}
