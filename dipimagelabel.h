#ifndef DIPIMAGELABEL_H
#define DIPIMAGELABEL_H

#include <QLabel>
#include <QResizeEvent>
#include <QDebug>

class DIPImageLabel : public QLabel
{
    Q_OBJECT
public:
    DIPImageLabel(QWidget *parent = 0);
    void adjustSize();
public slots:
signals:
    void __adjustSize(int w, int h);
protected:
    void paintEvent(QPaintEvent *event);
    void resizeEvent(QResizeEvent *event);
private:
};

#endif // DIPIMAGELABEL_H
