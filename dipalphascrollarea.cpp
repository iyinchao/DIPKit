#include "dipalphascrollarea.h"

DIPAlphaScrollArea::DIPAlphaScrollArea(QWidget *parent):QScrollArea(parent)
{
    colorA = QColor(255,255,255);
    colorB = QColor(225,225,225);
    size = 10;
    wrap = NULL;
    layout = NULL;
    content = NULL;
}

void DIPAlphaScrollArea::setGridColorA(int r, int g, int b, int a)
{
    colorA.setRed(r);
    colorA.setGreen(g);
    colorA.setBlue(b);
    colorA.setAlpha(a);

    update();
}

void DIPAlphaScrollArea::setGridColorB(int r, int g, int b, int a)
{
    colorB.setRed(r);
    colorB.setGreen(g);
    colorB.setBlue(b);
    colorB.setAlpha(a);

    update();
}

void DIPAlphaScrollArea::setGridSize(int size)
{
    this->size = size;
}

void DIPAlphaScrollArea::setWidget(QWidget *widget)
{
    if(layout) delete layout;
    if(wrap) delete wrap;
    wrap = new QWidget();
    wrap->setStyleSheet("background-color: rgba(0,0,0,0%)");
    wrap->setLayout(new QGridLayout(wrap));
    //w->resize(widget->width(), widget->height());
    layout = dynamic_cast<QGridLayout*>(wrap->layout());
    layout->addWidget(widget,0,0,1,1,Qt::AlignCenter);
    layout->setContentsMargins(0,0,0,0);
    content = widget;
    //connect(dynamic_cast<DIPImageLabel*>widget, SIGNAL())

    QScrollArea::setWidget(wrap);

}

void DIPAlphaScrollArea::contentResize(int w, int h)
{
    cw = w > 0? w : 1;
    ch = h > 0? h : 1;
    if(wrap){
        wrap->resize(cw < this->viewport()->width()? this->viewport()->width():cw,
                 ch < this->viewport()->height()? this->viewport()->height():ch);
    }
    // if(wrap)wrap->resize(cw, ch);
    //this->resizeEvent(NULL);
}

void DIPAlphaScrollArea::paintEvent(QPaintEvent *event)
{
    QScrollArea::paintEvent(event);

    QPainter painter(viewport());

    painter.setPen(Qt::NoPen);
    for (int x = 0; x < width(); x += size) {
        for (int y = 0; y < height(); y += size) {
            painter.save();
            //painter.translate(x, y);
            if((x / size) % 2 == (y / size) % 2){
                painter.fillRect(x, y, size, size, colorA);
            }else{
                painter.fillRect(x, y, size, size, colorB);
            }
            painter.restore();
        }
    }
}

void DIPAlphaScrollArea::resizeEvent(QResizeEvent *event)
{
    //setViewportMargins(5, 0, 0, 0);
    if(cw > 0 && ch > 0 && wrap){
        wrap->resize(cw < this->viewport()->width()? this->viewport()->width():cw,
                     ch < this->viewport()->height()? this->viewport()->height():ch);
        //if(content) qDebug()<<content->width();
    }
    QScrollArea::resizeEvent(event);
}
