#include "backendworker.hh"

BackendWorker::BackendWorker(const QString &program, const QStringList &params, const QString &name) :
  mProgram(program),
  mParams(params),
  mName(name)
{
  qDebug() << "[WORKER] Starting" << program << params;
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
    qDebug() << "[Worker] In destructor, state is" << mProc->state();
    mProc->terminate();
    mProc->waitForFinished();
    delete mProc;
  }
}

void BackendWorker::newDataInput(const QString &input)
{
  if (mProc && mProc->state() == QProcess::ProcessState::Running)
  {
    qDebug() << "[Worker] Passing data to backend -> " << input;
    mProc->write((input + QString("\n")).toStdString().c_str());
  }
}

void BackendWorker::newDataOutput()
{
  if (mProc && mProc->state() == QProcess::ProcessState::Running)
  {
    qDebug() << "[Worker] Backend has emitted data";
    QString line = mProc->readLine();
    emit resultReady(line);
  }
}

void BackendWorker::onStateChanged(QProcess::ProcessState newState)
{
  qDebug() << "[Worker] State changed ->" << newState << "with error" << mProc->errorString();
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
  emit resultReady(
    QString(
      "{\"results\":[{\"name\":\"%1\",\"description\":\"Backend stopped working, please check config and environment\",\"icon\":\":icons/backend-crash\"}]}"
    ).arg(mName)
  );
}
