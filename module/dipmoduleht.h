#ifndef DIPMODULEHT_H
#define DIPMODULEHT_H

#include <QGroupBox>
#include <QPushButton>
#include <QSlider>
#include "dipmodulebase.h"

class DIPModuleHT : public DIPModuleBase
{
    Q_OBJECT
public:
    DIPModuleHT(DIPImageView *source, DIPImageView *result, QWidget *parent = 0);

protected:
    void initUI();

private:
    QGroupBox *histoGB;
    QGroupBox *thresGB;
    QGridLayout *histoGBL;
    QPushButton *histoEquBt;
    QPushButton *otsuBt;
    QPushButton *entropyBt;
    QSlider *thresSd;
    QGridLayout *mainLt;

public slots:
    void __doHistoEqu();

};

#endif // DIPMODULEHT_H
