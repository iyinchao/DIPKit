#include "dipmoduleht.h"


DIPModuleHT::DIPModuleHT(DIPImageView *source, DIPImageView *result, QWidget *parent)
    :DIPModuleBase(source, result, parent){
    initUI();
    moduleName = new QString(tr("Histogram & threshold"));
}

void DIPModuleHT::initUI()
{
    DIPModuleBase::initUI();

    histoGB = new QGroupBox(tr("Histogram analysis"), parent);
    histoGBL = new QGridLayout(histoGB);
    histoEquBt = new QPushButton(tr("Histogram Equalization"), histoGB);
    mainLt = new QGridLayout(DIPModuleBase::getUI());

    connect(histoEquBt,SIGNAL(clicked()), this, SLOT(__doHistoEqu()));

    histoGBL->addWidget(histoEquBt,0,0,1,1,Qt::AlignCenter);

    histoGB->setLayout(histoGBL);

    mainLt->addWidget(histoGB,0,0,1,1);

    DIPModuleBase::getUI()->setLayout(mainLt);
}

void DIPModuleHT::__doHistoEqu()
{
    if(!sourceView->isImageLoaded()){
        emit _console(tr("ERROR: Image is not loaded"));
        return;
    }

    QImage *source = sourceView->getImage();

    int w = source->width(); //image dimensions
    int h = source->height();

    int cdf[256] = {0};
    int hisE[256] ={0}; //mapping of equalized gray scale

    int *his = sourceView->getHistoData(DIPImageView::CHANNEL_S, false);

    cdf[0] = his[0];
    for(int i = 1;i < 256; i++){
        cdf[i] = his[i] + cdf[i-1];
        //qDebug()<<cdf[i];
    }

    for(int i = 1;i < 256; i++){
        hisE[i] = (double)(cdf[i] - cdf[0]) / (w * h - cdf[0]) * 255 + 0.5;
    }

    QImage *result = new QImage(w,h,QImage::Format_ARGB32);
    for(int i = 0;i < h; i++){
        QRgb* row = (QRgb*)source->scanLine(i);
        for(int j = 0; j < w; j++){
            int v = hisE[qGray(qRed(row[j]),qGreen(row[j]), qBlue(row[j]))];
            result->setPixel(j, i, qRgba(v,v,v, 255));
            if(i == 10 && j == 10) qDebug()<<v;
        }
    }

    emit _resultImage(result, resultView);
    emit _console(tr("Histogram Equalization done."));
}

//alg
//Histogram Equalization
//QImage* DIPModuleHT::hisEqu(QImage *source, bool isGrayScale){
//    int w = source->width(); //image dimensions
//    int h = source->height();

//    int his[256] = {0}; //the histogram of gray scale
//    int cdf[256] = {0}; //the accumulate
//    int hisE[256] ={0}; //mapping of equalized gray scale

//    for(int i = 0;i < h; i++){
//        QRgb* row = (QRgb*)source->scanLine(i);
//        for(int j = 0; j < w; j++){
//            his[qGray(row[j])]++;
//        }
//    }

//    cdf[0] = his[0];
//    for(int i = 1;i < 256; i++){
//        cdf[i] = his[i] + cdf[i-1];
//        //qDebug()<<cdf[i];
//    }

//    for(int i = 1;i < 256; i++){
//        hisE[i] = (double)(cdf[i] - cdf[0]) / (w * h - cdf[0]) * 255 + 0.5;
//    }

//    QImage *result = new QImage(w,h,QImage::Format_RGB32);
//    for(int i = 0;i < h; i++){
//        QRgb* row = (QRgb*)source->scanLine(i);
//        for(int j = 0; j < w; j++){
//            int v = hisE[qGray(row[j])];
//            result->setPixel(j, i, qRgb(v,v,v));
//        }
//    }

//    return result;
//    /*QRgb* pointer = (QRgb*)sourceImage->scanLine(3);
//    qDebug()<<qRed(pointer[4]);*/

//}
