#ifndef DIPMODULEHT_H
#define DIPMODULEHT_H

#include <QGroupBox>
#include <QPushButton>
#include <QSlider>
#include <QLabel>
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
    QGridLayout *thresGBL;
    QPushButton *otsuBt;
    QPushButton *entropyBt;
    QLabel *thresLb;
    QSlider *thresSd;
    QGridLayout *mainLt;

public slots:
    void __doHistoEqu();
    void __doOtsu();
    void __doEntropy();
    void __doThresManual();

};

#endif // DIPMODULEHT_H
