#include "diphistowidget.h"

DIPHistoWidget::DIPHistoWidget(QWidget *parent)
    :QWidget(parent)
{
    channelMarker = 0x0000;

    isDisplay = false;

    imgH = -1;
    imgW = -1;
    channelMax = new int[5];

    pen = QPen();
    pen.setColor(QColor(255,0,0,100));
    pen.setCapStyle(Qt::RoundCap);
    pen.setWidth(2);
    pen.setStyle(Qt::SolidLine);

    brush = QBrush();
    brush.setColor(QColor(0,0,0,170));
    brush.setStyle(Qt::SolidPattern);
    //update();
}


void DIPHistoWidget::__drawEachChannel(QPainter &painter, int channel, int mode)
{
    QPoint p;
    QString t;
    switch(channel){
    case DIPImageView::CHANNEL_R:
        pen.setColor(QColor(255,0,0,70));
        p.setX(mg_l + pd_l + ct_w - 50);
        p.setY(mg_t + pd_t - 15);
        t = tr("R");
        break;
    case DIPImageView::CHANNEL_G:
        pen.setColor(QColor(0,255,0,70));
        p.setX(mg_l + pd_l + ct_w - 40);
        p.setY(mg_t + pd_t - 15);
        t = tr("G");
        break;
    case DIPImageView::CHANNEL_B:
        pen.setColor(QColor(0,0,255,70));
        p.setX(mg_l + pd_l + ct_w - 30);
        p.setY(mg_t + pd_t - 15);
        t = tr("B");
        break;
    case DIPImageView::CHANNEL_A:
        pen.setColor(QColor(0,0,0,70));
        p.setX(mg_l + pd_l + ct_w - 20);
        p.setY(mg_t + pd_t - 15);
        t = tr("A");
        break;
    case DIPImageView::CHANNEL_S:
        pen.setColor(QColor(255,255,255,70));
        p.setX(mg_l + pd_l + ct_w - 10);
        p.setY(mg_t + pd_t - 15);
        t = tr("S");
        break;
    }
    painter.setPen(pen);

    painter.setRenderHint(QPainter::Antialiasing, false);

    double h = 0;

    double maxRatio = (double)channelMax[DIPImageView::ct(channel)] / (imgW * imgH);

    if(maxRatio > 0.01){
        //channelMax[DIPImageView::ct(channel)] = (double)channelMax[DIPImageView::ct(channel)] / (maxRatio * 100 / 2);
        //qDebug()<<channelMax[DIPImageView::ct(channel)];
    }

    for(int i = 0; i < 256; i++){
        if(mode == DIPImageView::HG::ABSOLUTE){
            h = histoData[DIPImageView::ct(channel)][i] / (double)(imgW * imgH) * 100;
        }else if(mode == DIPImageView::HG::RELATIVE){
            h = histoData[DIPImageView::ct(channel)][i] / (double)channelMax[DIPImageView::ct(channel)] * 100;
            if(h > 100) h = 100;
        }
        painter.drawLine(mg_l + pd_l + i, mg_t + pd_t + ct_h, mg_l + pd_l + i, mg_t + pd_t + ct_h - h);
    }

    painter.setRenderHint(QPainter::Antialiasing, true);

    QColor c(pen.color());
    c.setAlpha(150);
    pen.setColor(c);

    painter.setPen(pen);
    painter.drawText(p, t);
}

