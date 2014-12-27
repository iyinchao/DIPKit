#include "dipmodulemm.h"

DIPModuleMM::DIPModuleMM(DIPImageView *source, DIPImageView *result, QWidget *parent):DIPModuleBase(source, result, parent)
{
    initUI();
    moduleName = new QString(tr("Mathematical morphology"));
}

void DIPModuleMM::initUI()
{
    DIPModuleBase::initUI();

    seGB = new QGroupBox(tr("Structure Element (SE)"), parent);
    seGBL = new QGridLayout(seGB);
    table = new QTableWidget(seGB);
    sizeSd = new QSlider(Qt::Horizontal, seGB);
    sizeLb = new QLabel(tr("Editor Size:"), seGB);
    opGB = new QGroupBox(tr("Operations"), parent);
    opGBL = new QGridLayout(opGB);
    opeBt = new QPushButton(tr("Open"), opGB);
    cloBt = new QPushButton(tr("Close"), opGB);
    dilBt = new QPushButton(tr("Dilation"), opGB);
    eroBt = new QPushButton(tr("Erosion"), opGB);
    recBt = new QPushButton(tr("Binary Reconstruction"), opGB);
    origIcon = QIcon(QIcon(":/resource/icon/origin.png"));
    origX = 0;
    origY = 0;

    seGBL->addWidget(sizeLb, 1,0,1,1);
    seGBL->addWidget(sizeSd, 2,0,1,1);
    seGBL->addWidget(table, 3,0,1,1);
    sizeSd->setMaximum(9);
    sizeSd->setMinimum(1);
    sizeSd->setValue(1);
    sizeSd->setPageStep(1);
    sizeSd->setTickPosition(QSlider::TicksBelow);
    opGBL->addWidget(dilBt, 1,0,1,1);
    opGBL->addWidget(eroBt, 1,1,1,1);
    opGBL->addWidget(opeBt, 2,0,1,1);
    opGBL->addWidget(cloBt, 2,1,1,1);
    opGBL->addWidget(recBt, 3,0,1,2);
    table->setMinimumSize(200, 200);

    mainLt = new QGridLayout(DIPModuleBase::getUI());
    mainLt->addWidget(seGB,0,0,1,1);
    mainLt->addWidget(opGB,1,0,1,1);

    DIPModuleBase::getUI()->setLayout(mainLt);

    calSdLb();

//    se = new QStandardItemModel(_cz(sizeSd->value()), _cz(sizeSd->value()), table);
//    table->setModel(se);
//    for (int y = 0; y < _cz(sizeSd->value()); ++y) {
//        for (int x = 0; x < _cz(sizeSd->value()); ++x) {
//            se->setItem(y, x, new QStandardItem(""));
//            table->setColumnWidth(x,table->rowHeight(y));
//        }
//    }

    //connect(se, SIGNAL(itemChanged(QStandardItem*)), this, SLOT(seMultiEdit(QStandardItem*)));
    connect(table, SIGNAL(doubleClicked(QModelIndex)), this ,SLOT(setSEOrigin(QModelIndex)));
    connect(table, SIGNAL(cellChanged(int,int)), this ,SLOT(seMultiEdit(int, int)));
    connect(sizeSd, SIGNAL(valueChanged(int)), this, SLOT(calSdLb(int)));
    connect(sizeSd, SIGNAL(valueChanged(int)), this, SLOT(updateEditor(int)));
    connect(dilBt, SIGNAL(clicked()), this, SLOT(__doDilEro()));
    connect(eroBt, SIGNAL(clicked()), this, SLOT(__doDilEro()));
    connect(opeBt, SIGNAL(clicked()), this, SLOT(__doOpen()));
    connect(cloBt, SIGNAL(clicked()), this, SLOT(__doClose()));
    connect(recBt, SIGNAL(clicked()), this, SLOT(__doReconstruction()));
    sizeSd->setValue(2);
    sizeSd->setValue(1);
}

