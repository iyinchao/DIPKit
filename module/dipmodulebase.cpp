#include "dipmodulebase.h"

DIPModuleBase::DIPModuleBase(DIPImageView *source, DIPImageView *result, QWidget *parent)
    :QObject(parent)
{
    this->parent = parent;
    sourceView = source;
    resultView = result;
    //initUI();
}

QWidget *DIPModuleBase::getUI()
{
    if(ui){
        return ui;
    }
    return 0;
}

const QString *DIPModuleBase::getModuleName()
{
    return moduleName;
}

void DIPModuleBase::initUI()
{
    ui = new QWidget(parent);
    emit _loadUI(ui);
}
