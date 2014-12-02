#ifndef DIPIMAGEVIEW_H
#define DIPIMAGEVIEW_H

#include <memory>
#include <QWidget>
#include <QScrollArea>
#include <QLabel>
#include <QGridLayout>
#include <QDebug>
#include <QPainter>
#include <QtMath>
#include <QMenuBar>
#include <QMenu>
#include <QWidgetAction>
#include <QSlider>
#include <QScrollBar>
#include <QFontMetrics>
#include <QFileDialog>
#include <QImageReader>
#include <QImageWriter>
#include <QRegularExpression>
#include "dipcolortag.h"
#include "dipelidelabel.h"
#include "diphistowidget.h"
#include "dipalphascrollarea.h"
#include "dipimagelabel.h"

class DIPHistoWidget;

class DIPImageView : public QWidget {
    Q_OBJECT
private:
    bool grayMode;
    QString *titleName;
    QString *titleInfo;
    QString *titleOption;
    QImage *image;
    QImage *imageOriginal;
    DIPImageLabel *label;
    //QLabel *prompt;
    DIPElideLabel *title;
    QString *filePath;
    QGridLayout *layout;
    DIPAlphaScrollArea *scrollArea;
    DIPColorTag *colorTag;

    int **histoData;
    DIPHistoWidget *histo;
    QPainter *painter;
    QPen *pen;
    QMenuBar *menuBar;

    void init(QWidget *parent = 0);
protected:
    void paintEvent(QPaintEvent *event);
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
    enum TITLE{
        NAME,
        INFO,
        OPT,
        ALL
    };
    static int ct(int channel);
    DIPImageView(QWidget *parent = 0);
    bool isImageLoaded();
    void displayHistogram(int channel, int mode = DIPImageView::HG::RELATIVE);
    void hideHistogram();
    int *getHistoData(int channel = DIPImageView::CHANNEL_S, bool recalculate = true);
    QMenuBar *getMenuBar();
    QImage *getImage();
    QImage *convertToGray(QImage *source);
    QScrollBar *verticalScrollBar();
    QScrollBar *horizontalScrollBar();
    DIPAlphaScrollArea *alphaScrollArea();
    DIPImageLabel *imageLabel();
    bool isColorTagPassive();
    void setColorTagPassive(bool value);
public slots:
    void setTitle(QString &text, TITLE component = TITLE::ALL);
    bool saveImage(const QString &path, const char *ext = "PNG");
    const QString saveImageWithDialog();
    bool loadImage(const QString &path);
    const QString loadImageWithDialog();
    void setGrayMode(bool on);
    QImage *setImage(QImage *result, QString *info = 0);
    QImage *receiveImage();
    QImage *clearImage();
    void __emitCTShow(QColor *color, int mouse_x, int mouse_y, int pic_x, int pic_y);
    void __emitCTHide();
    void colorTagShow(QColor*, int mouse_x, int mouse_y, int pic_x, int pic_y, DIPImageView *ref = 0);
    void colorTagHide();
    void __colorTag(QColor* color, int mouse_x, int mouse_y, int pic_x, int pic_y);
signals:
    void _imageLoaded(const QString);
    void _imageSaved(const QString);
    void _imageIsSet();
    void _imageNotSet();
    void _colorTagShow(QColor*,int,int,int,int);
    void _colorTagHide();

};

#endif // DIPIMAGEVIEW_H
