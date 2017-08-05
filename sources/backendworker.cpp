#include "backendworker.hh"

BackendWorker::BackendWorker(const QString &program, const QStringList &params, QString const &name) :
  mProgram(program),
  mParams(params),
  mName(name)
{
  qDebug() << QString("[Worker %1] Starting %2 with params").arg(mName, program).toStdString().c_str() << params;
  mProc = new QProcess(NULL);
  connect(mProc, SIGNAL(readyReadStandardOutput()), this, SLOT(newDataOutput()));
  connect(mProc, SIGNAL(stateChanged(QProcess::ProcessState)), this, SLOT(onStateChanged(QProcess::ProcessState)));

  QSettings           settings;
  QProcessEnvironment env = QProcessEnvironment::systemEnvironment();

  settings.beginGroup("env");
  foreach (const QString &key, settings.childKeys()) {
    env.insert(key, settings.value(key).toString());
  }
  settings.endGroup();

  mProc->setProcessEnvironment(env);
  mProc->start(program, params);
  mProc->waitForStarted();
}

BackendWorker::~BackendWorker()
{
  disconnect(mProc, SIGNAL(stateChanged(QProcess::ProcessState)), this, SLOT(onStateChanged(QProcess::ProcessState)));
  if (mProc)
  {
    qDebug() << QString("[Worker %1] In destructor, state is").arg(mName).toStdString().c_str() << mProc->state();
    mProc->terminate();
    mProc->waitForFinished();
    delete mProc;
  }
}

void BackendWorker::newDataInput(const QString &input)
{
  if (mProc && mProc->state() == QProcess::ProcessState::Running)
  {
    qDebug() << QString("[Worker %1] Passing data to backend -> %2").arg(mName, input).toStdString().c_str();
    mProc->write((input + QString("\n")).toStdString().c_str());
  }
}

void BackendWorker::newDataOutput()
{
  if (mProc && mProc->state() == QProcess::ProcessState::Running)
  {
    qDebug() << QString("[Worker %1] Backend has emitted data").arg(mName).toStdString().c_str();
    QString line = mProc->readLine();
    emit resultReady(line);
  }
}

void BackendWorker::onStateChanged(QProcess::ProcessState newState)
{
  qDebug() << QString("[Worker %1] State changed ->").arg(mName).toStdString().c_str() << newState << "with error" << mProc->errorString();
  if (newState == QProcess::NotRunning)
  {
    QSettings settings;
    QTimer::singleShot(
      settings.value("style/animation-duration", 400).toInt() + 400,
      this,
      SLOT(emitBackendCrash())
    );
  }
}

void BackendWorker::emitBackendCrash()
{
  qDebug() << mProc->readAllStandardError();
  emit resultReady(
    QString(
      "{\"results\":[{\"name\":\"%1\",\"description\":\"Backend stopped working, please check config and environment\",\"icon\":\":icons/backend-crash\"}]}"
    ).arg(mName)
  );
}
