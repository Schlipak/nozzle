#include "backendworker.hh"


BackendWorker::BackendWorker(const QString &program, const QStringList &params)
{
    this->mProgram = program;
    this->mParams = params;

    qDebug() << "[WORKER] Starting" << program << params;
    mProc = new QProcess(NULL);
    connect(mProc, SIGNAL(readyReadStandardOutput()), this, SLOT(newDataOutput()));
    mProc->start(program, params);
}

void BackendWorker::newDataInput(const QString &input)
{
    if (mProc && mProc->isOpen())
    {
        qDebug() << "[Worker] Passing data to backend -> " << input;
        mProc->write((input + QString("\n")).toStdString().c_str());
    }
}

void BackendWorker::newDataOutput()
{
    if (mProc && mProc->isOpen())
    {
        qDebug() << "[Worker] Backend has emitted data";
        QString line = mProc->readLine();
        emit resultReady(line);
    }
}
