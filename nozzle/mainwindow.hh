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

    bool                        closed;

    double                      getXOffset();

private slots:
    void                        animateOut();
};

#endif // MAINWINDOW_HH
