#include "backendworker.hh"


BackendWorker::BackendWorker(const QString &program, const QStringList &params)
{
    this->program = program;
    this->params = params;

    qDebug() << "STARTING " << program << " " << params;
    proc = new QProcess(NULL);
    connect(proc, SIGNAL(readyReadStandardOutput()), this, SLOT(newDataOutput()));
    proc->start(program, params);
}

void BackendWorker::newDataInput(const QString &input)
{
    if (proc && proc->isOpen())
    {
        qDebug() << "Passing data to backend -> " << input;
        proc->write((input + QString("\n")).toStdString().c_str());
    }
}

void BackendWorker::newDataOutput()
{
    if (proc && proc->isOpen())
    {
        qDebug() << "Backend has emitted data";
        QString line = proc->readLine();
        emit resultReady(line);
    }
}
