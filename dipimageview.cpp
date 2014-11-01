#include "dipimageview.h"

DIPImageView::DIPImageView(QWidget *parent)
    :QWidget(parent){
    init(parent);
}

DIPImageView::DIPImageView(const QString &promptText, QWidget *parent)
    :QWidget(parent){
    init(parent);
    prompt->setText(promptText);

}

void DIPImageView::init(QWidget *parent)
{
    image = NULL;
    imageSave = NULL;
    filePath = NULL;
    histoData = new int*[5];
    histoData[ct(DIPImageView::CHANNEL_R)] = NULL;
    histoData[ct(DIPImageView::CHANNEL_G)] = NULL;
    histoData[ct(DIPImageView::CHANNEL_B)] = NULL;
    histoData[ct(DIPImageView::CHANNEL_A)] = NULL;
    histoData[ct(DIPImageView::CHANNEL_S)] = NULL;

    histo = new HistoWidget(parent);
    histo->setGeometry(0,0,500,500);
    histo->setVisible(false);
    histo->setAttribute(Qt::WA_TransparentForMouseEvents);
    //histo->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);


    prompt = new QLabel(tr("No Image"),parent);
    prompt->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    prompt->setFont(QFont(tr("Microsoft YaHei"),19, QFont::Bold));
    prompt->setAttribute(Qt::WA_TransparentForMouseEvents);

    label = new QLabel();
    label->setScaledContents(true);
    label->setAlignment(Qt::AlignCenter);

    scrollArea = new QScrollArea(parent);
    //scrollArea->setBackgroundRole(QPalette::Dark);

    //scrollArea->setStyleSheet("background-color:transparent;");
    scrollArea->setWidget(label);

    layout = new QGridLayout(parent);

    layout->addWidget(scrollArea,0,0);
    layout->addWidget(prompt,0,0);
    layout->addWidget(histo,0,0);
    layout->setContentsMargins(0,0,0,0); //important!
    layout->addLayout(new QGridLayout(parent),0,0);

    setLayout(layout);

    setMinimumHeight(300);
    setMinimumWidth(300);

}

int DIPImageView::ct(int channel)
{
    switch(channel){
    case DIPImageView::CHANNEL_R:
        return 0;
        break;
    case DIPImageView::CHANNEL_G:
        return 1;
        break;
    case DIPImageView::CHANNEL_B:
        return 2;
        break;
    case DIPImageView::CHANNEL_A:
        return 3;
        break;
    case DIPImageView::CHANNEL_S:
        return 4;
        break;
    default:
        return 4;
    }
}

bool DIPImageView::loadImage(QString &path)
{
    if(path.isEmpty()){
        return false;
    }
    if(image != NULL){
        delete image;
        image = NULL;
    }
    image = new QImage(path);
    if(image->isNull()){
        return false;
    }
    if(filePath != NULL)delete filePath;
    filePath = new QString(path);
    label->setPixmap(QPixmap::fromImage(*image));
    label->adjustSize();
    prompt->setVisible(false);

    emit _imageLoaded();

    return true;
}

bool DIPImageView::isImageLoaded()
{
    //return !prompt->isVisible();
    //qDebug()<<image->isNull();
    return ! image == NULL;
}

void DIPImageView::displayHistogram(int channel, int mode)
{
    if(!isImageLoaded()){
        return;
    }
    getHistoData();
    histo->setVisible(true);
    histo->setData(image->width(),image->height(), histoData);
    histo->display(channel, mode);
    //histo->update();
}

int* DIPImageView::getHistoData(int channel, bool recalculate)
{
    if(!isImageLoaded()){
        return NULL;
    }
    if(histoData[ct(channel)] != NULL && recalculate == false){
        return histoData[ct(channel)];
    }else if(histoData[ct(channel)] != NULL){
        delete histoData[ct(DIPImageView::CHANNEL_R)];
        delete histoData[ct(DIPImageView::CHANNEL_G)];
        delete histoData[ct(DIPImageView::CHANNEL_B)];
        delete histoData[ct(DIPImageView::CHANNEL_A)];
        delete histoData[ct(DIPImageView::CHANNEL_S)];
        histoData[ct(DIPImageView::CHANNEL_R)] = NULL;
        histoData[ct(DIPImageView::CHANNEL_G)] = NULL;
        histoData[ct(DIPImageView::CHANNEL_B)] = NULL;
        histoData[ct(DIPImageView::CHANNEL_A)] = NULL;
        histoData[ct(DIPImageView::CHANNEL_S)] = NULL;
    }

    int w = image->width(); //image dimensions
    int h = image->height();

    histoData[ct(DIPImageView::CHANNEL_R)] = new int[256];
    histoData[ct(DIPImageView::CHANNEL_G)] = new int[256];
    histoData[ct(DIPImageView::CHANNEL_B)] = new int[256];
    histoData[ct(DIPImageView::CHANNEL_A)] = new int[256];
    histoData[ct(DIPImageView::CHANNEL_S)] = new int[256];

    memset(histoData[ct(DIPImageView::CHANNEL_R)],0,sizeof(int)*256);
    memset(histoData[ct(DIPImageView::CHANNEL_G)],0,sizeof(int)*256);
    memset(histoData[ct(DIPImageView::CHANNEL_B)],0,sizeof(int)*256);
    memset(histoData[ct(DIPImageView::CHANNEL_A)],0,sizeof(int)*256);
    memset(histoData[ct(DIPImageView::CHANNEL_S)],0,sizeof(int)*256);

    for(int i = 0;i < h; i++){
        QRgb* row = (QRgb*)image->scanLine(i);
        for(int j = 0; j < w; j++){
            histoData[ct(DIPImageView::CHANNEL_R)][qRed(row[j])]++;
            histoData[ct(DIPImageView::CHANNEL_G)][qGreen(row[j])]++;
            histoData[ct(DIPImageView::CHANNEL_B)][qBlue(row[j])]++;
            histoData[ct(DIPImageView::CHANNEL_A)][qAlpha(row[j])]++;
            histoData[ct(DIPImageView::CHANNEL_S)][qGray(row[j])]++;

        }
    }

    return histoData[ct(channel)];
}

HistoWidget::HistoWidget(QWidget *parent)
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


void HistoWidget::__drawEachChannel(QPainter &painter, int channel, int mode)
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
    for(int i = 0; i < 256; i++){
        if(mode == DIPImageView::HG::ABSOLUTE){
            h = histoData[DIPImageView::ct(channel)][i] / (double)(imgW * imgH) * 100;
        }else if(mode == DIPImageView::HG::RELATIVE){
            h = histoData[DIPImageView::ct(channel)][i] / (double)channelMax[DIPImageView::ct(channel)] * 100;
        }
        painter.drawLine(mg_l + pd_l + i, mg_t + pd_t + ct_h, mg_l + pd_l + i, mg_t + pd_t + ct_h - h);

    }

    painter.setRenderHint(QPainter::Antialiasing, true);

    painter.drawText(p, t);
}

void HistoWidget::paintEvent(QPaintEvent *)
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

void HistoWidget::setData(int imageW, int imageH, int **data){
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

void HistoWidget::display(int channel, int mode)
{
    channelMarker = channel;
    //histoData = data;

    this->mode = mode;

    update();
}

