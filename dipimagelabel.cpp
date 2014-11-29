#include "dipimagelabel.h"

DIPImageLabel::DIPImageLabel(QWidget *parent)
    :QLabel(parent)
{
    this->setCursor(Qt::CrossCursor);
}

void DIPImageLabel::adjustSize()
{
    QLabel::adjustSize();
    emit __adjustSize(width(), height());
}

void DIPImageLabel::paintEvent(QPaintEvent *event)
{
    QLabel::paintEvent(event);
}

void DIPImageLabel::resizeEvent(QResizeEvent *event)
{
    QLabel::resizeEvent(event);
    //emit __resize(event->size().width(), event->size().height());
    //qDebug()<<"ok";
}

