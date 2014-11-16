#include "dipmoduleag.h"

DIPModuleAG::DIPModuleAG(DIPImageView *source, DIPImageView *result, QWidget *parent)
    :DIPModuleBase(source, result, parent)
{
    initUI();
    moduleName = new QString(tr("Algebraic & Geometric Op."));
}

void DIPModuleAG::initUI()
{
    DIPModuleBase::initUI();

    algGB = new QGroupBox(tr("Algebraic Operations"), parent);
    geoGB = new QGroupBox(tr("Geometry Operations"), parent);
    algGBL = new QGridLayout(algGB);
    geoGBL = new QGridLayout(geoGB);
    opView = new DIPImageView(parent);
    addBt = new QPushButton(tr("+") ,algGB);
    minusBt = new QPushButton(tr("-") ,algGB);
    multiplyBt = new QPushButton(tr("×") ,algGB);
    divideBt = new QPushButton(tr("÷") ,algGB);
    opFileMenu = new QMenu(tr("&File"), opView);
    openOpAct  = new QAction(QIcon(":/resource/icon/blue-open.png"), tr("&Open a image"), opView);
    grayOpAct  = new QAction(QIcon(":/resource/icon/gray-scale.png"), tr("&Gray scale mode"), opView);
    histoROpAct = new QAction(QIcon(":/resource/icon/histo.png"), tr("&Red Channel"), opView);
    histoGOpAct = new QAction(QIcon(":/resource/icon/histo.png"), tr("&Green Channel"), opView);
    histoBOpAct = new QAction(QIcon(":/resource/icon/histo.png"), tr("&Blue Channel"), opView);
    histoAOpAct = new QAction(QIcon(":/resource/icon/histo.png"), tr("&Alpha Channel"), opView);
    histoSOpAct = new QAction(QIcon(":/resource/icon/histo.png"), tr("Gray &Scale Channel"), opView);
    nearRB = new QRadioButton(tr("Nearest Neighbour Interpolation"),geoGB);
    biliRB = new QRadioButton(tr("Bilinear Interpolation"),geoGB);
    rotateSd = new QSlider(Qt::Horizontal, geoGB);
    scaleSd = new QSlider(Qt::Horizontal, geoGB);
    rotateL = new QLabel(tr("Rotate (degree):"), geoGB);
    scaleL = new QLabel(tr("Scale:"), geoGB);

    addBt->setMaximumSize(40, 30);
    minusBt->setMaximumSize(40, 30);
    divideBt->setMaximumSize(40, 30);
    multiplyBt->setMaximumSize(40, 30);
    addBt->setFont(QFont(tr("Microsoft YaHei"),15));
    minusBt->setFont(QFont(tr("Microsoft YaHei"),15));
    divideBt->setFont(QFont(tr("Microsoft YaHei"),15));
    multiplyBt->setFont(QFont(tr("Microsoft YaHei"),15));
    nearRB->setChecked(true);
    biliRB->setContentsMargins(0,0,0,10);
    rotateSd->setTickInterval(1);
    rotateSd->setTickPosition(QSlider::TicksBelow);
    rotateSd->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    rotateSd->setMaximum(360);
    rotateSd->setMinimum(0);
    scaleSd->setTickInterval(1);
    scaleSd->setTickPosition(QSlider::TicksBelow);
    scaleSd->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    scaleSd->setMaximum(100);
    scaleSd->setMinimum(-100);
    connect(addBt, SIGNAL(clicked()), this, SLOT(__doAlgebraic()));
    connect(minusBt, SIGNAL(clicked()), this, SLOT(__doAlgebraic()));
    connect(multiplyBt, SIGNAL(clicked()), this, SLOT(__doAlgebraic()));
    connect(divideBt, SIGNAL(clicked()), this, SLOT(__doAlgebraic()));
    connect(scaleSd, SIGNAL(sliderReleased()), this, SLOT(__doScale()));
    connect(rotateSd, SIGNAL(valueChanged(int)), this, SLOT(__doRotate()));

    grayOpAct->setCheckable(true);
    grayOpAct->setChecked(false);
    histoROpAct->setCheckable(true);
    histoGOpAct->setCheckable(true);
    histoBOpAct->setCheckable(true);
    histoAOpAct->setCheckable(true);
    histoSOpAct->setCheckable(true);
    histoROpAct->setChecked(false);
    histoGOpAct->setChecked(false);
    histoBOpAct->setChecked(false);
    histoAOpAct->setChecked(false);
    histoSOpAct->setChecked(false);
    opFileMenu->addAction(openOpAct);
    opFileMenu->addAction(grayOpAct);
    opFileMenu->addSeparator();
    opFileMenu->addAction(histoROpAct);
    opFileMenu->addAction(histoGOpAct);
    opFileMenu->addAction(histoBOpAct);
    opFileMenu->addAction(histoAOpAct);
    opFileMenu->addAction(histoSOpAct);
    opView->getMenuBar()->addMenu(opFileMenu);
    connect(openOpAct, SIGNAL(triggered()), opView, SLOT(loadImageWithDialog()));
    connect(grayOpAct, SIGNAL(triggered(bool)), opView, SLOT(setGrayMode(bool)));
    connect(histoROpAct, SIGNAL(triggered()), this, SLOT(__displayHistogram()));
    connect(histoGOpAct, SIGNAL(triggered()), this, SLOT(__displayHistogram()));
    connect(histoBOpAct, SIGNAL(triggered()), this, SLOT(__displayHistogram()));
    connect(histoAOpAct, SIGNAL(triggered()), this, SLOT(__displayHistogram()));
    connect(histoSOpAct, SIGNAL(triggered()), this, SLOT(__displayHistogram()));
    connect(opView, SIGNAL(_imageSetted()), this, SLOT(__displayHistogram()));

    algGB->setLayout(algGBL);
    algGBL->addWidget(opView,0,0,1,4);
    algGBL->addWidget(addBt,1,0,1,1,Qt::AlignCenter);
    algGBL->addWidget(minusBt,1,1,1,1,Qt::AlignCenter);
    algGBL->addWidget(multiplyBt,1,2,1,1,Qt::AlignCenter);
    algGBL->addWidget(divideBt,1,3,1,1,Qt::AlignCenter);
    geoGB->setLayout(geoGBL);
    geoGBL->addWidget(nearRB,0,0,1,1);
    geoGBL->addWidget(biliRB,1,0,1,1);
    geoGBL->addWidget(rotateL,2,0,1,1,Qt::AlignLeft);
    geoGBL->addWidget(rotateSd,3,0,1,1);
    geoGBL->addWidget(scaleL,4,0,1,1,Qt::AlignLeft);
    geoGBL->addWidget(scaleSd,5,0,1,1);

    mainLt = new QGridLayout(DIPModuleBase::getUI());

    mainLt->addWidget(algGB,0,0,1,1);
    mainLt->addWidget(geoGB,1,0,1,1);

    DIPModuleBase::getUI()->setLayout(mainLt);
}

