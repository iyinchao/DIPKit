#ifndef DIPALPHASCROLLAREA_H
#define DIPALPHASCROLLAREA_H

#include <QScrollArea>
#include <QPainter>
#include <QResizeEvent>
#include <QGridLayout>
#include <QMargins>
#include <QtMath>
#include <QDebug>

#include "dipimagelabel.h"

class DIPAlphaScrollArea : public QScrollArea
{
    Q_OBJECT
public:
    DIPAlphaScrollArea(QWidget *parent = 0);
    void setGridColorA(int r, int g, int b, int a = 255);
    void setGridColorB(int r, int g, int b, int a = 255);
    void setGridSize(int size);
    void setWidget(QWidget *widget);
public slots:
    void contentResize(int w, int h);
protected:
    void paintEvent(QPaintEvent *event);
    void resizeEvent(QResizeEvent *event);
private:
    QColor colorA;
    QColor colorB;
    int size;
    QGridLayout *layout;
    QWidget *wrap;
    QWidget *content;
    int cw, ch = -1;
};

#endif // DIPALPHASCROLLAREA_H
