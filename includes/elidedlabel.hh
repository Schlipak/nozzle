#ifndef ELIDEDLABEL_HH
#define ELIDEDLABEL_HH

#include <QObject>
#include <QWidget>
#include <QLabel>
#include <QResizeEvent>
#include <QFontMetrics>

#include <QDebug>

class ElidedLabel : public QLabel
{
public:
    ElidedLabel(QWidget *parent = Q_NULLPTR, Qt::WindowFlags f = Qt::WindowFlags());
    ElidedLabel(const QString &text, QWidget *parent = Q_NULLPTR, Qt::WindowFlags f = Qt::WindowFlags());

    void setText(const QString &string);

protected:
    virtual void resizeEvent(QResizeEvent *event);

private:
    QString originalText;
};

#endif // ELIDEDLABEL_HH
