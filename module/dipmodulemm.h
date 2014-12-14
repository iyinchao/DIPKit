#ifndef DIPMODULEMM_H
#define DIPMODULEMM_H

#include <QPushButton>
#include <QGroupBox>
#include <QSlider>
#include <QRadioButton>
#include <QTableWidget>
#include <QStandardItemModel>
#include <QColor>
#include <limits>
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
    QGroupBox *seGB;
    QGridLayout *seGBL;
    QGroupBox *opGB;
    QGridLayout *opGBL;
    QTableWidget *table;
    QSlider *sizeSd;
    QLabel *sizeLb;
    QPushButton *opeBt;
    QPushButton *cloBt;
    QPushButton *dilBt;
    QPushButton *eroBt;
    QPushButton *recBt;
    QIcon origIcon;
    int origX;
    int origY;

    int _cz(int value);

    int _diffPixels(QImage *ia, QImage *ib);
    QImage *_binaryUnion(QImage *ia, QImage *ib);
    QImage *_copyQImage(QImage *source);

    enum OP{
        DIL = 0,
        ERO = 1
    };

    QImage *dilEro(QImage *source, DIPModuleMM::OP op = OP::DIL);

public slots:
    void calSdLb(int value = -1);
    void updateEditor(int value);
    void seMultiEdit(int, int);
    void setSEOrigin(QModelIndex index);
    void __doDilEro();
    void __doOpen();
    void __doClose();
    void __doReconstruction();
};

#endif // DIPMODULEMM_H