int DIPModuleAG::dipRed(QRgb rgb)
{
    if(qAlpha(rgb) == 0){
        return 0;
    }else{
        return qRed(rgb);
    }
}

int DIPModuleAG::dipBlue(QRgb rgb)
{
    if(qAlpha(rgb) == 0){
        return 0;
    }else{
        return qBlue(rgb);
    }
}

int DIPModuleAG::dipGreen(QRgb rgb)
{
    if(qAlpha(rgb) == 0){
        return 0;
    }else{
        return qGreen(rgb);
    }
}

double DIPModuleAG::st(int v)
{
    double vt;
    if(v < 0){
        //vt = double(9) / (100 * 100 * 10) * (v + 100) * (v + 100) + 0.1;
        vt = double(4) / (100 * 100 * 5) * (v + 100) * (v + 100) + 0.2;
    }else{
        //vt = -1 * double(9) / (100 * 100) * (v - 100) * (v - 100) + 10;
        vt = -1 * double(4) / (100 * 100) * (v - 100) * (v - 100) + 5;
    }
    return vt;
}

double DIPModuleAG::bilinear(double x, double y, int x1, int x2, int y1, int y2,int f11, int f12, int f21, int f22)
{
    if(x2 == x1) qDebug()<<"fail";
    return (f11 * (x2 -x) * (y2 - y) +
            f21 * (x -x1) * (y2 - y) +
            f12 * (x2 -x) * (y - y1) +
            f22 * (x -x1) * (y - y1)) /
            (x2 - x1) / (y2 -y1);
}

void DIPModuleAG::__displayHistogram()
{
    int channelO = 0;

    if(histoROpAct->isChecked()){
        channelO |= DIPImageView::CHANNEL_R;
    }
    if(histoGOpAct->isChecked()){
        channelO |= DIPImageView::CHANNEL_G;
    }
    if(histoBOpAct->isChecked()){
        channelO |= DIPImageView::CHANNEL_B;
    }
    if(histoAOpAct->isChecked()){
        channelO |= DIPImageView::CHANNEL_A;
    }
    if(histoSOpAct->isChecked()){
        channelO |= DIPImageView::CHANNEL_S;
    }
    if(channelO){
        opView->displayHistogram(channelO);
    }else{
        opView->hideHistogram();
    }
}

