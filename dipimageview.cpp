#include "dipimageview.h"

DIPImageView::DIPImageView(QWidget *parent)
    :QWidget(parent){
    init(parent);

}

void DIPImageView::init(QWidget *parent)
{
    grayMode = false;
    titleName = NULL;
    titleInfo = NULL;
    titleOption = NULL;
    image = NULL;
    imageOriginal = NULL;
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
    title->setText(QString("--"));
    //title->setStyleSheet("QLabel { background-color : #1e1e1a; color : #e1e4e5; }");
    //setTitle(tr("--"));

    label = new DIPImageLabel(this);
    label->setScaledContents(true);
    label->setAlignment(Qt::AlignCenter);
    label->setStyleSheet("background-color: rgba(0,0,0,0%)");

    scrollArea = new DIPAlphaScrollArea(this);
    //scrollArea->setGridSize(10);
    //scrollArea->setBackgroundRole(QPalette::Dark);

    //scrollArea->setStyleSheet("background-color:transparent;");
    scrollArea->setWidget(label);
    connect(label, SIGNAL(__adjustSize(int,int)), scrollArea, SLOT(contentResize(int,int)));

    colorTag = new DIPColorTag(scrollArea);
    colorTag->setAttribute(Qt::WA_TransparentForMouseEvents);
    //connect(label, SIGNAL(__showColorTag(QColor*,int,int,int,int)), colorTag, SLOT(showContent(QColor*,int,int,int,int)));
    connect(label, SIGNAL(__showColorTag(QColor*,int,int,int,int)), this, SLOT(__colorTag(QColor*,int,int,int,int)));
    connect(label, SIGNAL(__hideColorTag()), colorTag, SLOT(hideContent()));
    connect(label, SIGNAL(__showColorTag(QColor*,int,int,int,int)), this, SLOT(__emitCTShow(QColor*,int,int,int,int)));
    connect(label, SIGNAL(__hideColorTag()), this, SLOT(__emitCTHide()));

    layout = new QGridLayout(this);

    layout->addWidget(title,0,0);
    layout->addWidget(scrollArea,1,0);
    layout->addWidget(histo,1,0);
    layout->addWidget(colorTag,1,0);
    //layout->addWidget(prompt,1,0);
    layout->addWidget(menuBar,2,0);
    layout->setContentsMargins(0,0,0,0); //important!
    layout->setSpacing(0);
    //layout->addLayout(new QGridLayout(parent),0,0);

    setLayout(layout);

    setMinimumHeight(300);
    setMinimumWidth(300);

}

void DIPImageView::paintEvent(QPaintEvent *event)
{
    QWidget::paintEvent(event);

//    if(isColorPick){
//        QPainter painter(this);
//        QPen pen = QPen();
//        QBrush brush = QBrush();
//        int w = 60;
//        int h = 35;
//        int rc = 5;


//        brush.setColor(QColor(0,0,0,150));
//        brush.setStyle(Qt::SolidPattern);
//        painter.setRenderHint(QPainter::Antialiasing, true);

//        painter.setPen(Qt::NoPen);
//        painter.setBrush(brush);
//        painter.drawRoundedRect(10,10,w,h,rc,rc,Qt::AbsoluteSize);
//    }
}

void DIPImageView::setTitle(QString &text, TITLE component)
{
    if(component == TITLE::ALL){
        if(titleName){
            delete titleName;
            titleName = NULL;
        }
        if(titleOption){
            delete titleOption;
            titleOption = NULL;
        }
        if(titleInfo){
            delete titleInfo;
        }
        titleInfo = new QString(text);
    }
    if(component == TITLE::NAME){
        if(titleName){
            delete titleName;
        }
        titleName = new QString(text);
    }
    QString s = QString("");
    if(titleName){
        s.append(QString("<b>%1</b>").arg(*titleName));
    }
    if(titleName && titleInfo){
        s.append(" - ");
    }
    if(titleInfo){
        s.append(*titleInfo);
    }
    if(titleOption && titleInfo){
        s.append(" - ");
    }
    if(titleOption){
        s.append(*titleOption);
    }
    title->setText(s);
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
    if(!isImageLoaded()){
        return QString("");
    }
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
    QImage *temp = new QImage(path);
    if(temp->isNull()){
        delete temp;
        return false;
    }
    image = new QImage(temp->convertToFormat(QImage::Format_ARGB32));
    delete temp;
    if(filePath != NULL)delete filePath;
    filePath = new QString(path);
    if(grayMode){
//        label->setPixmap(QPixmap::fromImage(*(convertToGray(image))));
        label->setImage(convertToGray(image));
    }else{
        //label->setPixmap(QPixmap::fromImage(*image));
        label->setImage(image);
    }
    label->adjustSize();

    getHistoData();

    emit _imageLoaded(path);
    emit _imageIsSet();

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
    //qDebug()<<image;
    return ! (image == NULL || image->isNull());
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
    histo->update();
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
    QImage* image = grayMode?convertToGray(this->image):this->image;

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
        return NULL;
    }else{
        if(grayMode){
            return convertToGray(image);
        }
        return image;
    }
}

