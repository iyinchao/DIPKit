#include "dipelidelabel.h"

DIPElideLabel::DIPElideLabel(QWidget *parent):QLabel(parent)
{
    mode = Qt::ElideRight;
    //update();
}

DIPElideLabel::DIPElideLabel(Qt::TextElideMode elideMode, const QString &text, QWidget *parent):QLabel(parent)
{
    mode = elideMode;
    if(!text.isNull()) setText(text);
    //update();
}

Qt::TextElideMode DIPElideLabel::elideMode()
{
    return mode;
}

void DIPElideLabel::setElideMode(Qt::TextElideMode elideMode)
{
    mode = elideMode;
}

void DIPElideLabel::setText(const QString &text){
    str = text;  //?
    QLabel::setText(str);
}

QString DIPElideLabel::text() const{
    return str;
}

void DIPElideLabel::paintEvent(QPaintEvent *event)
{
    QFontMetrics fm(font());
    if (fm.width(text()) > contentsRect().width()) {
        QString elided_txt;
        elided_txt = this->fontMetrics().elidedText(text(), mode, rect().width()/*, Qt::TextShowMnemonic*/);
        QLabel::setText(elided_txt);
    }else{
       QLabel::setText(str);
    }

    QLabel::paintEvent(event);
}

