#include "dipimageview.h"

DIPImageView::DIPImageView(QWidget *parent)
    :QWidget(parent){
    init(parent);

}

void DIPImageView::init(QWidget *)
{
    grayMode = false;
    image = NULL;
    imageOriginal = NULL;
    filePath = NULL;
    histoData = new int*[5];
    histoData[ct(DIPImageView::CHANNEL_R)] = NULL;
    histoData[ct(DIPImageView::CHANNEL_G)] = NULL;
    histoData[ct(DIPImageView::CHANNEL_B)] = NULL;
    histoData[ct(DIPImageView::CHANNEL_A)] = NULL;
    histoData[ct(DIPImageView::CHANNEL_S)] = NULL;

    histo = new DIPHistoWidget(this);
    histo->setGeometry(0,0,500,500);
    histo->setVisible(false);
    histo->setAttribute(Qt::WA_TransparentForMouseEvents);
    //histo->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

    menuBar = new QMenuBar(this);
//    QMenuBar *m = new QMenuBar();
//    QMenu *item = new QMenu(tr("test"));
//    m->addMenu(item);
    /*item->addAction(tr("test2"));
    QWidgetAction *widgetAction=new QWidgetAction(this);
    QSlider * slider = new QSlider(Qt::Horizontal,this);
    slider->setFixedSize(500,30);
    widgetAction->setDefaultWidget(slider);
    item->addAction(widgetAction);*/
    //m->setStyleSheet("background-color: rgba(0,0,0,0%)");

    //m->setContentsMargins(3,3,3,3);


    /*prompt = new QLabel(tr("No Image"),parent);
    prompt->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    prompt->setFont(QFont(tr("Microsoft YaHei"),19, QFont::Bold));
    prompt->setAttribute(Qt::WA_TransparentForMouseEvents);*/

    title = new DIPElideLabel(this);
    title->setFont(QFont(tr("Microsoft YaHei"),10));
    title->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    title->setContentsMargins(3,3,3,3);
    //title->setStyleSheet("QLabel { background-color : #1e1e1a; color : #e1e4e5; }");
    setTitleText(tr("--"));

    label = new QLabel(this);
    label->setScaledContents(true);
    label->setAlignment(Qt::AlignCenter);
    label->setStyleSheet("background-color: rgba(0,0,0,0%)");

    scrollArea = new DIPAlphaScrollArea(this);
    //scrollArea->setGridSize(10);
    //scrollArea->setBackgroundRole(QPalette::Dark);

    //scrollArea->setStyleSheet("background-color:transparent;");
    scrollArea->setWidget(label);

    layout = new QGridLayout(this);


    layout->addWidget(title,0,0);
    layout->addWidget(scrollArea,1,0);
    //layout->addWidget(prompt,1,0);
    layout->addWidget(histo,1,0);
    layout->addWidget(menuBar,2,0);
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

bool DIPImageView::saveImage(const QString &path, const char* ext)
{
    if(isImageLoaded()){
        if(image->save(path, ext)){
            emit _imageSaved(path);
        }
    }
    return false;
}

const QString DIPImageView::saveImageWithDialog()
{
    QStringList mimeTypeFilters;
    foreach (const QByteArray &mimeTypeName, QImageWriter::supportedMimeTypes())
        mimeTypeFilters.append(mimeTypeName);
    mimeTypeFilters.sort();
    mimeTypeFilters.append(QString("application/octet-stream"));

    QFileDialog dialog(this,
                       tr("Save image"),
                       "./");
    dialog.setMimeTypeFilters(mimeTypeFilters);
    dialog.selectMimeTypeFilter("image/png");
    dialog.setAcceptMode(QFileDialog::AcceptSave);

    QString path;

    if (dialog.exec() == QDialog::Accepted) {
            path = dialog.selectedFiles().value(0);
    }

    if(!path.isEmpty()){
        QStringList tmp = path.split(QRegExp("/")).last().split(QRegExp("\\."));
        QString ext = tmp.last();
        if(tmp.length() == 1){
            ext = "PNG";
        }
        std::string str = ext.toStdString();
        const char* extchar = str.c_str();
        if(saveImage(path, extchar)){
            return path;
        }else{
            return QString("");
        }
    }

    return path;
}

bool DIPImageView::loadImage(const QString &path)
{
    if(path.isEmpty()){
        return false;
    }
    if(image != NULL){
        delete image;
        image = NULL;
    }
    if(imageOriginal != NULL){
        delete imageOriginal;
        imageOriginal = NULL;
    }
    image = new QImage(path);
    qDebug()<<image->format();
    if(image->isNull()){
        delete image;
        image = NULL;
        return false;
    }
    if(filePath != NULL)delete filePath;
    filePath = new QString(path);
    if(grayMode){
        label->setPixmap(QPixmap::fromImage(*(convertToGray(image))));
    }else{
        label->setPixmap(QPixmap::fromImage(*image));
    }
    label->adjustSize();

    getHistoData();

    emit _imageLoaded(path);

    return true;
}

const QString DIPImageView::loadImageWithDialog()
{
    QStringList mimeTypeFilters;
    foreach (const QByteArray &mimeTypeName, QImageReader::supportedMimeTypes())
        mimeTypeFilters.append(mimeTypeName);
    mimeTypeFilters.sort();
    mimeTypeFilters.append(QString("application/octet-stream"));

    QFileDialog dialog(this,
                       tr("Load image"),
                       QString("./"));
    dialog.setMimeTypeFilters(mimeTypeFilters);
    dialog.selectMimeTypeFilter("image/png");
    dialog.setAcceptMode(QFileDialog::AcceptOpen);

    QString path;

    if (dialog.exec() == QDialog::Accepted) {
            path = dialog.selectedFiles().value(0);
    }

    if(!path.isEmpty()){
        if(loadImage(path)){
            return path;
        }else{
            return QString("");
        }
    }

    return path;
}

void DIPImageView::setGrayMode(bool on)
{
    if(on){
        grayMode = true;
    }else{
        grayMode = false;
    }
    //refresh
    if(isImageLoaded())setImage(this->image);
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

void DIPImageView::hideHistogram()
{
    histo->hide();
}

int* DIPImageView::getHistoData(int channel, bool recalculate)
{
    QImage* image = convertToGray(this->image);

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

QMenuBar *DIPImageView::getMenuBar()
{
    return menuBar;
}

QImage *DIPImageView::getImage()
{
    if(image == NULL || image->isNull()){
        return 0;
    }else{
        if(grayMode){
            return convertToGray(image);
        }
        return image;
    }
}

QImage *DIPImageView::setImage(QImage *result)
{
    if(imageOriginal != NULL){
        delete imageOriginal;
        imageOriginal = NULL;
        if(image != NULL && !image->isNull()){
            imageOriginal = image;
        }
    }

    image = result;
    if(grayMode){
        label->setPixmap(QPixmap::fromImage(*(convertToGray(image))));
    }else{
        label->setPixmap(QPixmap::fromImage(*image));
    }
    label->adjustSize();

    getHistoData();

    return imageOriginal;
}

QImage *DIPImageView::convertToGray(QImage *source)
{
    int w = source->width(); //image dimensions
    int h = source->height();

    QImage *result = new QImage(w,h,QImage::Format_ARGB32);
    for(int i = 0;i < h; i++){
        QRgb* row = (QRgb*)source->scanLine(i);
        for(int j = 0; j < w; j++){
            int v =  qGray(qRed(row[j]),qGreen(row[j]), qBlue(row[j]));
            result->setPixel(j, i, qRgb(v,v,v));
        }
    }

    return result;
}

