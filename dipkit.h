#ifndef DIPKIT_H
#define DIPKIT_H

#include <QMainWindow>
#include "dipimageview.h"

namespace Ui {
class DIPKit;
}



class DIPKit : public QMainWindow
{
    Q_OBJECT

public:
    explicit DIPKit(QWidget *parent = 0);
    ~DIPKit();

private:
    //Ui::DIPKit *ui;
    void initUI();
    DIPImageView *viewer;
    QGridLayout *layoutMain;
    DIPImageView *viewLeft;
    DIPImageView *viewRight;
};



#endif // DIPKit_H
