#include "backendworker.hh"


void BackendWorker::run()
{
    proc = new QProcess(NULL);
    std::cout << "STARTING " << program.toStdString() << std::endl;
    proc->start(program, params);
}

void BackendWorker::setParams(const QString &program, const QStringList &params)
{
    this->program = program;
    this->params = params;
}

void BackendWorker::newDataInput(const QString &input)
{
    std::cout << "NEW DATA INPUT: " << input.toStdString() << std::endl;
    if (proc && proc->waitForStarted()) {
        proc->write((input + QString("\n")).toStdString().c_str());
        QString line = proc->readLine();
        emit resultReady(line);
    }
}
