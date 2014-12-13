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
            table->setItem(j, i, new QTableWidgetItem(""));
        }
    }
    //table->item(origY, origX)->setIcon(QIcon());
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

void DIPModuleMM::__doDilEro(int op)
{
    if(!sourceView->isImageLoaded()){
        emit _console(tr("ERROR: Source image is not loaded"));
        return;
    }

    if(sender()){
        if(sender() == dilBt){
            op = OP::DIL;
        }else if(sender() == eroBt){
            op = OP::ERO;
        }
    }

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
    //qDebug()<<seMax<<seMin;
    QImage *source = sourceView->getImage();
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
//                        if(i == 0 && j== 4){
//                            qDebug()<<"in!"<<(top < qGray(area[k]) * seVal[k])<<top<<qGray(area[k]) * seVal[k];
//                        }

                        top = qGray(area[k]) * seVal[k];
                        topPixel = area[k];
//                        if(i == 3 && j== 0){
//                            qDebug()<<"in!"<<top;

//                        }
                        //qDebug()<<"in!"<<i<<j;
                        //qDebug()<<qGray(area[k]);
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

    //qDebug()<<qRed(((QRgb*)result->scanLine(3))[3]);

    emit _resultImage(result, resultView);
}

void DIPModuleMM::__doOpen()
{
    if(!sourceView->isImageLoaded()){
        emit _console(tr("ERROR: Source image is not loaded"));
        return;
    }
}

int DIPModuleMM::_cz(int value)
{
    return value * 2 + 1;
}