void DIPModuleMM::calSdLb(int value)
{
    if(sender()){
        if(sender() == sizeSd){
            if(sizeLb->isVisible()){
                sizeLb->setText(QString("Editor Size: %1").arg(_cz(value)));
            }
        }
    }else{
        sizeLb->setText(QString("Editor Size: %1").arg(_cz(sizeSd->value())));
    }
}

void DIPModuleMM::updateEditor(int value)
{
//    if(!se || se->columnCount() == _cz(value)){
//        return;
//    }
//    if(se) delete se;
//    se = new QStandardItemModel(_cz(sizeSd->value()), _cz(sizeSd->value()), table);
//    table->setModel(se);
//    for (int y = 0; y < _cz(sizeSd->value()); ++y) {
//        for (int x = 0; x < _cz(sizeSd->value()); ++x) {
//            se->setItem(y, x, new QStandardItem(""));
//            table->setColumnWidth(x,table->rowHeight(y));
//        }
//    }
//    connect(se, SIGNAL(itemChanged(QStandardItem*)), this, SLOT(seMultiEdit(QStandardItem*)));
    //qDebug()<<value;
    table->setColumnCount(_cz(value));
    table->setRowCount(_cz(value));
    for(int i = 0; i < table->columnCount(); i++){
        table->setColumnWidth(i, table->rowHeight(0));
        for(int j = 0; j < table->rowCount(); j++){
            if(!table->item(j, i)){
                table->setItem(j, i, new QTableWidgetItem(""));
            }
        }
    }
    //table->item(origY, origX)->setIcon(QIcon());
    table->item(origY, origX)->setIcon(QIcon());
    table->item((table->rowCount() - 1) / 2, (table->columnCount() - 1) / 2)->setIcon(origIcon);
    origX = (table->columnCount() - 1) / 2;
    origY = (table->rowCount() - 1) / 2;
    //QLabel *label = new QLabel(tr());
    //label->setAttribute(Qt::WA_TransparentForMouseEvents);
    //table->setItem(0,0,new QTableWidgetItem(QIcon(":/resource/icon/origin.png"),"sdfs"));
    //table->setCellWidget(0,0,label);
    //table->cellWidget(0,0)->setStyleSheet("background-color:#FF0000;opacity:0.5");
}

void DIPModuleMM::seMultiEdit(int row, int col)
{
//    QModelIndexList selectedIndex = table->selectionModel()->selectedIndexes();
//    foreach(QModelIndex i, selectedIndex){
//        //se->index(i.row(), i.column()).
//        se->item(i.row(), i.column())->setData(item->data(Qt::DisplayRole), Qt::EditRole);
//    }
//    //qDebug()<<(item->data(Qt::DisplayRole)).toDouble();
    QList<QTableWidgetSelectionRange> selection = table->selectedRanges();
    QList<QTableWidgetSelectionRange>::iterator i;
    int n = 0;
    for(i = selection.begin(); i != selection.end(); i++){
        for(int y = (*i).topRow(); y <= (*i).bottomRow(); y++){
            for(int x = (*i).leftColumn(); x <= (*i).rightColumn(); x++){
                n++;
            }
        }
    }
    if(n <= 1) return;
    for(i = selection.begin(); i != selection.end(); i++){
        for(int y = (*i).topRow(); y <= (*i).bottomRow(); y++){
            for(int x = (*i).leftColumn(); x <= (*i).rightColumn(); x++){
                table->item(y, x)->setData(Qt::DisplayRole, table->item(row, col)->data(Qt::DisplayRole));
            }
        }
    }
}

void DIPModuleMM::setSEOrigin(QModelIndex index)
{
    table->item(origY, origX)->setIcon(QIcon());
    table->item(index.row(), index.column())->setIcon(origIcon);
    origX = index.column();
    origY = index.row();
}

void DIPModuleMM::__doDilEro()
{
    if(!sourceView->isImageLoaded()){
        emit _console(tr("ERROR: Source image is not loaded"));
        return;
    }

    OP op = OP::DIL;
    if(sender()){
        if(sender() == dilBt){
            op = OP::DIL;
        }else if(sender() == eroBt){
            op = OP::ERO;
        }
    }

    emit _resultImage(dilEro(sourceView->getImage(), op), resultView);
    emit _console(QString("%1 operation is done.").arg(op == OP::DIL?"Dilation":"Erosion"));
}

