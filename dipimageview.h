#ifndef DIPIMAGEVIEW_H
#define DIPIMAGEVIEW_H

#include<memory>
#include<QWidget>
#include<QScrollArea>
#include<QLabel>
#include<QGridLayout>
#include <QDebug>
#include <QPainter>
#include<QtMath>

/*QT_BEGIN_NAMESPACE
class QMenu;
class QLabel;
class QPushButton;
class QScrollArea;
class QScrollBar;
class QGridLayout;
class QScreen;
class QPlainTextEdit;
class QImage;
QT_END_NAMESPACE*/

class HistoWidget;

class DIPImageView : public QWidget {
    Q_OBJECT
private:
    QImage *image;
    QImage *imageSave;
    QLabel *label;
    QLabel *prompt;
    QString *filePath;
    QGridLayout *layout;
    QScrollArea *scrollArea;

    int **histoData;
    HistoWidget *histo;
    QPainter *painter;
    QPen *pen;


    void init(QWidget *parent = 0);

private slots:

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
    DIPImageView(const QString &promptText,QWidget *parent = 0);
    //DIPImageView();
    bool loadImage(QString &path);
    bool isImageLoaded();
    void displayHistogram(int channel, int mode);

    int* getHistoData(int channel = DIPImageView::CHANNEL_S, bool recalculate = false);


signals:
    void _imageLoaded();

};

class HistoWidget : public QWidget{
    Q_OBJECT
private:
    QPen pen;
    QBrush brush;
    int channelMarker;
    int **histoData;
    int imgH;
    int imgW;
    int *channelMax;
    bool isDisplay;
    int mode;
    int pd_t = 40;
    int pd_l = 20;
    int pd_r = 20;
    int pd_b = 20;
    int mg_l = 5;
    int mg_t = 5;
    int ct_h = 100;
    int ct_w = 256;
    int rc = 5;
    void __drawEachChannel(QPainter &painter, int channel, int mode);
protected:
    void paintEvent(QPaintEvent *event);
public slots:
    //void setShape(Shape shape);
//    void setPen(const QPen &pen);
//    void setBrush(const QBrush &brush);
    void setData(int imageW, int imageH, int** data);
    void display(int channel, int mode);
public:
    HistoWidget(QWidget *parent = 0);
};

#endif // DIPIMAGEVIEW_H
