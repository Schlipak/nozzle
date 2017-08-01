#ifndef MAINWINDOW_HH
#define MAINWINDOW_HH

#include <QMainWindow>
#include <QDesktopWidget>
#include <QGraphicsDropShadowEffect>
#include <QPropertyAnimation>
#include <QEasingCurve>
#include <QRect>
#include <QTimer>
#include <QCloseEvent>
#include <QProcess>
#include <QStringList>
#include <QSettings>
#include <QListWidget>
#include <QListWidgetItem>
#include <QList>
#include <QUuid>
#include <QLabel>
#include <QMutex>

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>

#include <QDebug>

#include "backend.hh"
#include "entry.hh"
#include "entrylistitem.hh"

namespace Ui {
  class MainWindow;
}

class MainWindow : public QMainWindow
{
  Q_OBJECT

public:
  explicit MainWindow(QWidget *parent, QApplication const &app);
  ~MainWindow();

protected:
  void closeEvent(QCloseEvent *event);

private:
  Ui::MainWindow      *ui;
  QApplication const  &app;
  QUuid               mUid;
  QMutex              mMutex;

  QGraphicsDropShadowEffect   *mShadow;
  QPropertyAnimation          *mAnimIntro;
  QPropertyAnimation          *mAnimSize;
  QTimer                      *mAnimIntroTimer;
  Backend                     *mBackend;

  QList<Backend *>            mbackends;

  bool                        mClosed;
  unsigned int                mCount;

  double                      getXOffset();
  void                        setupUi();
  bool                        eventFilter(QObject *target, QEvent *event);
  void                        selectIndex(int offset);
  void                        clearEntries(QUuid const &uid);

public slots:
  void                        onNewBackendResults(const Backend &backend, QString const &results);
  void                        onEntrySelected(QListWidgetItem *item);
  void                        onEnterPressedInInput();

private slots:
  void                        animateOut();
  void                        on_searchInput_textChanged(const QString &string);
};

#endif // MAINWINDOW_HH
