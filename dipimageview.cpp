#include "dipimageview.h"

DIPImageView::DIPImageView(QWidget *parent)
    :QWidget(parent){
    init(parent);
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

    histo = new DIPHistoWidget(parent);
    histo->setGeometry(0,0,500,500);
    histo->setVisible(false);
    histo->setAttribute(Qt::WA_TransparentForMouseEvents);
    //histo->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

    QMenuBar *m = new QMenuBar(parent);
    QMenu *item = new QMenu(tr("test"));
    m->addMenu(item);
    item->addAction(tr("test2"));
    QWidgetAction *widgetAction=new QWidgetAction(this);
    QSlider * slider = new QSlider(Qt::Horizontal,this);
    slider->setFixedSize(500,30);
    widgetAction->setDefaultWidget(slider);
    item->addAction(widgetAction);
    //m->setContentsMargins(0,0,0,0);


    /*prompt = new QLabel(tr("No Image"),parent);
    prompt->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    prompt->setFont(QFont(tr("Microsoft YaHei"),19, QFont::Bold));
    prompt->setAttribute(Qt::WA_TransparentForMouseEvents);*/

    title = new DIPElideLabel(parent);
    title->setFont(QFont(tr("Microsoft YaHei"),10));
    title->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    title->setContentsMargins(3,3,3,3);
    //title->setStyleSheet("QLabel { background-color : #1e1e1a; color : #e1e4e5; }");
    setTitleText(tr("--"));

    label = new QLabel(parent);
    label->setScaledContents(true);
    label->setAlignment(Qt::AlignCenter);
    label->setStyleSheet("background-color: rgba(0,0,0,0%)");

    scrollArea = new DIPAlphaScrollArea(parent);
    //scrollArea->setGridSize(10);
    //scrollArea->setBackgroundRole(QPalette::Dark);

    //scrollArea->setStyleSheet("background-color:transparent;");
    scrollArea->setWidget(label);

    layout = new QGridLayout(parent);



    layout->addWidget(title,0,0);
    layout->addWidget(scrollArea,1,0);
    //layout->addWidget(prompt,1,0);
    layout->addWidget(histo,1,0);

    layout->addWidget(m,2,0);

    layout->setContentsMargins(0,0,0,0); //important!
    layout->setSpacing(0);
    //layout->addLayout(new QGridLayout(parent),0,0);

    setLayout(layout);

    setMinimumHeight(300);
    setMinimumWidth(300);

}

void DIPImageView::setTitleText(QString &text)
{
    title->setText(text);
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
            histoData[ct(DIPImageView::CHANNEL_A)][qAlpha(row[j])]++;
            if(qAlpha(row[j]) < 1){
                continue;
            }
            histoData[ct(DIPImageView::CHANNEL_R)][qRed(row[j])]++;
            histoData[ct(DIPImageView::CHANNEL_G)][qGreen(row[j])]++;
            histoData[ct(DIPImageView::CHANNEL_B)][qBlue(row[j])]++;
            histoData[ct(DIPImageView::CHANNEL_S)][qGray(row[j])]++;
            //if(i == 510 && j == 138) qDebug()<<qAlpha(row[j]);

            /*if(qRed(row[j]) == 0 && qAlpha(row[j]) != 255){
                t++;
                qDebug()<<j<<", "<<i<<" Alpha:"<<qAlpha(row[j]) / 2.55;
            }*/

        }
    }

    //histoData[ct(DIPImageView::CHANNEL_R)][0] /= 10;

//    for(int i = 0;i < 256; i++){
//        //qDebug()<<histoData[ct(DIPImageView::CHANNEL_R)][i];
//    }
    //histoData[ct(DIPImageView::CHANNEL_R)][0] = 0;

    return histoData[ct(channel)];
}

