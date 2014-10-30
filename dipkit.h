#ifndef DIPKIT_H
#define DIPKIT_H

#include <QMainWindow>

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
    Ui::DIPKit *ui;
};

#endif // DIPKit_H
