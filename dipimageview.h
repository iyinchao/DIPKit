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
#include <QScrollBar>
#include<QFontMetrics>
#include<QFileDialog>
#include<QImageReader>
#include<QImageWriter>
#include<QRegularExpression>
#include"dipelidelabel.h"
#include"diphistowidget.h"
#include"dipalphascrollarea.h"

class DIPHistoWidget;

class DIPImageView : public QWidget {
    Q_OBJECT
private:
    bool grayMode;
    QImage *image;
    QImage *imageOriginal;
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
    QMenuBar *menuBar;

    void init(QWidget *parent = 0);

public slots:
    void setTitleText(QString &text);
    bool saveImage(const QString &path, const char *ext = "PNG");
    const QString saveImageWithDialog();
    bool loadImage(const QString &path);
    const QString loadImageWithDialog();
    void setGrayMode(bool on);
public:
    enum {
        CHANNEL_R = 0x0001,
        CHANNEL_G = 0x0002,
        CHANNEL_B = 0x0004,
        CHANNEL_A = 0x0010,
        CHANNEL_S = 0x0020,
        CHANNEL_ALL = CHANNEL_A | CHANNEL_R | CHANNEL_G | CHANNEL_B | CHANNEL_S
    };
    enum HG{
        ABSOLUTE,
        RELATIVE
    };

    static int ct(int channel);
    DIPImageView(QWidget *parent = 0);
    bool isImageLoaded();
    void displayHistogram(int channel, int mode = DIPImageView::HG::RELATIVE);
    void hideHistogram();
    int *getHistoData(int channel = DIPImageView::CHANNEL_S, bool recalculate = true);
    QMenuBar *getMenuBar();
    QImage *getImage();
    QImage *setImage(QImage *result);
    QImage *convertToGray(QImage *source);
    QScrollBar *verticalScrollBar();
    QScrollBar *horizontalScrollBar();

signals:
    void _imageLoaded(const QString);
    void _imageSaved(const QString);
    void _imageSetted();

};

#endif // DIPIMAGEVIEW_H
