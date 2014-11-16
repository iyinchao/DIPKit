#ifndef DIPMODULEAG_H
#define DIPMODULEAG_H

#include <QGroupBox>
#include <QPushButton>
#include <QMenu>
#include <QRadioButton>
#include <QSlider>

#include "dipmodulebase.h"

class DIPModuleAG : public DIPModuleBase
{
    Q_OBJECT
public:
    DIPModuleAG(DIPImageView *source, DIPImageView *result, QWidget *parent = 0);

protected:
    void initUI();

private:
    QGroupBox *algGB;
    QGroupBox *geoGB;
    QGridLayout *algGBL;
    QGridLayout *geoGBL;
    DIPImageView *opView;
    QPushButton *addBt;
    QPushButton *minusBt;
    QPushButton *multiplyBt;
    QPushButton *divideBt;
    QRadioButton *nearRB;
    QRadioButton *biliRB;
    QSlider *rotateSd;
    QLabel *rotateL;
    QSlider *scaleSd;
    QLabel *scaleL;

    QMenu *opFileMenu;
    QAction *openOpAct;
    QAction *grayOpAct;
    QAction *histoROpAct;
    QAction *histoGOpAct;
    QAction *histoBOpAct;
    QAction *histoAOpAct;
    QAction *histoSOpAct;

    int dipRed(QRgb rgb);
    int dipBlue(QRgb rgb);
    int dipGreen(QRgb rgb);
    double st(int v);
    double bilinear(double x, double y, int x1, int x2, int y1, int y2, int f11, int f12, int f21, int f22);
    //int qRed(QRgb rgb);
//    QPushButton *histoEquBt;

//    QPushButton *otsuBt;
//    QPushButton *entropyBt;
//    QLabel *thresLb;
//    QSlider *thresSd;
    QGridLayout *mainLt;

public slots:
    void __displayHistogram();
    void __doAlgebraic();
    void __doRotate();
    void __doScale();
//    void __doOtsu();
//    void __doEntropy();
//    void __doThresManual();

};

#endif // DIPMODULEAG_H