QImage *DIPImageView::setImage(QImage *result, QString *info)
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
        //label->setPixmap(QPixmap::fromImage(*(convertToGray(image))));
        label->setImage(convertToGray(image));
    }else{
        //label->setPixmap(QPixmap::fromImage(*image));
        label->setImage(image);
    }
    label->adjustSize();

    getHistoData();

    if(info){

    }

    emit _imageIsSet();

    return imageOriginal;
}

QImage *DIPImageView::receiveImage()
{
    if(!sender()){
        return NULL;
    }
    DIPImageView *s = (DIPImageView*)(sender()->parent());
    //qDebug()<<s->isImageLoaded();
    if(s->isImageLoaded()){
        QImage *temp = new QImage(*(this->getImage()));
        this->setImage(new QImage(*(s->getImage())));
        s->clearImage();
        return temp;
    }else{
        return NULL;
    }
}

QImage *DIPImageView::clearImage()
{
    if(this->isImageLoaded()){
        QImage *temp = new QImage(*(this->getImage()));
        label->clear();
        if(image != NULL){
            delete image;
            image = NULL;
        }
        if(imageOriginal != NULL){
            delete imageOriginal;
            imageOriginal = NULL;
        }
        if(filePath != NULL){
            delete filePath;
            filePath = NULL;
        }
        histo->setVisible(false);
        return temp;
    }else{
        return NULL;
    }
}

void DIPImageView::__emitCTShow(QColor *color, int mouse_x, int mouse_y, int pic_x, int pic_y)
{
    emit _colorTagShow(color, mouse_x, mouse_y, pic_x, pic_y);
}

void DIPImageView::__emitCTHide()
{
    emit _colorTagHide();
}

void DIPImageView::colorTagShow(QColor *color, int mouse_x, int mouse_y, int pic_x, int pic_y, DIPImageView *ref)
{
    QColor *passiveColor;
    int diffMX, diffMY, diffPX, diffPY;
    if(colorTag->isPassive()){
        if(!ref) ref = this;
        diffMX = ref->horizontalScrollBar()->value(); //- this->horizontalScrollBar()->value();
        diffMY = ref->verticalScrollBar()->value(); //- this->verticalScrollBar()->value();
        diffPX = (ref->alphaScrollArea()->geometry().left() - ref->imageLabel()->geometry().left()) -
                    (this->scrollArea->geometry().left() - this->label->geometry().left()) + ref->horizontalScrollBar()->value() - this->horizontalScrollBar()->value();
        diffPY = (ref->alphaScrollArea()->geometry().top() - ref->imageLabel()->geometry().top()) -
                    (this->scrollArea->geometry().top() - this->label->geometry().top()) + ref->verticalScrollBar()->value() - this->verticalScrollBar()->value();
        pic_x -= diffPX;
        pic_y -= diffPY;
        mouse_x -= diffMX;
        mouse_y -= diffMY;
        QRgb pixel = label->getPixel(pic_x, pic_y);
        passiveColor = new QColor(qRed(pixel), qGreen(pixel), qBlue(pixel), qAlpha(pixel));
        if(!passiveColor){
            passiveColor = new QColor(0,0,0,0);
        }
    }

    colorTag->showContent(passiveColor, mouse_x, mouse_y, pic_x, pic_y);
}

void DIPImageView::colorTagHide()
{
    colorTag->hideContent();
}

void DIPImageView::__colorTag(QColor *color, int mouse_x, int mouse_y, int pic_x, int pic_y)
{

    colorTag->showContent(color, mouse_x - horizontalScrollBar()->value(), mouse_y - verticalScrollBar()->value(), pic_x, pic_y);
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

QScrollBar *DIPImageView::verticalScrollBar()
{
    return scrollArea->verticalScrollBar();
}

QScrollBar *DIPImageView::horizontalScrollBar()
{
    return scrollArea->horizontalScrollBar();
}

DIPAlphaScrollArea *DIPImageView::alphaScrollArea()
{
    return scrollArea;
}

DIPImageLabel *DIPImageView::imageLabel()
{
    return label;
}

bool DIPImageView::isColorTagPassive()
{
    return colorTag->isPassive();
}

void DIPImageView::setColorTagPassive(bool value)
{
    colorTag->setPassive(value);
}

