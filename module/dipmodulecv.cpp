#include "dipmodulecv.h"

DIPModuleCV::DIPModuleCV(DIPImageView *source, DIPImageView *result, QWidget *parent)
    :DIPModuleBase(source, result, parent)
{
    initUI();
    moduleName = new QString(tr("Convolution & Filters"));
    custK = NULL;
}

void DIPModuleCV::initUI()
{
    DIPModuleBase::initUI();

    edgeGB = new QGroupBox(tr("Edge detection"), parent);
    edgeGBL = new QGridLayout(edgeGB);
    robertsBt = new QPushButton(tr("Roberts"), edgeGB);
    sobelBt = new QPushButton(tr("Sobel"), edgeGB);
    prewittBt = new QPushButton(tr("Prewitt"), edgeGB);
    edgeDirL = new QLabel(tr("Direction:"), edgeGB);
    edgeXRB = new QRadioButton(tr("X"), edgeGB);
    edgeYRB = new QRadioButton(tr("Y"), edgeGB);
    edgeCRB = new QRadioButton(tr("Combined"), edgeGB);
    filterGB = new QGroupBox(tr("Filters"), parent);
    filterGBL = new QGridLayout(filterGB);
    meanRB = new QRadioButton(tr("Mean"), filterGB);
    mediRB = new QRadioButton(tr("Median"), filterGB);
    gausRB = new QRadioButton(tr("Gaussian"), filterGB);
    custRB = new QRadioButton(tr("Customize"), filterGB);
    sizeSd = new QSlider(Qt::Horizontal, filterGB);
    sigmaSd = new QSlider(Qt::Horizontal, filterGB);
    sizeLb = new QLabel(tr("Kernel Size:"),filterGB);
    sigmaLb = new QLabel(tr("Sigma:"),filterGB);
    doFilterBt = new QPushButton(tr("Apply Filter"), filterGB);
    table = new QTableView(filterGB);

    edgeGBL->addWidget(edgeDirL,0,0,1,1);
    edgeGBL->addWidget(edgeXRB,1,0,1,1);
    edgeGBL->addWidget(edgeYRB,1,1,1,1);
    edgeGBL->addWidget(edgeCRB,1,2,1,1);
    edgeGBL->addWidget(robertsBt,2,0,1,1);
    edgeGBL->addWidget(sobelBt,2,1,1,1);
    edgeGBL->addWidget(prewittBt,2,2,1,1);
    edgeXRB->setChecked(true);
    //edgeGB->setSizePolicy(QSizePolicy(QSizePolicy::Expanding,QSizePolicy::Fixed));

    filterGBL->addWidget(meanRB,0,0,1,1);
    filterGBL->addWidget(mediRB,0,1,1,1);
    filterGBL->addWidget(gausRB,1,0,1,1);
    filterGBL->addWidget(custRB,1,1,1,1);
    filterGBL->addWidget(sizeLb,2,0,1,2);
    filterGBL->addWidget(sizeSd,3,0,1,2);
    filterGBL->addWidget(sigmaLb,4,0,1,2);
    filterGBL->addWidget(sigmaSd,5,0,1,2);
    filterGBL->addWidget(doFilterBt,6,0,1,2);
    filterGBL->addWidget(table,5,0,1,2);
    table->setVisible(false);
    sigmaLb->setVisible(false);
    sigmaSd->setVisible(false);
    meanRB->setChecked(true);
    sizeSd->setMaximum(5);
    sizeSd->setMinimum(1);
    sizeSd->setValue(1);
    sizeSd->setPageStep(1);
    sizeSd->setTickPosition(QSlider::TicksBelow);
    sigmaSd->setMaximum(100);
    sigmaSd->setMinimum(-100);
    //sigmaSd->setPageStep(2);
    sigmaSd->setTickPosition(QSlider::TicksBelow);
    //filterGB->setSizePolicy(QSizePolicy(QSizePolicy::Maximum,QSizePolicy::Maximum));

    mainLt = new QGridLayout(DIPModuleBase::getUI());
    mainLt->addWidget(edgeGB,0,0,1,1);
    mainLt->addWidget(filterGB,1,0,1,1);
    //mainLt->setSizeConstraint(QLayout::SetFixedSize);
    //filterGBL->setSizeConstraint(QLayout::SetFixedSize);
    //edgeGBL->setSizeConstraint(QLayout::SetFixedSize);

    connect(robertsBt, SIGNAL(clicked()), this, SLOT(__doEdgeDete()));
    connect(sobelBt, SIGNAL(clicked()), this, SLOT(__doEdgeDete()));
    connect(prewittBt, SIGNAL(clicked()), this, SLOT(__doEdgeDete()));
    connect(meanRB, SIGNAL(toggled(bool)), this, SLOT(switchFilter(bool)));
    connect(mediRB, SIGNAL(toggled(bool)), this, SLOT(switchFilter(bool)));
    connect(gausRB, SIGNAL(toggled(bool)), this, SLOT(switchFilter(bool)));
    connect(custRB, SIGNAL(toggled(bool)), this, SLOT(switchFilter(bool)));
    connect(sizeSd, SIGNAL(valueChanged(int)), this, SLOT(calSdLb(int)));
    connect(sigmaSd, SIGNAL(valueChanged(int)), this, SLOT(calSdLb(int)));
    connect(doFilterBt, SIGNAL(clicked()), this, SLOT(__doFilter()));
    connect(sizeSd, SIGNAL(valueChanged(int)), this, SLOT(updateCustTable(int)));

    DIPModuleBase::getUI()->setLayout(mainLt);

    calSdLb();
    //DIPModuleBase::getUI()->setSizePolicy(QSizePolicy(QSizePolicy::Expanding,QSizePolicy::Fixed));
    //QMouseEvent *event = new QMouseEvent(QEvent::MouseButtonPress, )

}