void DIPModuleMM::__doOpen()
{
    if(!sourceView->isImageLoaded()){
        emit _console(tr("ERROR: Source image is not loaded"));
        return;
    }

    emit _resultImage(dilEro(dilEro(sourceView->getImage(), OP::ERO), OP::DIL), resultView);
    emit _console(QString("Open operation is done."));
}

void DIPModuleMM::__doClose(){
    if(!sourceView->isImageLoaded()){
        emit _console(tr("ERROR: Source image is not loaded"));
        return;
    }

    emit _resultImage(dilEro(dilEro(sourceView->getImage(), OP::DIL), OP::ERO), resultView);
    emit _console(QString("Close operation is done."));
}


//see http://stackoverflow.com/questions/17317047/binary-reconstruction-algorithm
//img = imread ('Input.jpg');
//img = im2bw(img, 0.8);
//J = bwmorph(img,'open');
//THRESH = 0;
//while (1)
//    T = J;
//    J = bwmorph(J, 'dilate');
//    J = img & J;
//    if (sum(T(:) - J(:)) <= THRESH)
//        break;
//    end
//end
void DIPModuleMM::__doReconstruction()
{
    if(!sourceView->isImageLoaded()){
        emit _console(tr("ERROR: Source image is not loaded"));
        return;
    }

    //OTSU method to get binary image
   /* QImage *source = sourceView->getImage();
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

    sourceView->setImage(result);*/

    //do reconstruction
    QImage *source = sourceView->getImage();
    QImage *J= dilEro(dilEro(sourceView->getImage(), OP::ERO), OP::DIL);
    QImage *T = NULL;
    int THRES = 0;
    do{
            if(T) delete T;
        T = _copyQImage(J);
            QImage *temp = J;
        J = dilEro(J, OP::DIL);
            delete temp;
            temp = J;
        J = _binaryUnion(source, J);
            delete temp;
    }while(_diffPixels(J, T) > THRES);

    emit _resultImage(T, resultView);
    emit _console(QString("Binary reconstruction is done."));
}

int DIPModuleMM::_cz(int value)
{
    return value * 2 + 1;
}

int DIPModuleMM::_diffPixels(QImage *ia, QImage *ib)
{
    qDebug()<<ia;
    qDebug()<<ib;
//    !ia || !ib || !ia->isNull() || !ib->isNull() ||
    if(ia->width() != ib->width() || ia->height() != ib->height()){
        return -1;
    }

    int n = 0;
    int h = ia->height();
    int w = ia->width();
    for(int i = 0; i < h; i++){
        QRgb *ra = (QRgb*)ia->scanLine(i);
        QRgb *rb = (QRgb*)ib->scanLine(i);
        for(int j = 0; j < w; j++){
            if(qGray(ra[j]) != qGray(rb[j])){
                n++;
            }
        }
    }
    qDebug()<<n;
    return n;
}

QImage *DIPModuleMM::_binaryUnion(QImage *ia, QImage *ib)
{
    if(!ia || !ib || ia->isNull() || ib->isNull() ||ia->width() != ib->width() || ia->height() != ib->height()){
        return NULL;
    }

    int h = ia->height();
    int w = ia->width();
    QImage *result = new QImage(w,h,QImage::Format_ARGB32);
    for(int i = 0; i < h; i++){
        QRgb *ra = (QRgb*)ia->scanLine(i);
        QRgb *rb = (QRgb*)ib->scanLine(i);
        QRgb* rowResult = (QRgb*)result->scanLine(i);
        for(int j = 0; j < w; j++){
//            if(qGray(ra[j]) != qGray(rb[j])){
//                n++;
//            }
            if(qGray(ra[j]) && qGray(rb[j])){
                rowResult[j] = qRgb(255,255,255);
            }else{
                rowResult[j] = qRgb(0,0,0);
            }
        }
    }

    return result;
}

