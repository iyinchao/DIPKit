#ifndef DIPMODULEMM_H
#define DIPMODULEMM_H

#include <QPushButton>
#include <QGroupBox>
#include <QSlider>
#include <QRadioButton>
#include <QTableView>
#include <QStandardItemModel>
#include "dipmodulebase.h"

class DIPModuleMM : public DIPModuleBase
{
    Q_OBJECT
public:
    DIPModuleMM(DIPImageView *source, DIPImageView *result, QWidget *parent = 0);
protected:
    void initUI();

private:
    QStandardItemModel *se;
    QGridLayout *mainLt;
    QGroupBox *seGB;
    QGridLayout *seGBL;
    QTableView *table;
    QSlider *sizeSd;
    QLabel *sizeLb;
    QRadioButton *binaRB;
    QRadioButton *grayRB;
    QPushButton *OpeBt;
    QPushButton *CloBt;
    QPushButton *DilBt;
    QPushButton *EroBt;

    int _cz(int value);

public slots:
    void calSdLb(int value = -1);
    void updateEditor(int value);
    void seMultiEdit(QStandardItem *item);
};

#endif // DIPMODULEMM_H