int DIPModuleCV::_cz(int value)
{
    return value * 2 + 1;
}

double DIPModuleCV::_cg(int value)
{
    if(value <= 0){
        return 0.9 / 10000 * (value + 100) * (value + 100) + 0.1;
    }else{
        return -9.0 / 10000 * (value - 100) * (value - 100) + 10;
    }
}


void DIPModuleCV::__doEdgeDete()
{
    if(!sourceView->isImageLoaded()){
        emit _console(tr("ERROR: Source image is not loaded"));
        return;
    }

    QString opName;
    QString opDir;

    if(!sender()){
        return;
    }

    if(sender() == robertsBt){
        opName = tr("ROBERTS");
    }else if(sender() == prewittBt){
        opName = tr("PREWITT");
    }else if(sender() == sobelBt){
         opName = tr("SOBEL");
    }
    if(edgeXRB->isChecked()){
        opDir = tr("X");
    }else if(edgeYRB->isChecked()){
        opDir = tr("Y");
    }else if(edgeCRB->isChecked()){
        opDir = tr("Combine");
    }

    QImage *source = sourceView->getImage();
    int w = source->width();
    int h = source->height();

    QImage *result = new QImage(w,h,QImage::Format_ARGB32); //todo: size to big?
    for(int i = 0;i < h; i++){
        QRgb* rowResult = (QRgb*)result->scanLine(i);
        for(int j = 0; j < w; j++){
            QRgb *rowS2;
            QRgb *rowS0;
            QRgb *rowS1;
            if(opName == tr("ROBERTS")){
                if(edgeXRB->isChecked()){
                    rowS1 = (QRgb*)source->scanLine(i);
                    if(i+1 >= h) {
                        rowS2 = rowS1;
                    }else{
                        rowS2 = (QRgb*)source->scanLine(i+1);
                    }
                    QRgb f00, f11;
                    f00 = rowS1[j];
                    if(j+1 >= w){
                        f11 = rowS2[j];
                    }else{
                        f11 = rowS2[j+1];
                    }
                    int vR = qFabs(qRed(f11) - qRed(f00));
                    int vG = qFabs(qGreen(f11) - qGreen(f00));
                    int vB = qFabs(qBlue(f11) - qBlue(f00));
                    rowResult[j] = qRgb(vR, vG, vB);
                }else if(edgeYRB->isChecked()){
                    rowS1 = (QRgb*)source->scanLine(i);
                    if(i+1 >= h) {
                        rowS2 = rowS1;
                    }else{
                        rowS2 = (QRgb*)source->scanLine(i+1);
                    }
                    QRgb f10, f01;
                    f01 = rowS2[j];
                    if(j+1 >= w){
                        f10 = rowS1[j];
                    }else{
                        f10 = rowS1[j+1];
                    }
                    int vR = qFabs(qRed(f10) - qRed(f01));
                    int vG = qFabs(qGreen(f10) - qGreen(f01));
                    int vB = qFabs(qBlue(f10) - qBlue(f01));
                    rowResult[j] = qRgb(vR, vG, vB);
                }else{
                    rowS1 = (QRgb*)source->scanLine(i);
                    if(i+1 >= h) {
                        rowS2 = rowS1;
                    }else{
                        rowS2 = (QRgb*)source->scanLine(i+1);
                    }
                    QRgb f00, f10, f01, f11;
                    f01 = rowS2[j];
                    f00 = rowS1[j];
                    if(j+1 >= w){
                        f11 = rowS2[j];
                        f10 = rowS1[j];
                    }else{
                        f11 = rowS2[j+1];
                        f10 = rowS1[j+1];
                    }
                    int xR = qFabs(qRed(f11) - qRed(f00));
                    int xG = qFabs(qGreen(f11) - qGreen(f00));
                    int xB = qFabs(qBlue(f11) - qBlue(f00));
                    int yR = qFabs(qRed(f10) - qRed(f01));
                    int yG = qFabs(qGreen(f10) - qGreen(f01));
                    int yB = qFabs(qBlue(f10) - qBlue(f01));
                    int gR = qSqrt(xR*xR + yR*yR);
                    int gG = qSqrt(xG*xG + yG*yG);
                    int gB = qSqrt(xB*xB + yB*yB);
                    rowResult[j] = qRgb(gR, gG, gB);
                }
            }else if(opName == "PREWITT" || opName == "SOBEL"){
                int t;
                if(opName == "SOBEL"){
                    t = 2;
                }else{
                    t = 1;
                }
                if(edgeXRB->isChecked()){
                    //rowS1 = (QRgb*)source->scanLine(i);
                    if(i+1 >= h) {
                        rowS2 = (QRgb*)source->scanLine(i);
                    }else{
                        rowS2 = (QRgb*)source->scanLine(i+1);
                    }
                    if(i-1 < 0) {
                        rowS0 = (QRgb*)source->scanLine(i);
                    }else{
                        rowS0 = (QRgb*)source->scanLine(i-1);
                    }
                    QRgb f00, f10, f20;
                    QRgb f02, f12, f22;
                    f10 = rowS0[j];
                    f12 = rowS2[j];
                    if(j+1 >= w){
                        f20 = rowS0[j];
                        f22 = rowS2[j];
                    }else{
                        f20 = rowS0[j+1];
                        f22 = rowS2[j+1];
                    }
                    if(j-1 < 0){
                        f00 = rowS0[j];
                        f02 = rowS2[j];
                    }else{
                        f00 = rowS0[j-1];
                        f02 = rowS2[j-1];
                    }

                    int vR = qFabs(qRed(f00)+t*qRed(f10)+qRed(f20)-qRed(f02)-t*qRed(f12)-qRed(f22));
                    int vG = qFabs(qGreen(f00)+t*qGreen(f10)+qGreen(f20)-qGreen(f02)-t*qGreen(f12)-qGreen(f22));
                    int vB = qFabs(qBlue(f00)+t*qBlue(f10)+qBlue(f20)-qBlue(f02)-t*qBlue(f12)-qBlue(f22));
                    rowResult[j] = qRgb(vR, vG, vB);
                }else if(edgeYRB->isChecked()){
                    rowS1 = (QRgb*)source->scanLine(i);
                    if(i+1 >= h) {
                        rowS2 = (QRgb*)source->scanLine(i);
                    }else{
                        rowS2 = (QRgb*)source->scanLine(i+1);
                    }
                    if(i-1 < 0) {
                        rowS0 = (QRgb*)source->scanLine(i);
                    }else{
                        rowS0 = (QRgb*)source->scanLine(i-1);
                    }
                    QRgb f00, f20;
                    QRgb f01, f21;
                    QRgb f02, f22;
                    if(j+1 >= w){
                        f20 = rowS0[j];
                        f21 = rowS1[j];
                        f22 = rowS2[j];
                    }else{
                        f20 = rowS0[j+1];
                        f21 = rowS1[j+1];
                        f22 = rowS2[j+1];
                    }
                    if(j-1 < 0){
                        f00 = rowS0[j];
                        f01 = rowS1[j];
                        f02 = rowS2[j];
                    }else{
                        f00 = rowS0[j-1];
                        f01 = rowS1[j-1];
                        f02 = rowS2[j-1];
                    }
                    int vR = qFabs(qRed(f00)+t*qRed(f01)+qRed(f02)-qRed(f20)-t*qRed(f21)-qRed(f22));
                    int vG = qFabs(qGreen(f00)+t*qGreen(f01)+qGreen(f02)-qGreen(f20)-t*qGreen(f21)-qGreen(f22));
                    int vB = qFabs(qBlue(f00)+t*qBlue(f01)+qBlue(f02)-qBlue(f20)-t*qBlue(f21)-qBlue(f22));
                    rowResult[j] = qRgb(vR, vG, vB);
                }else{
                    rowS1 = (QRgb*)source->scanLine(i);
                    if(i+1 >= h) {
                        rowS2 = (QRgb*)source->scanLine(i);
                    }else{
                        rowS2 = (QRgb*)source->scanLine(i+1);
                    }
                    if(i-1 < 0) {
                        rowS0 = (QRgb*)source->scanLine(i);
                    }else{
                        rowS0 = (QRgb*)source->scanLine(i-1);
                    }
                    QRgb f00, f10, f20;
                    QRgb f01,      f21;
                    QRgb f02, f12, f22;
                    f10 = rowS0[j];
                    f12 = rowS2[j];
                    if(j+1 >= w){
                        f20 = rowS0[j];
                        f21 = rowS1[j];
                        f22 = rowS2[j];
                    }else{
                        f20 = rowS0[j+1];
                        f21 = rowS1[j+1];
                        f22 = rowS2[j+1];
                    }
                    if(j-1 < 0){
                        f00 = rowS0[j];
                        f01 = rowS1[j];
                        f02 = rowS2[j];
                    }else{
                        f00 = rowS0[j-1];
                        f01 = rowS1[j-1];
                        f02 = rowS2[j-1];
                    }
                    int xR = qFabs(qRed(f00)+t*qRed(f10)+qRed(f20)-qRed(f02)-t*qRed(f12)-qRed(f22));
                    int xG = qFabs(qGreen(f00)+t*qGreen(f10)+qGreen(f20)-qGreen(f02)-t*qGreen(f12)-qGreen(f22));
                    int xB = qFabs(qBlue(f00)+t*qBlue(f10)+qBlue(f20)-qBlue(f02)-t*qBlue(f12)-qBlue(f22));
                    int yR = qFabs(qRed(f00)+t*qRed(f01)+qRed(f02)-qRed(f20)-t*qRed(f21)-qRed(f22));
                    int yG = qFabs(qGreen(f00)+t*qGreen(f01)+qGreen(f02)-qGreen(f20)-t*qGreen(f21)-qGreen(f22));
                    int yB = qFabs(qBlue(f00)+t*qBlue(f01)+qBlue(f02)-qBlue(f20)-t*qBlue(f21)-qBlue(f22));
                    int gR = qSqrt(xR*xR + yR*yR);
                    int gG = qSqrt(xG*xG + yG*yG);
                    int gB = qSqrt(xB*xB + yB*yB);
                    rowResult[j] = qRgb(gR, gG, gB);
                }
            }
        }
    }
    emit _resultImage(result, resultView);
    emit _console(QString("Edge detection is done, using %1 operator, in direction %2.").arg(opName, opDir));
}

