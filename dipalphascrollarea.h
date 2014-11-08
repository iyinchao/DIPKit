#ifndef DIPALPHASCROLLAREA_H
#define DIPALPHASCROLLAREA_H

#include <QScrollArea>
#include <QPainter>

class DIPAlphaScrollArea : public QScrollArea
{
public:
    DIPAlphaScrollArea(QWidget *parent = 0);
    void setGridColorA(int r, int g, int b, int a = 255);
    void setGridColorB(int r, int g, int b, int a = 255);
    void setGridSize(int size);
protected:
    void paintEvent(QPaintEvent *event);
private:
    QColor colorA;
    QColor colorB;
    int size;
};

#endif // DIPALPHASCROLLAREA_H