void DIPModuleAG::__doAlgebraic()
{
    if(!sourceView->isImageLoaded()){
        emit _console(tr("ERROR: Source image is not loaded"));
        return;
    }
    if(!opView->isImageLoaded()){
        emit _console(tr("ERROR: Oprand image is not loaded"));
        return;
    }

    if(QObject::sender() == NULL){
        return;
    }

    QImage *source = sourceView->getImage();
    QImage *oprand = opView->getImage();
    int w = source->width();
    int h = source->height();
    int opw = oprand->width();
    int oph = oprand->height();
    QImage *result = new QImage(w,h,QImage::Format_ARGB32);

    for(int i = 0;i < h; i++){
        QRgb* row = (QRgb*)source->scanLine(i);
        QRgb* rowResult = (QRgb*)result->scanLine(i);
        QRgb* rowOprand = NULL;
        if(i <= oph) rowOprand = (QRgb*)oprand->scanLine(i);
        for(int j = 0; j < w; j++){
             if(i > oph || j > opw){
                 rowResult[j] = row[j];
                 continue;
             }else if(rowOprand){
                 int r,g,b,a = 0;
                 if(sender() == addBt){
                    r = dipRed(row[j]) + dipRed(rowOprand[j]);
                    g = dipGreen(row[j]) + dipGreen(rowOprand[j]);
                    b = dipBlue(row[j]) + dipBlue(rowOprand[j]);
                    a = qAlpha(row[j]) + qAlpha(rowOprand[j]);
                 }
                 if(sender() == minusBt){
                    r = dipRed(row[j]) - dipRed(rowOprand[j]);
                    g = dipGreen(row[j]) - dipGreen(rowOprand[j]);
                    b = dipBlue(row[j]) - dipBlue(rowOprand[j]);
                    if(qAlpha(row[j]) != qAlpha(rowOprand[j]))
                        a = qAlpha(row[j]) - qAlpha(rowOprand[j]);
                    else
                        a = qAlpha(row[j]);
                 }
                 if(sender() == multiplyBt){
                    r = dipRed(row[j]) * dipRed(rowOprand[j]);
                    g = dipGreen(row[j]) * dipGreen(rowOprand[j]);
                    b = dipBlue(row[j]) * dipBlue(rowOprand[j]);
                    a = qAlpha(row[j]) * qAlpha(rowOprand[j]);
                 }
                 if(sender() == divideBt){
                    (dipRed(rowOprand[j]) == 0) ? (r = 255) : (r = 255 * qRound(double(dipRed(row[j])) / dipRed(rowOprand[j])));
                    (dipGreen(rowOprand[j]) == 0) ? (g = 255) : (g = 255 * qRound(double(dipGreen(row[j])) / dipGreen(rowOprand[j])));
                    (dipBlue(rowOprand[j]) == 0) ? (b = 255) : (b = 255 * qRound(double(dipBlue(row[j])) / dipBlue(rowOprand[j])));
                    if(qAlpha(row[j]) != qAlpha(rowOprand[j]))
                        (qAlpha(rowOprand[j]) == 0) ? (a = 255) : (a = 255 * qRound(double(qAlpha(row[j])) / qAlpha(rowOprand[j])));
                    else
                        a = qAlpha(row[j]);
                 }
                 if(r > 255) r = 255;
                 if(g > 255) g = 255;
                 if(b > 255) b = 255;
                 if(a > 255) a = 255;
                 if(r < 0) r = 0;
                 if(g < 0) g = 0;
                 if(b < 0) b = 0;
                 if(a < 0) a = 0;
                 rowResult[j] = qRgba(r,g,b,a);
             }
        }
    }

    emit _resultImage(result, resultView);
    emit _console(QString(tr("Algebraic operation \"%1\" is done")).arg(qobject_cast<QPushButton *>(sender())->text()));

}

