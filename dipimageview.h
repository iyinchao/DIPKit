#ifndef DIPIMAGEVIEW_H
#define DIPIMAGEVIEW_H

#include<memory>
#include<QWidget>
#include<QScrollArea>
#include<QLabel>
#include<QGridLayout>
#include<QDebug>
#include<QPainter>
#include<QtMath>
#include<QMenuBar>
#include<QMenu>
#include<QWidgetAction>
#include<QSlider>
#include<QFontMetrics>
#include"dipelidelabel.h"
#include"diphistowidget.h"
#include"dipalphascrollarea.h"

class DIPHistoWidget;

class DIPImageView : public QWidget {
    Q_OBJECT
private:
    QImage *image;
    QImage *imageSave;
    QLabel *label;
    //QLabel *prompt;
    DIPElideLabel *title;
    QString *filePath;
    QGridLayout *layout;
    DIPAlphaScrollArea *scrollArea;

    int **histoData;
    DIPHistoWidget *histo;
    QPainter *painter;
    QPen *pen;

    void init(QWidget *parent = 0);

public slots:
    void setTitleText(QString &text);

public:
    enum {
        CHANNEL_R = 0x0001,
        CHANNEL_G = 0x0002,
        CHANNEL_B = 0x0004,
        CHANNEL_A = 0x0010,
        CHANNEL_S = 0x0020,
    };
    enum HG{
        ABSOLUTE,
        RELATIVE
    };

    static int ct(int channel);
    DIPImageView(QWidget *parent = 0);
    bool loadImage(QString &path);
    bool isImageLoaded();
    void displayHistogram(int channel, int mode);
    int* getHistoData(int channel = DIPImageView::CHANNEL_S, bool recalculate = false);

signals:
    void _imageLoaded();

};

#endif // DIPIMAGEVIEW_H