void DIPModuleCV::__doFilter()
{
    if(!sourceView->isImageLoaded()){
        emit _console(tr("ERROR: Source image is not loaded"));
        return;
    }

    QString opName;
    if(meanRB->isChecked()){
        opName = tr("Mean");
    }else if(mediRB->isChecked()){
        opName = tr("Median");
    }else if(gausRB->isChecked()){
        opName = tr("Gaussian");
    }else if(custRB->isChecked()){
        opName = tr("Customized");
    }

    QImage *source = sourceView->getImage();
    int w = source->width();
    int h = source->height();
    QImage *result = new QImage(w,h,QImage::Format_ARGB32);

    int size = _cz(sizeSd->value());
    QRgb **sRows = new QRgb*[size];
    QRgb *sVals = new QRgb[size * size];
    int *sortR = new int[size * size];
    int *sortG = new int[size * size];
    int *sortB = new int[size * size];
    //int *sortA = new int[size * size];

    for(int i = 0;i < h; i++){
        QRgb* rowResult = (QRgb*)result->scanLine(i);
        int yc = (size-1) / 2; //kernel center
        sRows[yc] = (QRgb*)source->scanLine(i);
        for(int k = 1; k <= (size-1) / 2; k++){
            if(i - k < 0){
                sRows[yc - k] = sRows[yc - k + 1];
            }else{
                sRows[yc - k] = (QRgb*)source->scanLine(i - k);
            }
            if(i + k >= h){
                sRows[yc + k] = sRows[yc + k - 1];
            }else{
                sRows[yc + k] = (QRgb*)source->scanLine(i + k);
            }
        }

        if(meanRB->isChecked()){
            for(int j = 0; j < w; j++){
                int xc = (size-1) / 2;
                for(int y = 0; y < size; y++){
                    sVals[xc + y*size] = sRows[y][j];
                }
                for(int k = 1;k <= (size-1) / 2; k++){
                    int x1 = xc - k, x2 = xc + k;
                    for(int y = 0; y < size; y++){
                        if(j - k < 0){
                            sVals[x1 + y * size] = sVals[x1 + 1 + y * size];
                        }else{
                            sVals[x1 + y * size] = sRows[y][j - k];
                        }
                        if(j + k >= w){
                            sVals[x2 + y * size] = sVals[x2 - 1 + y * size];
                        }else{
                            sVals[x2 + y * size] = sRows[y][j + k];
                        }
                    }
                }
                long vR = 0,vG = 0,vB = 0,vA = 0;
                for(int v = 0; v < size * size; v++){
                    vR += qRed(sVals[v]);
                    vB += qBlue(sVals[v]);
                    vG += qGreen(sVals[v]);
                    vA += qAlpha(sVals[v]);
                }
                vR = qRound(vR / (double)(size * size));
                vG = qRound(vG / (double)(size * size));
                vB = qRound(vB / (double)(size * size));
                vA = qRound(vA / (double)(size * size));
                rowResult[j] = qRgba(vR, vG, vB, vA);
            }
        }else if(mediRB->isChecked()){
            for(int j = 0; j < w; j++){
                int xc = (size-1) / 2;
                for(int y = 0; y < size; y++){
                    sVals[xc + y*size] = sRows[y][j];
                }
                for(int k = 1;k <= (size-1) / 2; k++){
                    int x1 = xc - k, x2 = xc + k;
                    for(int y = 0; y < size; y++){
                        if(j - k < 0){
                            sVals[x1 + y * size] = sVals[x1 + 1 + y * size];
                        }else{
                            sVals[x1 + y * size] = sRows[y][j - k];
                        }
                        if(j + k >= w){
                            sVals[x2 + y * size] = sVals[x2 - 1 + y * size];
                        }else{
                            sVals[x2 + y * size] = sRows[y][j + k];
                        }
                    }
                }
                //find size^2 / 2 th
                for(int s = 0; s < size * size; s++){
                    sortR[s] = qRed(sVals[s]);
                    sortG[s] = qGreen(sVals[s]);
                    sortB[s] = qBlue(sVals[s]);
                    //sortR[s] = qRed(sVals[s]);
                }
                for(int s = 0; s < size * size; s++){
                    for(int t = 0; t < s - 1; t++){
                        if(sortR[t] > sortR[t + 1]){
                            int temp = sortR[t];
                            sortR[t] = sortR[t + 1];
                            sortR[t + 1] = temp;
                        }
                    }
                    for(int t = 0; t < s - 1; t++){
                        if(sortG[t] > sortG[t + 1]){
                            int temp = sortG[t];
                            sortG[t] = sortG[t + 1];
                            sortG[t + 1] = temp;
                        }
                    }
                    for(int t = 0; t < s - 1; t++){
                        if(sortB[t] > sortB[t + 1]){
                            int temp = sortB[t];
                            sortB[t] = sortB[t + 1];
                            sortB[t + 1] = temp;
                        }
                    }
                }

//                QList<int> lR, lG, lB;
//                for(int s = 0; s < size * size; s++){
//                    lR<<qRed(sVals[s]);
//                    lG<<qGreen(sVals[s]);
//                    lB<<qBlue(sVals[s]);
//                    //sortR[s] = qRed(sVals[s]);
//                }
//                qSort(lR.begin(), lR.end());
//                qSort(lG.begin(), lG.end());
//                qSort(lB.begin(), lB.end());


//                if(i == 30 && j == 60){
//                    false;
//                    for(int t =0; t < size*size; t++){
//                        qDebug()<<sortR[t];
//                    }

//                }
                //rowResult[j] = ((QRgb*)source->scanLine(i))[j];
                //rowResult[j] = qRgb(lR.at(size * size / 2 - 1), lG.at(size * size / 2 - 1), lB.at(size * size / 2 - 1));
                rowResult[j] = qRgb(sortR[size * size / 2 - 1], sortG[size * size / 2 - 1], sortB[size * size / 2 - 1]);
            }
        }else if(gausRB->isChecked()){
            double sigma = _cg(sigmaSd->value());
            double *guask = new double[size* size];
            //double guask00;
            double guasSum = 0;
            for(int y = 0; y < size; y++){
                int cy = y - (size - 1) / 2;
                for(int x = 0; x < size; x++){
                    int cx = x - (size - 1) / 2;
                    guask[x + y * size] = 1 / (2 * M_PI * sigma * sigma) * qExp(-1.0 * (cx * cx + cy * cy) / 2 / (sigma * sigma));
                    //if(x == 1 && y == 1)qDebug()<<1 / (2 * M_PI * sigma * sigma) * qExp(-1 * (cx * cx + cy * cy) / 2 / (sigma * sigma));
//                    if(x == 0 && y == 0){
//                        guask00 = guask[0];
//                        guask[0] = 1;
//                    }else{
//                        guask[x + y * size] = qRound(guask[x + y * size] / guask00);
//                    }
                    //if(i == 100)qDebug()<<guask[x + y * size];
                    //if(i == 100)qDebug()<<(cx * cx + cy * cy);
                    guasSum += guask[x + y * size];
                }
            }
            //qDebug()<<1 / (2 * M_PI * sigma * sigma);
            //qDebug()<<-1 * (0 * 0 + 0 * 0) / 2 / (sigma * sigma);
            //qDebug()<<guask[0 + 0 * size];// / guask[0];
            for(int j = 0; j < w; j++){
                int xc = (size-1) / 2;
                for(int y = 0; y < size; y++){
                    sVals[xc + y*size] = sRows[y][j];
                }
                for(int k = 1;k <= (size-1) / 2; k++){
                    int x1 = xc - k, x2 = xc + k;
                    for(int y = 0; y < size; y++){
                        if(j - k < 0){
                            sVals[x1 + y * size] = sVals[x1 + 1 + y * size];
                        }else{
                            sVals[x1 + y * size] = sRows[y][j - k];
                        }
                        if(j + k >= w){
                            sVals[x2 + y * size] = sVals[x2 - 1 + y * size];
                        }else{
                            sVals[x2 + y * size] = sRows[y][j + k];
                        }
                    }
                }
                //
                double vR = 0,vG = 0,vB = 0,vA = 0;
                for(int v = 0; v < size * size; v++){
                    vR += qRed(sVals[v]) * guask[v];
                    vG += qGreen(sVals[v]) * guask[v];
                    vB += qBlue(sVals[v]) * guask[v];
                    vA += qAlpha(sVals[v]) * guask[v];
                }
                vR = qRound(vR / guasSum);
                vG = qRound(vG / guasSum);
                vB = qRound(vB / guasSum);
                vA = qRound(vA / guasSum);
                rowResult[j] = qRgba(vR, vG, vB, vA);
            }
        }else if(custRB->isChecked()){
            if(!custK){
                emit _console(tr("ERROR: Customize kernel not found."));
                return;
            }
            double *kernel = new double[size*size];
            double custSum = 0;
            for(int y = size - 1; y >= 0; y--){
                for(int x = size - 1; x >= 0; x--){
                    int kx = size - 1 - x;
                    int ky = size - 1 - y;
                    //kernel[kx + size * ky] =
                    //custK->item(1,1);
                    kernel[kx + size * ky] = ((custK->item(y,x))->data(Qt::DisplayRole)).toDouble();//(custK->data(custK->index(x, y))).toDouble()
                    custSum += kernel[kx + size * ky];
                    //if(i == 100) qDebug()<<kernel[kx + size * ky]<<custSum;
                }
            }
            for(int j = 0; j < w; j++){
                int xc = (size-1) / 2;
                for(int y = 0; y < size; y++){
                    sVals[xc + y*size] = sRows[y][j];
                }
                for(int k = 1;k <= (size-1) / 2; k++){
                    int x1 = xc - k, x2 = xc + k;
                    for(int y = 0; y < size; y++){
                        if(j - k < 0){
                            sVals[x1 + y * size] = sVals[x1 + 1 + y * size];
                        }else{
                            sVals[x1 + y * size] = sRows[y][j - k];
                        }
                        if(j + k >= w){
                            sVals[x2 + y * size] = sVals[x2 - 1 + y * size];
                        }else{
                            sVals[x2 + y * size] = sRows[y][j + k];
                        }
                    }
                }
                double vR = 0,vG = 0,vB = 0,vA = 0;
                for(int v = 0; v < size * size; v++){
                    vR += qRed(sVals[v]) * kernel[v];
                    vG += qGreen(sVals[v]) * kernel[v];
                    vB += qBlue(sVals[v]) * kernel[v];
                    vA += qAlpha(sVals[v]) * kernel[v];
                }
                vR = qRound(vR / custSum);
                vG = qRound(vG / custSum);
                vB = qRound(vB / custSum);
                vA = qRound(vA / custSum);
                rowResult[j] = qRgba(vR, vG, vB, vA);
            }
        }
    }
    emit _resultImage(result, resultView);
    emit _console(QString("Filter is applied, using %1 filter.").arg(opName));
}

