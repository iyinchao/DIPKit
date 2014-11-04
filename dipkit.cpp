#include "dipkit.h"
#include <QFileDialog>
#include <QDebug>
//#include "ui_dipkit.h"

DIPKit::DIPKit(QWidget *parent) :
    QMainWindow(parent)
{
    initUI();
}

DIPKit::~DIPKit()
{

}

void DIPKit::initUI()
{
    viewer = new DIPImageView();
    setCentralWidget(viewer);
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open a image"),"./",tr("Images (*.png *.bmp *.jpg *.gif);;All Files (*)"));
    viewer->loadImage(fileName);

   // qDebug()<<(viewer->getHistoData(DIPImageView::CHANNEL_R))[89];

    viewer->displayHistogram(DIPImageView::CHANNEL_G | DIPImageView::CHANNEL_B | DIPImageView::CHANNEL_R | DIPImageView::CHANNEL_A | DIPImageView::CHANNEL_S, DIPImageView::HG::RELATIVE);
    //viewer->displayHistogram(DIPImageView::CHANNEL_R, DIPImageView::HG::RELATIVE);

}
