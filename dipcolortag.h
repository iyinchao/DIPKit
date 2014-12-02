#ifndef DIPCOLORTAG_H
#define DIPCOLORTAG_H

#include <QWidget>
#include <QDebug>
#include <QPainter>
#include <QImage>

class DIPColorTag : public QWidget
{
    Q_OBJECT
public:
    explicit DIPColorTag(QWidget *parent = 0);

    bool isPassive() const;
    void setPassive(bool value);

signals:

public slots:
    void showContent(QColor *color, int mouse_x, int mouse_y, int pic_x, int pic_y);
    void hideContent();

protected:
    void paintEvent(QPaintEvent *event);

private:
    bool passive;
    int w;
    int h;
    int rc;
    int offsetX;
    int offsetY;
    int bPad;
    int mx;
    int my;
    int px;
    int py;
    QBrush brush;
    QPen pen;
    QColor *nowColor;
    QWidget *parent;
};

#endif // DIPCOLORTAG_H