void DIPModuleAG::__doRotate()
{
    if(!sourceView->isImageLoaded()){
        emit _console(tr("ERROR: Source image is not loaded"));
        return;
    }

    double theta = rotateSd->value() / double(180) * M_PI;
    QImage *source = sourceView->getImage();
    int w = source->width();
    int h = source->height();
    int rh = qCeil(h * qFabs(qCos(theta)) + w * qFabs(qSin(theta)));
    int rw = qCeil(w * qFabs(qCos(theta)) + h * qFabs(qSin(theta)));
    double ox, oy;
    if(rotateSd->value() >=0 && rotateSd->value() <90){
        ox = h * qSin(theta);
        oy = 0;
    }else if(rotateSd->value() >=90 && rotateSd->value() <180){
        ox = rw;
        oy = h * -1 * qCos(theta);
    }else if(rotateSd->value() >=180 && rotateSd->value() <270){
        ox = w * -1 * qCos(theta);
        oy = rh;
    }else{
        ox = 0;
        oy = w * -1 * qSin(theta);
    }

    QImage *result = new QImage(rw,rh,QImage::Format_ARGB32);
    for(int i = 0;i < rh; i++){
        QRgb* rowResult = (QRgb*)result->scanLine(i);
        for(int j = 0; j < rw; j++){
            double sx = qCos(theta) * (j - ox) + qSin(theta) * (i - oy);
            double sy = -1 * qSin(theta) * (j - ox) + qCos(theta) * (i - oy);
            if(nearRB->isChecked()){
                int x,y;
                if(sx - qFloor(sx) >= qCeil(sx) - sx){
                    x = qCeil(sx);
                }else{
                    x = qFloor(sx);
                }
                if(sy - qFloor(sy) >= qCeil(sy) - sy){
                    y = qCeil(sy);
                }else{
                    y = qFloor(sy);
                }
                QRgb v;
                if(x >= w || y >= h || x < 0 || y < 0){
                    v = qRgba(0,0,0,0);
                }else{
                    v = ((QRgb*)source->scanLine(y))[x];
                }
                rowResult[j] = qRgba(qRed(v), qGreen(v), qBlue(v), qAlpha(v));
            }else if(biliRB->isChecked()){
                int x1,y1,x2,y2;
                x1 = qFloor(sx);
                x2 = qCeil(sx);
                y1 = qFloor(sy);
                y2 = qCeil(sy);
                if(x1 == x2){
                    x1 -= 1;
                }
                if(y1 == y2){
                    y1 -= 1;
                }
                if(((x1 >=w || x1 < 0) || (y1 >=h || y1 < 0)) &&
                   ((x2 >=w || x2 < 0) || (y1 >=h || y1 < 0)) &&
                   ((x1 >=w || x1 < 0) || (y2 >=h || y2 < 0)) &&
                   ((x2 >=w || x2 < 0) || (y2 >=h || y2 < 0))){
                    rowResult[j] = qRgba(0,0,0,0);
                    continue;
                }
                QRgb Q11,Q12,Q21,Q22;
                if(y1 < 0 || y1 >= h){
                    Q11 = qRgba(0,0,0,0);
                    Q21 = qRgba(0,0,0,0);
                }else{
                    if(x1 >= w){
                        Q11 = qRgba(0,0,0,0);
                    }else if(x2 >= w){
                        Q21 = qRgba(0,0,0,0);
                    }else{
                        QRgb * v1 = (QRgb*)source->scanLine(y1);
                        Q11 = v1[x1];
                        Q21 = v1[x2];
                    }
                }
                if(y2 < 0 || y2 >= h){
                    Q12 = qRgba(0,0,0,0);
                    Q22 = qRgba(0,0,0,0);
                }else{
                    if(x1 >= w){
                        Q12 = qRgba(0,0,0,0);
                    }else if(x2 >= w){
                        Q22 = qRgba(0,0,0,0);
                    }else{
                        QRgb * v1 = (QRgb*)source->scanLine(y2);
                        Q12 = v1[x1];
                        Q22 = v1[x2];
                    }
                }
                int r = qRound(bilinear(sx,sy,x1,x2,y1,y2,qRed(Q11),qRed(Q12),qRed(Q21),qRed(Q22)));
                int g = qRound(bilinear(sx,sy,x1,x2,y1,y2,qGreen(Q11),qGreen(Q12),qGreen(Q21),qGreen(Q22)));
                int b = qRound(bilinear(sx,sy,x1,x2,y1,y2,qBlue(Q11),qBlue(Q12),qBlue(Q21),qBlue(Q22)));
                int a = qRound(bilinear(sx,sy,x1,x2,y1,y2,qAlpha(Q11),qAlpha(Q12),qAlpha(Q21),qAlpha(Q22)));
                rowResult[j] = qRgba(r,g,b,a);
            }
            //int v = qGray(qRed(row[j]),qGreen(row[j]), qBlue(row[j]));
        }
    }
    QString opname;
    if(biliRB->isChecked()){
        opname = tr("Bilinear interpolation");
    }else{
        opname = tr("Nearest neighbour interpolation");
    }
    emit _resultImage(result, resultView);
    rotateL->setText(QString("Rotate (degree): %1").arg(rotateSd->value()));
    emit _console(QString(tr("Geometry operation \"Rotate\" is done, rotate at %1 degree, using \"%2\"").arg(rotateSd->value()).arg(opname)));

}

