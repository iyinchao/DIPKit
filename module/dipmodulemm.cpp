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
    table = new QTableView(seGB);
    sizeSd = new QSlider(Qt::Horizontal, seGB);
    sizeLb = new QLabel(tr("Editor Size:"), seGB);

    seGBL->addWidget(sizeLb, 1,0,1,1);
    seGBL->addWidget(sizeSd, 2,0,1,1);
    seGBL->addWidget(table, 3,0,1,1);
    sizeSd->setMaximum(5);
    sizeSd->setMinimum(1);
    sizeSd->setValue(1);
    sizeSd->setPageStep(1);
    sizeSd->setTickPosition(QSlider::TicksBelow);

    mainLt = new QGridLayout(DIPModuleBase::getUI());
    mainLt->addWidget(seGB,0,0,1,1);

    connect(sizeSd, SIGNAL(valueChanged(int)), this, SLOT(calSdLb(int)));
    connect(sizeSd, SIGNAL(valueChanged(int)), this, SLOT(updateEditor(int)));

    DIPModuleBase::getUI()->setLayout(mainLt);

    calSdLb();
    se = new QStandardItemModel(_cz(sizeSd->value()), _cz(sizeSd->value()), table);
    table->setModel(se);
    for (int y = 0; y < _cz(sizeSd->value()); ++y) {
        for (int x = 0; x < _cz(sizeSd->value()); ++x) {
            se->setItem(y, x, new QStandardItem(""));
            table->setColumnWidth(x,table->rowHeight(y));
        }
    }
    connect(se, SIGNAL(itemChanged(QStandardItem*)), this, SLOT(seMultiEdit(QStandardItem*)));

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
    if(!se || se->columnCount() == _cz(value)){
        return;
    }
    if(se) delete se;
    se = new QStandardItemModel(_cz(sizeSd->value()), _cz(sizeSd->value()), table);
    table->setModel(se);
    for (int y = 0; y < _cz(sizeSd->value()); ++y) {
        for (int x = 0; x < _cz(sizeSd->value()); ++x) {
            se->setItem(y, x, new QStandardItem(""));
            table->setColumnWidth(x,table->rowHeight(y));
        }
    }
    connect(se, SIGNAL(itemChanged(QStandardItem*)), this, SLOT(seMultiEdit(QStandardItem*)));
}

void DIPModuleMM::seMultiEdit(QStandardItem *item)
{
    QModelIndexList selectedIndex = table->selectionModel()->selectedIndexes();
    foreach(QModelIndex i, selectedIndex){
        //se->index(i.row(), i.column()).
        se->item(i.row(), i.column())->setData(item->data(Qt::DisplayRole), Qt::EditRole);
    }
    //qDebug()<<(item->data(Qt::DisplayRole)).toDouble();
}

int DIPModuleMM::_cz(int value)
{
    return value * 2 + 1;
}
