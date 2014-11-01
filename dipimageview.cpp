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
    scrollArea = new QScrollArea(parent);

    //scrollArea->setStyleSheet("background-color:transparent;");
    scrollArea->setWidget(label);

    layout = new QGridLayout(parent);

    layout->addWidget(scrollArea,0,0);
    layout->addWidget(prompt,0,0);
    layout->addWidget(histo,0,0);
    layout->setContentsMargins(0,0,0,0); //important!
    layout->addLayout(new QGridLayout(parent),0,0);

    setLayout(layout);

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
    return !prompt->isVisible();
}

void DIPImageView::displayHistogram(int channel)
{
    getHistoData();
    histo->setVisible(true);
    histo->setData(image->width(),image->height(), histoData);
    //histo->update();

    if(!isImageLoaded()){
        return;
    }
    if(channel & DIPImageView::CHANNEL_R){

    }
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
            histoData[ct(DIPImageView::CHANNEL_S)][qGray(row[j])]++;
            histoData[ct(DIPImageView::CHANNEL_R)][qRed(row[j])]++;
            histoData[ct(DIPImageView::CHANNEL_G)][qGreen(row[j])]++;
            histoData[ct(DIPImageView::CHANNEL_B)][qBlue(row[j])]++;
            histoData[ct(DIPImageView::CHANNEL_A)][qAlpha(row[j])]++;
            //qDebug()<<qRed(row[j]);
        }
    }

    return histoData[ct(channel)];
}

HistoWidget::HistoWidget(QWidget *parent)
    :QWidget(parent)
{
    channelMarker = 0x0000;

    imgH = -1;
    imgW = -1;
    channelMax = new int[5];

    pen = QPen();
    pen.setColor(QColor(255,0,0,100));
    pen.setCapStyle(Qt::RoundCap);
    pen.setWidth(2);
    pen.setStyle(Qt::SolidLine);

    brush = QBrush();
    brush.setColor(QColor(0,0,0,100));
    brush.setStyle(Qt::SolidPattern);


    //update();
}


void HistoWidget::paintEvent(QPaintEvent *)
{

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setPen(Qt::NoPen);
    painter.setBrush(brush);
    QRect bgSize;

    bgSize.setWidth(this->geometry().width());
    bgSize.setHeight(this->geometry().height());
    if(bgSize.width() * 0.7 < 300){
        bgSize.setWidth(300 / 0.7);
    }
    bgSize.setWidth(bgSize.width() * 0.7);
    bgSize.setHeight(double(bgSize.width()) / 255 * 100 + 20);
    bgSize.setX(5);
    bgSize.setY(5);
    //qDebug()<<this->geometry().height() - bgSize.height() / 2;
    //bgSize.setLeft(this->geometry().width() - bgSize.width() / 2);
    //bgSize.setTopLeft(QPoint(this->geometry().width() - bgSize.width() / 2,this->geometry().height() - bgSize.height() / 2));
    //bgSize.setTop(qFabs(this->geometry().height() - bgSize.height()) / 2);

    painter.drawRoundedRect(bgSize, 5, 5, Qt::AbsoluteSize);

}

void HistoWidget::setData(int imageW, int imageH, int **data)
{
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
    qDebug()<<channelMax[0];
    qDebug()<<channelMax[1];
    qDebug()<<channelMax[2];
    qDebug()<<channelMax[3];
    qDebug()<<channelMax[4];

}

/*void HistoWidget::setPen(const QPen &pen)
{
    painter->setPen(pen);
    update();
}

void HistoWidget::setBrush(const QBrush &brush)
{
    painter->setBrush(brush);
    update();
}*/

void HistoWidget::display(int channel, int mode)
{
    channelMarker = channel;
    //histoData = data;



    update();
}

