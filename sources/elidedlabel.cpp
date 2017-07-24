#include "elidedlabel.hh"


ElidedLabel::ElidedLabel(QWidget *parent, Qt::WindowFlags f) :
    QLabel(parent, f),
    originalText("")
{
    setAlignment(Qt::AlignRight | Qt::AlignVCenter);
}

ElidedLabel::ElidedLabel(const QString &text, QWidget *parent, Qt::WindowFlags f) :
    QLabel(text, parent, f),
    originalText(text)
{
    setAlignment(Qt::AlignRight);
}

void ElidedLabel::setText(const QString &string)
{
    QLabel::setText(string);
    originalText = string;
}

void ElidedLabel::resizeEvent(QResizeEvent *event)
{
    QLabel::resizeEvent(event);

    QFontMetrics metrics(font());
    QString elidedText = metrics.elidedText(originalText, Qt::ElideRight, width());
    QLabel::setText(elidedText);
}