void DIPModuleCV::switchFilter(bool on)
{
    if(on && sender()){
        if(sender() == meanRB || sender() == mediRB){
            sigmaLb->setVisible(false);
            sigmaSd->setVisible(false);
            table->setVisible(false);
        }else if(sender() == gausRB){
            sigmaLb->setVisible(true);
            sigmaSd->setVisible(true);
            table->setVisible(false);
        }else if(sender() == custRB){
            sigmaLb->setVisible(false);
            sigmaSd->setVisible(false);
            table->setVisible(true);
            if(!custK || custK->columnCount() != _cz(sizeSd->value())){
                if(custK) delete custK;
                custK = new QStandardItemModel(_cz(sizeSd->value()), _cz(sizeSd->value()), table);
                table->setModel(custK);
                for (int y = 0; y < _cz(sizeSd->value()); ++y) {
                    for (int x = 0; x < _cz(sizeSd->value()); ++x) {
                        custK->setItem(y, x, new QStandardItem("1"));
                        table->setColumnWidth(x,table->rowHeight(y));
                    }
                }
            }
        }

        edgeGB->adjustSize();
        filterGB->adjustSize();
        filterGB->resize(edgeGB->size().width(), filterGB->size().height());
        DIPModuleBase::getUI()->adjustSize();
        emit _adjustDialogSize();
    }
}

void DIPModuleCV::calSdLb(int value)
{
    if(sender()){
        if(sender() == sigmaSd){
            if(sigmaLb->isVisible()){
                sigmaLb->setText(QString("Sigma: %1").arg(_cg(value)));
            }
        }
        if(sender() == sizeSd){
            if(sizeLb->isVisible()){
                sizeLb->setText(QString("Kernel Size: %1").arg(_cz(value)));
            }
        }
    }else{
        sigmaLb->setText(QString("Sigma: %1").arg(_cg(sigmaSd->value())));
        sizeLb->setText(QString("Kernel Size: %1").arg(_cz(sizeSd->value())));
    }
}

void DIPModuleCV::updateCustTable(int value)
{
    if(!custK || !custRB->isChecked() || custK->columnCount() == _cz(value)){
        return;
    }
    if(custK) delete custK;
    custK = new QStandardItemModel(_cz(sizeSd->value()), _cz(sizeSd->value()), table);
    table->setModel(custK);
    for (int y = 0; y < _cz(sizeSd->value()); ++y) {
        for (int x = 0; x < _cz(sizeSd->value()); ++x) {
            custK->setItem(y, x, new QStandardItem("1"));
            table->setColumnWidth(x,table->rowHeight(y));
        }
    }
}
