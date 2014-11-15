#include "dipkit.h"
#include <QFileDialog>
#include <QDebug>
//#include "ui_dipkit.h"

DIPKit::DIPKit(QWidget *parent) :
    QMainWindow(parent)
{
    module = NULL;

    initUI();
    initMenu();
    //setWindowFlags(Qt::Window | Qt::WindowTitleHint | Qt::CustomizeWindowHint);
}

DIPKit::~DIPKit()
{

}

void DIPKit::initUI()
{
    mainWidget = new QWidget(this);
    mainLayout = new QGridLayout(mainWidget);
    mainSplitter = new QSplitter(mainWidget);
    viewWidget = new QWidget(mainSplitter);
    viewLayout = new QGridLayout(viewWidget);
    sourceView = new DIPImageView(mainSplitter);
    resultView = new DIPImageView(mainSplitter);
    console = new QPlainTextEdit(mainSplitter);

    viewLayout->setContentsMargins(0,0,0,0);
    viewLayout->addWidget(sourceView,0,0,1,1);
    viewLayout->addWidget(resultView,0,1,1,1);
    viewWidget->setLayout(viewLayout);

    console->setReadOnly(true);
    console->setMinimumHeight(28);
    console->setUndoRedoEnabled(true);
    console->setLineWrapMode(QPlainTextEdit::WidgetWidth);
    console->appendPlainText(tr("Ready"));
    //console->setFont();

    mainSplitter->setOrientation(Qt::Vertical);
    mainSplitter->setChildrenCollapsible(false);
    mainSplitter->addWidget(viewWidget);
    mainSplitter->addWidget(console);
    mainSplitter->setStretchFactor(0,1);
    mainSplitter->setStretchFactor(1,0);
    QList<int> sizes;
    sizes << -1 << 28 ;
    mainSplitter->setSizes(sizes);

    //mainLayout->setContentsMargins(0,0,0,0);
    mainLayout->addWidget(mainSplitter, 0, 0, 1, 1);
    mainWidget->setLayout(mainLayout);

    toolDialog = new DIPToolDialog(this);
    toolDialog->show();

    setCentralWidget(mainWidget);
    resize(800,500);
    setFont(QFont(tr("Microsoft YaHei"),9));

    connect(console, SIGNAL(textChanged()), this, SLOT(consoleScrollBottom()));

    //mainLayout->addWidget();

// mainLayout->addWidget(sourceView,0,0,1,1);
//    mainLayout->addWidget(resultView,0,1,1,1);


    //viewer = new DIPImageView(this);
    //QString fileName = QFileDialog::getOpenFileName(this, tr("Open a image"),"./",tr("Images (*.png *.bmp *.jpg *.gif);;All Files (*)"));
    //viewer->loadImage(fileName);
    //sourceView->loadImageWithDialog();

   // qDebug()<<(viewer->getHistoData(DIPImageView::CHANNEL_R))[89];

    //sourceView->displayHistogram(DIPImageView::CHANNEL_G | DIPImageView::CHANNEL_B | DIPImageView::CHANNEL_R | DIPImageView::CHANNEL_A | DIPImageView::CHANNEL_S, DIPImageView::HG::RELATIVE);
    //viewer->displayHistogram(DIPImageView::CHANNEL_R, DIPImageView::HG::RELATIVE);
    //viewer->hideHistogram();
    //fileName = QFileDialog::getSaveFileName(this, tr("Save image"), "./", tr("PNG (*.png);;All Files (*)"));
    //viewer->saveImage(fileName);
    //viewer->saveImageWithDialog();
}

void DIPKit::initMenu()
{
    //main
    mainFileMenu = new QMenu(tr("&File"), this);
    mainProjectMenu = new QMenu(tr("&Projects"), this);
    projectHTAct = new QAction(QIcon(":/resource/icon/module.png"), tr("Project 1 (Histogram & Threshold)"), this);

    mainProjectMenu->addAction(projectHTAct);

    connect(projectHTAct, SIGNAL(triggered()), this, SLOT(loadModule()));

    menuBar()->addMenu(mainFileMenu);
    menuBar()->addMenu(mainProjectMenu);

    //source
    openSrcAct  = new QAction(QIcon(":/resource/icon/blue-open.png"), tr("&Open a image"), sourceView);
    graySrcAct  = new QAction(QIcon(":/resource/icon/gray-scale.png"), tr("&Gray scale mode"), sourceView);
    histoRSrcAct = new QAction(QIcon(":/resource/icon/histo.png"), tr("&Red Channel"), sourceView);

    srcFileMenu = new QMenu(tr("&File"), sourceView);

    graySrcAct->setCheckable(true);
    graySrcAct->setChecked(false);
    srcFileMenu->addAction(openSrcAct);
    srcFileMenu->addAction(graySrcAct);
    srcFileMenu->addSeparator();
    srcFileMenu->addAction(histoRSrcAct);

    connect(openSrcAct, SIGNAL(triggered()), sourceView, SLOT(loadImageWithDialog()));
    connect(graySrcAct, SIGNAL(triggered(bool)), sourceView, SLOT(setGrayMode(bool)));
    //connect(histoRSrcAct, SIGNAL(triggered()), sourceView, SLOT());

    sourceView->getMenuBar()->setStyleSheet("background-color: rgba(0,0,0,0%)");
    sourceView->getMenuBar()->addMenu(srcFileMenu);


   //loadModule();
}

void DIPKit::loadModule()
{
    if(sender()){
        if(module){
            delete module;
        }
        //QObject *s = sender();
        if(sender() == projectHTAct){
            module = new DIPModuleHT(sourceView, resultView, toolDialog);
        }
        toolDialog->loadUI(module->getUI());
        toolDialog->setTitle(module->getModuleName());
        connect(module, SIGNAL(_loadUI(QWidget*)), toolDialog, SLOT(loadUI(QWidget*)));
        connect(module, SIGNAL(_console(const QString &)), this, SLOT(writeConsole(QString)));
        connect(module, SIGNAL(_resultImage(QImage*,DIPImageView*)), this, SLOT(applyResultImage(QImage*,DIPImageView*)));
    }
}

void DIPKit::writeConsole(const QString &text)
{
    console->appendPlainText(text);
}

void DIPKit::consoleScrollBottom()
{
    QScrollBar *s = console->verticalScrollBar();
    s->setValue(s->maximum());
}

void DIPKit::clearConsole()
{
    console->document()->setPlainText(tr("Ready"));
}

void DIPKit::applyResultImage(QImage *result, DIPImageView *dest)
{
    dest->setImage(result);
}

//void DIPKit::loadModule(QWidget *ui)
//{
//    //(this, Qt::Window | Qt::WindowTitleHint | Qt::CustomizeWindowHint);
//    // toolDialog

//    //toolDialog->loadUI(sourceView);
//    testModule = new Test(sourceView, resultView, this);
//    connect(testModule, SIGNAL(_loadUI(QWidget*),)
//}
