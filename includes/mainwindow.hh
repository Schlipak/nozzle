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

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>

#include <QLabel>
#include <QDebug>

#include "backendscript.hh"
#include "entry.hh"

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

    QGraphicsDropShadowEffect   *shadow;
    QPropertyAnimation          *anim;
    QTimer                      *animTimer;
    BackendScript               *backend;

    bool                        closed;
    unsigned int                count;

    double                      getXOffset();
    void                        setupUi();

public slots:
    void                        onNewBackendResults(QString const &results);

private slots:
    void                        animateOut();
    void                        on_searchInput_textChanged(const QString &string);
};

#endif // MAINWINDOW_HH
