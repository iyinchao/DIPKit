#include "dipmodulemm.h"

DIPModuleMM::DIPModuleMM(DIPImageView *source, DIPImageView *result, QWidget *parent):DIPModuleBase(source, result, parent)
{
    initUI();
    moduleName = new QString(tr("Mathematical morphology"));
}

void DIPModuleMM::initUI()
{
    DIPModuleBase::initUI();

    mainLt = new QGridLayout(DIPModuleBase::getUI());


    DIPModuleBase::getUI()->setLayout(mainLt);
}
