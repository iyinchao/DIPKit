#include "dipimagelabel.h"

DIPImageLabel::DIPImageLabel(QWidget *parent)
    :QLabel(parent)
{
    nowColor = new QColor(0,0,0,0);
}

void DIPImageLabel::adjustSize()
{
    QLabel::adjustSize();
    emit __adjustSize(width(), height());
}

void DIPImageLabel::setImage(QImage *image)
{
    this->image = image;
    this->setPixmap(QPixmap::fromImage(*image));
}

QRgb DIPImageLabel::getPixel(int x, int y)
{
    if(!this->pixmap()) return 0;
    if(x >= 0 && x < image->width() && y >= 0 && y < image->height()){
        return image->pixel(x, y);
    }else{
        return qRgba(0,0,0,0);
    }
}

void DIPImageLabel::clear()
{
    this->image = NULL;
    QLabel::clear();
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

void DIPImageLabel::mousePressEvent(QMouseEvent *event)
{
    if(!this->pixmap())return;

    if(event->button() == Qt::LeftButton){
        this->setCursor(Qt::CrossCursor);
        setMouseTracking(true);
    }
    this->mouseMoveEvent(event);

}

void DIPImageLabel::mouseReleaseEvent(QMouseEvent *event)
{
    if(!this->pixmap())return;

    if(event->button() == Qt::LeftButton){
        this->setCursor(Qt::ArrowCursor);
        setMouseTracking(false);
        emit __hideColorTag();
    }
}

void DIPImageLabel::mouseMoveEvent(QMouseEvent *event)
{
    if(!this->pixmap())return;

    int w = image->width();
    int h = image->height();
    if(event->pos().x() >= 0 && event->pos().x() < w
            && event->pos().y() >= 0 && event->pos().y() < h){
        QRgb pixel = image->pixel(event->pos().x(), event->pos().y());
        nowColor->setRed(qRed(pixel));
        nowColor->setGreen(qGreen(pixel));
        nowColor->setBlue(qBlue(pixel));
        nowColor->setAlpha(qAlpha(pixel));
    }else{
        nowColor->setRgb(0, 0, 0, 0);
    }
    emit __showColorTag(nowColor,
                        this->x() + event->pos().x(),
                        this->y() + event->pos().y(),
                        event->pos().x(),
                        event->pos().y());
}



