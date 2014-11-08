#include "dipalphascrollarea.h"

DIPAlphaScrollArea::DIPAlphaScrollArea(QWidget *parent):QScrollArea(parent)
{
    colorA = QColor(255,255,255);
    colorB = QColor(225,225,225);
    size = 10;
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