void DIPHistoWidget::paintEvent(QPaintEvent *)
{

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setPen(Qt::NoPen);
    painter.setBrush(brush);

    QRect bgSize;
    bgSize.setLeft(mg_l);
    bgSize.setTop(mg_t);
    bgSize.setBottom(mg_t + pd_t + pd_b + ct_h);
    bgSize.setRight(mg_l + pd_l + ct_w + pd_r);
    painter.drawRoundedRect(bgSize, rc, rc, Qt::AbsoluteSize);

    pen.setWidth(1);

    painter.setFont(QFont(tr("Microsoft YaHei"), 8));

    if(channelMarker & DIPImageView::CHANNEL_R){
        __drawEachChannel(painter, DIPImageView::CHANNEL_R, mode);
    }

    if(channelMarker & DIPImageView::CHANNEL_G){
         __drawEachChannel(painter, DIPImageView::CHANNEL_G, mode);
    }

    if(channelMarker & DIPImageView::CHANNEL_B){
         __drawEachChannel(painter, DIPImageView::CHANNEL_B, mode);
    }

    if(channelMarker & DIPImageView::CHANNEL_A){
         __drawEachChannel(painter, DIPImageView::CHANNEL_A, mode);
    }

    if(channelMarker & DIPImageView::CHANNEL_S){
         __drawEachChannel(painter, DIPImageView::CHANNEL_S, mode);
    }

    pen.setColor(QColor(255,255,255,100));
    painter.setPen(pen);
    painter.drawLine(mg_l + pd_l, mg_t + pd_t, mg_l + pd_l, mg_t + pd_t + ct_h);
    painter.drawLine(mg_l + pd_l, mg_t + pd_t + ct_h, mg_l + pd_l + ct_w, mg_t + pd_t + ct_h);

    painter.drawText(QPointF(mg_l + pd_l - 8, mg_t + pd_t + ct_h + 12), tr("0"));
    painter.drawText(QPointF(mg_l + pd_l - 10 + ct_w, mg_t + pd_t + ct_h + 12), tr("255"));
    if(mode == DIPImageView::HG::RELATIVE)
        painter.drawText(QPointF(mg_l + pd_l - 14, mg_t + pd_t), tr("1/MAX"));
    else
        painter.drawText(QPointF(mg_l + pd_l - 14, mg_t + pd_t), tr("%"));

    painter.drawText(QPointF(mg_l + pd_l + 100, mg_t + pd_t - 20), tr("Histogram"));

}

void DIPHistoWidget::setData(int imageW, int imageH, int **data){
    imgH = imageH;
    imgW = imageW;
    histoData = data;

    int maxR = 0;
    int maxG = 0;
    int maxB = 0;
    int maxA = 0;
    int maxS = 0;

    for(int i = 0; i < 256; i++){
        if(histoData[DIPImageView::ct(DIPImageView::CHANNEL_R)][i] > maxR){
            maxR = histoData[DIPImageView::ct(DIPImageView::CHANNEL_R)][i];
        }
        if(histoData[DIPImageView::ct(DIPImageView::CHANNEL_G)][i] > maxG){
            maxG = histoData[DIPImageView::ct(DIPImageView::CHANNEL_G)][i];
        }
        if(histoData[DIPImageView::ct(DIPImageView::CHANNEL_B)][i] > maxB){
            maxB = histoData[DIPImageView::ct(DIPImageView::CHANNEL_B)][i];
        }
        if(histoData[DIPImageView::ct(DIPImageView::CHANNEL_A)][i] > maxA){
            maxA = histoData[DIPImageView::ct(DIPImageView::CHANNEL_A)][i];
        }
        if(histoData[DIPImageView::ct(DIPImageView::CHANNEL_S)][i] > maxS){
            maxS = histoData[DIPImageView::ct(DIPImageView::CHANNEL_S)][i];
        }
    }
    channelMax[DIPImageView::ct(DIPImageView::CHANNEL_R)] = maxR;
    channelMax[DIPImageView::ct(DIPImageView::CHANNEL_G)] = maxG;
    channelMax[DIPImageView::ct(DIPImageView::CHANNEL_B)] = maxB;
    channelMax[DIPImageView::ct(DIPImageView::CHANNEL_A)] = maxA;
    channelMax[DIPImageView::ct(DIPImageView::CHANNEL_S)] = maxS;

}

void DIPHistoWidget::display(int channel, int mode)
{
    channelMarker = channel;
    this->mode = mode;

    isDisplay = true;

    update();
}
