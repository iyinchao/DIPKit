#ifndef DIPIMAGELABEL_H
#define DIPIMAGELABEL_H

#include <QLabel>
#include <QResizeEvent>
#include <QDebug>
#include <QColor>

class DIPImageLabel : public QLabel
{
    Q_OBJECT
public:
    DIPImageLabel(QWidget *parent = 0);
    void adjustSize();
    void setImage(QImage* image);
    QRgb getPixel(int x, int y);
public slots:
    void clear();
signals:
    void __adjustSize(int w, int h);
    void __showColorTag(QColor *color, int mouse_x, int mouse_y, int pic_x, int pic_y);
    void __hideColorTag();
protected:
    void paintEvent(QPaintEvent *event);
    void resizeEvent(QResizeEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
private:
    QColor *nowColor;
    QImage *image;
};

#endif // DIPIMAGELABEL_H
