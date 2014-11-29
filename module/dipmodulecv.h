#ifndef DIPMODULECV_H
#define DIPMODULECV_H

#include <QGroupBox>
#include <QPushButton>
#include <QRadioButton>
#include <QSlider>
#include <QTableView>
#include <QMouseEvent>
#include <QStandardItemModel>
#include "dipmodulebase.h"

class DIPModuleCV : public DIPModuleBase
{
    Q_OBJECT
public:
    DIPModuleCV(DIPImageView *source, DIPImageView *result, QWidget *parent = 0);

protected:
    void initUI();

private:
    const int *K_SOBEL = new int[1,2,3];
    const int *K_PREWITT;
    const int *K_ROBERTS;
    QGridLayout *mainLt;
    QGroupBox *edgeGB;
    QGridLayout *edgeGBL;
    QPushButton *robertsBt;
    QPushButton *sobelBt;
    QPushButton *prewittBt;
    QLabel *edgeDirL;
    QRadioButton *edgeXRB;
    QRadioButton *edgeYRB;
    QRadioButton *edgeCRB;
    QGroupBox *filterGB;
    QGridLayout *filterGBL;
    QRadioButton *meanRB;
    QRadioButton *mediRB;
    QRadioButton *gausRB;
    QRadioButton *custRB;
    QSlider *sizeSd;
    QLabel *sizeLb;
    QSlider *sigmaSd;
    QLabel *sigmaLb;
    QTableView *table;
    QPushButton *doFilterBt;
    QStandardItemModel *custK;
    int _cz(int value);
    double _cg(int value);
public slots:
    void __doEdgeDete();
    void __doFilter();
    void switchFilter(bool on);
    void calSdLb(int value = -1);
    void updateCustTable(int value);
};

#endif // DIPMODULECV_H