QImage *DIPModuleMM::_copyQImage(QImage *source)
{
    if(!source || source->isNull()){
        return NULL;
    }

    int w = source->width(); //image dimensions
    int h = source->height();
    QImage *result = new QImage(w,h,QImage::Format_ARGB32);
    for(int i = 0; i < h; i++){
        QRgb* rS = (QRgb*)source->scanLine(i);
        QRgb* rR = (QRgb*)result->scanLine(i);
        for(int j = 0; j < w; j++){
            rR[j] = rS[j];
        }
    }
    return result;
}

QImage *DIPModuleMM::dilEro(QImage *source, OP op)
{
    int seSize = _cz(sizeSd->value());
    double seSum = 0;
    double seMax = -10000;
    double seMin = 10000;
    double *seVal = new double[seSize * seSize];
    for(int k = 0;k < seSize * seSize; k++){
//        if((table->item(k / seSize, k % seSize)->data(Qt::DisplayRole)).toString() == QString("")){
//            if(op == OP::ERO){
//                seVal[k] = 1000;
//            }else{
//                seVal[k] = -1000;
//            }
//        }else{
//            seVal[k] = (table->item(k / seSize, k % seSize)->data(Qt::DisplayRole)).toDouble();
//        }
        seVal[k] = (table->item(k / seSize, k % seSize)->data(Qt::DisplayRole)).toDouble();
        seSum += seVal[k];
        if(seMax < seVal[k]) seMax = seVal[k];
        if(seMin > seVal[k]) seMin = seVal[k];
    }

    int w = source->width();
    int h = source->height();
    QImage *result = new QImage(w,h,QImage::Format_ARGB32);
    QRgb **rows = new QRgb*[seSize];
    QRgb *area = new QRgb[seSize * seSize];

    for(int i = 0; i < h; i++){
        for(int k =0; k <seSize; k++){
            if(k + i - origY < 0 || k + i - origY >= h){
                rows[k] = NULL;
            }else{
               rows[k] = (QRgb*)source->scanLine(k + i - origY);
            }
        }
        QRgb* rowResult = (QRgb*)result->scanLine(i);
        for(int j = 0; j < w; j++){
            double top;
            if(op == OP::DIL){
                top = -std::numeric_limits<double>::max();
            }else{
                top = std::numeric_limits<double>::max();
            }
            //double top = (op == OP::DIL?-10000:(seMax * 2550));
//            if(i == 1 && j== 1){
//                qDebug()<<top;
//            }
            QRgb topPixel = qRgba(0,0,0,100);// = (op == OP::DIL)?qRgb(0,0,0);
            for(int k = 0; k < seSize * seSize; k++){
                int l = k % seSize;
                if(l + j - origX < 0 || l + j - origX >= w || rows[k / seSize] == NULL || seVal[k] == 0){
                    continue;
                    //area[k] = (op == OP::DIL)?qRgb(0,0,0):qRgb(255,255,255);
                }else{
                    area[k] = rows[k / seSize][l + j - origX];
                }
                if(op == OP::DIL){
                    //qDebug()<<qGray(area[k]);
                    //qDebug()<<top<<qGray(area[k]) * seVal[k];
                    if(i == 0 && j== 4){
                        //qDebug()<<"in!"<<(top < (double)(qGray(area[k]) * seVal[k]))<<top<<qGray(area[k]) * seVal[k];
                    }
                    if(top < qGray(area[k]) * seVal[k]){
                        top = qGray(area[k]) * seVal[k];
                        topPixel = area[k];
                    }
                }else{
//                    if(i == 0 && j== 0){
//                        qDebug()<<top<<qGray(area[k]);
//                    }
                    if(top > qGray(area[k]) * seVal[k]){
                        top = qGray(area[k]) * seVal[k];
                        topPixel = area[k];
                    }
                }
            }
            rowResult[j] = topPixel;
        }
    }

    return result;
}