void DIPModuleAG::__doScale()
{
    if(!sourceView->isImageLoaded()){
        emit _console(tr("ERROR: Source image is not loaded"));
        return;
    }

    double rate = st(scaleSd->value());
    QImage *source = sourceView->getImage();
    int w = source->width();
    int h = source->height();
    int rw = qCeil(w * rate);
    int rh = qCeil(h * rate);

    QImage *result = new QImage(rw,rh,QImage::Format_ARGB32); //todo: size to big?
    for(int i = 0;i < rh; i++){
        QRgb* rowResult = (QRgb*)result->scanLine(i);
        for(int j = 0; j < rw; j++){
            double sx = j / rate;
            double sy = i / rate;
            if(nearRB->isChecked()){
                int x,y;
                if(sx - qFloor(sx) >= qCeil(sx) - sx){
                    x = qCeil(sx);
                }else{
                    x = qFloor(sx);
                }
                if(sy - qFloor(sy) >= qCeil(sy) - sy){
                    y = qCeil(sy);
                }else{
                    y = qFloor(sy);
                }
                if(x >= w) x = w - 1;
                if(y >= h) y = h -1;
                if(x < 0) x = 0;
                if(y < 0) y = 0;
                QRgb v = ((QRgb*)source->scanLine(y))[x];
                rowResult[j] = qRgba(qRed(v), qGreen(v), qBlue(v), qAlpha(v));
            }else if(biliRB->isChecked()){
                int x1,y1,x2,y2;
                x1 = qFloor(sx);
                x2 = qCeil(sx);
                y1 = qFloor(sy);
                y2 = qCeil(sy);
                if(x1 == x2){
                    x1 -= 1;
                }
                if(y1 == y2){
                    y1 -= 1;
                }
                if(((x1 >=w || x1 < 0) || (y1 >=h || y1 < 0)) &&
                   ((x2 >=w || x2 < 0) || (y1 >=h || y1 < 0)) &&
                   ((x1 >=w || x1 < 0) || (y2 >=h || y2 < 0)) &&
                   ((x2 >=w || x2 < 0) || (y2 >=h || y2 < 0))){
                    rowResult[j] = qRgba(0,0,0,0);
                    continue;
                }
                QRgb Q11,Q12,Q21,Q22;
                if(y1 < 0 || y1 >= h){
                    Q11 = qRgba(0,0,0,0);
                    Q21 = qRgba(0,0,0,0);
                }else{
                    if(x1 >= w){
                        Q11 = qRgba(0,0,0,0);
                    }else if(x2 >= w){
                        Q21 = qRgba(0,0,0,0);
                    }else{
                        QRgb * v1 = (QRgb*)source->scanLine(y1);
                        Q11 = v1[x1];
                        Q21 = v1[x2];
                    }
                }
                if(y2 < 0 || y2 >= h){
                    Q12 = qRgba(0,0,0,0);
                    Q22 = qRgba(0,0,0,0);
                }else{
                    if(x1 >= w){
                        Q12 = qRgba(0,0,0,0);
                    }else if(x2 >= w){
                        Q22 = qRgba(0,0,0,0);
                    }else{
                        QRgb * v1 = (QRgb*)source->scanLine(y2);
                        Q12 = v1[x1];
                        Q22 = v1[x2];
                    }
                }
                int r = qRound(bilinear(sx,sy,x1,x2,y1,y2,qRed(Q11),qRed(Q12),qRed(Q21),qRed(Q22)));
                int g = qRound(bilinear(sx,sy,x1,x2,y1,y2,qGreen(Q11),qGreen(Q12),qGreen(Q21),qGreen(Q22)));
                int b = qRound(bilinear(sx,sy,x1,x2,y1,y2,qBlue(Q11),qBlue(Q12),qBlue(Q21),qBlue(Q22)));
                int a = qRound(bilinear(sx,sy,x1,x2,y1,y2,qAlpha(Q11),qAlpha(Q12),qAlpha(Q21),qAlpha(Q22)));
                rowResult[j] = qRgba(r,g,b,a);
            }
        }
    }

    QString opname;
    if(biliRB->isChecked()){
        opname = tr("Bilinear interpolation");
    }else{
        opname = tr("Nearest neighbour interpolation");
    }


    emit _resultImage(result, resultView);
    scaleL->setText(QString("Scale: %1x").arg(st(scaleSd->value())));
    emit _console(QString(tr("Geometry operation \"Scale\" is done, scale to %1x, using \"%2\"").arg(st(scaleSd->value())).arg(opname)));
}


