#include "dipcolortag.h"

DIPColorTag::DIPColorTag(QWidget *parent) :
    QWidget(parent)
{
    //qDebug()<<height();
    passive = false;
    w = 150;
    h = 60;
    rc = 5;
    offsetX = 15;
    offsetY = 15;
    bPad = 5;
    mx = 0;
    my = 0;
    px = 0;
    py = 0;
    brush = QBrush();
    //brush.setColor(QColor(0,0,0,200));
    brush.setStyle(Qt::SolidPattern);
    pen = QPen();
    pen.setCapStyle(Qt::RoundCap);
    pen.setWidth(2);
    pen.setStyle(Qt::SolidLine);
    this->parent = parent;
    this->setVisible(false);
}

bool DIPColorTag::isPassive() const
{
    return passive;
}

void DIPColorTag::setPassive(bool value)
{
    passive = value;
}

void DIPColorTag::showContent(QColor *color, int mouse_x, int mouse_y, int pic_x, int pic_y)
{
    if(!isVisible())this->setVisible(true);
    nowColor = color;
    mx = mouse_x;
    my = mouse_y;
    px = pic_x;
    py = pic_y;
    this->update();
}

void DIPColorTag::hideContent()
{
    if(isVisible())this->setVisible(false);
    passive = false;
}

void DIPColorTag::paintEvent(QPaintEvent *event)
{
    QWidget::paintEvent(event);

    QPainter painter(this);

    //painter.drawRoundedRect(bgSize, rc, rc, Qt::AbsoluteSize);

    int ox, oy;
    if(mx + offsetX + w + bPad >= this->parent->width()){
        ox = this->parent->width() - bPad - w;
    }else if(mx + offsetX <= bPad){
        ox = bPad;
    }else{
        ox = mx + offsetX;
    }
    if(my + offsetY + h + bPad >= this->parent->height()){
        oy = this->parent->height() - bPad - h;
    }else if(my + offsetY <= bPad){
        oy = bPad;
    }else{
        oy = my + offsetY;
    }
    painter.setRenderHint(QPainter::Antialiasing, true);
    brush.setColor(QColor(0,0,0,150));
    painter.setBrush(brush);
    painter.setPen(Qt::NoPen);
    painter.drawRoundedRect(QRect(ox, oy, w, h), rc, rc, Qt::AbsoluteSize);
    for (int x = 0; x < 40; x += 10) {
        for (int y = 0; y < 40; y += 10) {
            painter.save();
            //painter.translate(x, y);
            if((x / 10) % 2 == (y / 10) % 2){
                painter.fillRect(x + ox + 10, y + oy + 10, 10, 10, QColor(255,255,255));
            }else{
                painter.fillRect(x + ox + 10, y + oy + 10, 10, 10, QColor(225,225,225));
            }
            painter.restore();
        }
    }
    brush.setColor(*nowColor);
    painter.setBrush(brush);
    painter.drawRect(ox + 10, oy + 10, 40, 40);
    painter.setFont(QFont(tr("Microsoft YaHei"), 9));
    pen.setColor(QColor(255,255,255,150));
    painter.setPen(pen);
    painter.drawText(QPoint(ox + 60, oy + 20), QString("(%1, %2)").arg(px).arg(py));
    QString r,g,b,a;
    a = QString::number(qAlpha(nowColor->rgba()));
    if(qAlpha(nowColor->rgba()) > 0){
        r = QString::number(qRed(nowColor->rgba()));
        g = QString::number(qGreen(nowColor->rgba()));
        b = QString::number(qBlue(nowColor->rgba()));
    }else{
        r = QString("--");
        g = QString("--");
        b = QString("--");
    }
    painter.drawText(QPoint(ox + 61, oy + 35), /*QString().sprintf("R:%ls   G:%ls", r, g));//*/QString("R:%1").arg(r));
    painter.drawText(QPoint(ox + 106, oy + 35), QString("G:%1").arg(g));
    painter.drawText(QPoint(ox + 61, oy + 50), QString("B:%1").arg(b));
    painter.drawText(QPoint(ox + 106, oy + 50), QString("A:%1").arg(a));
    //painter.drawText(QPoint(ox + 61, oy + 50), QString("B:%1   A:%2").arg(b).arg(a));

    //painter.fillRect(0, 0, width(), height(), QColor(0,0,0, 100));
    if(passive){
        brush.setColor(QColor(0,0,0,150));
        pen.setColor(QColor(255,255,255,150));
        painter.setBrush(brush);
        painter.setPen(pen);
        painter.drawEllipse(QPoint(mx, my), 4, 4);
    }
}



