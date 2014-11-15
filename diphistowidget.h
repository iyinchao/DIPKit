#ifndef DIPHISTOWIDGET_H
#define DIPHISTOWIDGET_H

#include <QWidget>
#include "dipimageview.h"

class DIPHistoWidget : public QWidget
{
    Q_OBJECT
private:
    QImage *cache;
    QPen pen;
    QBrush brush;
    int channelMarker;
    int **histoData;
    int imgH;
    int imgW;
    int *channelMax;
    bool isDisplay;
    int mode;
    int pd_t = 40;
    int pd_l = 20;
    int pd_r = 20;
    int pd_b = 20;
    int mg_l = 5;
    int mg_t = 5;
    int ct_h = 100;
    int ct_w = 256;
    int rc = 5;
    bool __redraw;
    void __drawEachChannel(QPainter &painter, int channel, int mode);
protected:
    void paintEvent(QPaintEvent *event);
public slots:
    void setData(int imageW, int imageH, int** data);
    void display(int channel, int mode);
    void hide();
    void update();

public:
    DIPHistoWidget(QWidget *parent = 0);

};

#endif // DIPHISTOWIDGET_H
