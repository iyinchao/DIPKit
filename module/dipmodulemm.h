#ifndef DIPMODULEMM_H
#define DIPMODULEMM_H

#include "dipmodulebase.h"

class DIPModuleMM : public DIPModuleBase
{
    Q_OBJECT
public:
    DIPModuleMM(DIPImageView *source, DIPImageView *result, QWidget *parent = 0);
protected:
    void initUI();

private:
    QGridLayout *mainLt;
};

#endif // DIPMODULEMM_H
