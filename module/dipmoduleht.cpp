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
    thresGB = new QGroupBox(tr("Threshold"), parent);
    thresGBL = new QGridLayout(thresGB);
    otsuBt = new QPushButton(tr("OTSU algorithm"), thresGB);
    entropyBt = new QPushButton(tr("Entropy"), thresGB);
    thresSd = new QSlider(Qt::Horizontal, thresGB);
    thresLb = new QLabel(tr("Manual"), thresGB);
    mainLt = new QGridLayout(DIPModuleBase::getUI());

    //thresSd->setTickInterval(1);
    thresSd->setTickPosition(QSlider::TicksBelow);
    thresSd->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    thresSd->setMaximum(255);
    thresSd->setMinimum(0);

    connect(histoEquBt,SIGNAL(clicked()), this, SLOT(__doHistoEqu()));
    connect(otsuBt,SIGNAL(clicked()), this, SLOT(__doOtsu()));
    connect(entropyBt,SIGNAL(clicked()), this, SLOT(__doEntropy()));
    connect(thresSd, SIGNAL(valueChanged(int)), this, SLOT(__doThresManual()));

    histoGBL->addWidget(histoEquBt,0,0,1,1,Qt::AlignCenter);
    thresGBL->addWidget(otsuBt,0,0,1,1,Qt::AlignHCenter);
    thresGBL->addWidget(entropyBt,1,0,1,1,Qt::AlignHCenter);
    thresGBL->addWidget(thresSd,2,0,1,1);
    thresGBL->addWidget(thresLb,3,0,1,1,Qt::AlignHCenter);

    histoGB->setLayout(histoGBL);
    thresGB->setLayout(thresGBL);

    mainLt->addWidget(histoGB,0,0,1,1);
    mainLt->addWidget(thresGB,1,0,1,1);

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
        }
    }

    emit _resultImage(result, resultView);
    emit _console(tr("Histogram Equalization done."));
}

void DIPModuleHT::__doOtsu()
{
    if(!sourceView->isImageLoaded()){
        emit _console(tr("ERROR: Image is not loaded"));
        return;
    }

    QImage *source = sourceView->getImage();

    int w = source->width(); //image dimensions
    int h = source->height();

    int *his = sourceView->getHistoData(DIPImageView::CHANNEL_S, true);
    double max = 0;
    int threshold = 0;

    for(int i = 0; i < 255; i++){
        double N0 = 0;
        double U0 = 0;
        double N1 = 0;
        double U1 = 0;
        for(int j = 0; j <= i; j++){
            N0 += his[j];
            U0 += his[j] * j;
        }
        N0 /= double(w*h);
        if(N0) U0 /= double(N0);
        for(int j = i+1; j <= 255; j++){
            N1 += his[j];
            U1 += his[j] * j;
        }
        N1 /= double(w*h);
        if(N1) U1 /= double(N1);
        double d = N0 * N1 * (U0 - U1) * (U0 - U1);
        if(d > max){
            max = d;
            threshold = i;
        }
    }

    QImage *result = new QImage(w,h,QImage::Format_ARGB32);
    for(int i = 0;i < h; i++){
        QRgb* row = (QRgb*)source->scanLine(i);
        QRgb* rowResult = (QRgb*)result->scanLine(i);
        for(int j = 0; j < w; j++){
            int v = qGray(qRed(row[j]),qGreen(row[j]), qBlue(row[j]));
            rowResult[j] = v >= threshold ? qRgba(255,255,255,255):qRgba(0,0,0,255);
            //result->setPixel(j, i, v > threshold ? qRgba(255,255,255,255):qRgba(0,0,0,255));
        }
    }

    emit _resultImage(result, resultView);
    emit _console(tr("OTSU operation done. Threshold: %1").arg(threshold));
}

//http://www.johnloomis.org/ece563/notes/basics/entropy/entropy.html
void DIPModuleHT::__doEntropy()
{
    if(!sourceView->isImageLoaded()){
        emit _console(tr("ERROR: Image is not loaded"));
        return;
    }

    QImage *source = sourceView->getImage();
    int w = source->width(); //image dimensions
    int h = source->height();
    int *his = sourceView->getHistoData(DIPImageView::CHANNEL_S, false);

    double hisN[256] ={0}; //normalized histogram

    for(int i = 0; i < 256; i++){
        hisN[i] = double(his[i]) / double(w * h);
    }

    double max;
    int threshold = 0;

    for(int i = 0; i < 256; i++){
        double h1 = 0; //sum of probablity of black set
        double h2 = 0;
        double HB = 0; //entropy of black set;
        double HW = 0; //entropy of white set;
        for(int j = 0; j < 256; j++){
            if(j <= i){
                h1 += hisN[j];
            }else{
                h2 += hisN[j];
            }
        }
        for(int j = 0; j < 256; j++){
            if(hisN[j] == 0) continue;
            if(j <= i){
                HB -= (hisN[j] / h1) * qLn(hisN[j] / h1);
            }else{
                HW -= (hisN[j] / h2) * qLn(hisN[j] / h2);
            }
        }

        if(i == 0){
            max = HB + HW;
            threshold = 0;
        }else{
            if((HB + HW) > max){
                max = (HB + HW);
                threshold = i;
            }
        }
    }

    QImage *result = new QImage(w,h,QImage::Format_ARGB32);
    for(int i = 0;i < h; i++){
        QRgb* row = (QRgb*)source->scanLine(i);
        QRgb* rowResult = (QRgb*)result->scanLine(i);
        for(int j = 0; j < w; j++){
            int v = qGray(qRed(row[j]),qGreen(row[j]), qBlue(row[j]));
            rowResult[j] = v >= threshold ? qRgba(255,255,255,255):qRgba(0,0,0,255);
        }
    }

    emit _resultImage(result, resultView);
    emit _console(tr("Entropy operation done. Threshold: %1").arg(threshold));
}

void DIPModuleHT::__doThresManual()
{
    if(!sourceView->isImageLoaded()){
        emit _console(tr("ERROR: Image is not loaded"));
        return;
    }

    QImage *source = sourceView->getImage();

    int w = source->width(); //image dimensions
    int h = source->height();
    int threshold = thresSd->value();

    if(threshold >= 255) threshold = 255;
    if(threshold <= 0) threshold = 0;

    QImage *result = new QImage(w,h,QImage::Format_ARGB32);
    for(int i = 0;i < h; i++){
        QRgb* row = (QRgb*)source->scanLine(i);
        QRgb* rowResult = (QRgb*)result->scanLine(i);
        for(int j = 0; j < w; j++){
            int v = qGray(qRed(row[j]),qGreen(row[j]), qBlue(row[j]));
            rowResult[j] = v >= threshold ? qRgba(255,255,255,255):qRgba(0,0,0,255);
            //result->setPixel(j, i, v > threshold ? qRgba(255,255,255,255):qRgba(0,0,0,255));
        }
    }

    thresLb->setText(QString("Manual Threshold: %1").arg(thresSd->value()));

    emit _resultImage(result, resultView);
    emit _console(QString(tr("Manual change threshold: %1")).arg(threshold));

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
