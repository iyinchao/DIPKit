#include "dipkit.h"
#include <QFileDialog>
#include <QDebug>
//#include "ui_dipkit.h"

DIPKit::DIPKit(QWidget *parent) :
    QMainWindow(parent)
{
    module = NULL;
    isViewSync = true;
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

    sourceView->setTitle(tr("Source"), DIPImageView::TITLE::NAME);
    resultView->setTitle(tr("Result"), DIPImageView::TITLE::NAME);

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
    connect(sourceView, SIGNAL(_imageIsSet()), this, SLOT(displayHistogram()));
    connect(resultView, SIGNAL(_imageIsSet()), this, SLOT(displayHistogram()));
    connect(sourceView->verticalScrollBar(), SIGNAL(valueChanged(int)), this, SLOT(viewSRScrollSync(int)));
    connect(sourceView->horizontalScrollBar(), SIGNAL(valueChanged(int)), this, SLOT(viewSRScrollSync(int)));
    connect(resultView->verticalScrollBar(), SIGNAL(valueChanged(int)), this, SLOT(viewSRScrollSync(int)));
    connect(resultView->horizontalScrollBar(), SIGNAL(valueChanged(int)), this, SLOT(viewSRScrollSync(int)));

    //mainLayout->addWidget();

// mainLayout->addWidget(sourceView,0,0,1,1);
//    mainLayout->addWidget(resultView,0,1,1,1);


    //DIPImageView *viewer = new DIPImageView(this);
    //setCentralWidget(viewer);
    //QString fileName = QFileDialog::getOpenFileName(this, tr("Open a image"),"./",tr("Images (*.png *.bmp *.jpg *.gif);;All Files (*)"));
    //viewer->loadImage(fileName);
    //viewer->loadImageWithDialog();

   // qDebug()<<(viewer->getHistoData(DIPImageView::CHANNEL_R))[89];

    //viewer->displayHistogram(DIPImageView::CHANNEL_G | DIPImageView::CHANNEL_B | DIPImageView::CHANNEL_R | DIPImageView::CHANNEL_A | DIPImageView::CHANNEL_S, DIPImageView::HG::RELATIVE);
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
    viewSyncAct = new QAction(QIcon(":/resource/icon/view-sync.png"), tr("&Sync viewer scroll"), this);
    toggleToolAct = new QAction(QIcon(":/resource/icon/tool-dialog.png"), tr("&Toggle tool dialog"), this);
    projectHTAct = new QAction(QIcon(":/resource/icon/module.png"), tr("Project 1 (Histogram && Threshold)"), this);
    projectAGAct = new QAction(QIcon(":/resource/icon/module.png"), tr("Project 2 (Algebraic && Geometry Operation)"), this);
    projectCVAct = new QAction(QIcon(":/resource/icon/module.png"), tr("Project 3 (Convolution && Filters)"), this);
    projectMMAct = new QAction(QIcon(":/resource/icon/module.png"), tr("Project 4 (Mathematical morphology)"), this);

    viewSyncAct->setCheckable(true);
    viewSyncAct->setChecked(true);
    toggleToolAct->setCheckable(true);
    toggleToolAct->setChecked(true);
    //projectHTAct->setIconText();

    mainFileMenu->addAction(viewSyncAct);
    mainFileMenu->addAction(toggleToolAct);
    mainProjectMenu->addAction(projectHTAct);
    mainProjectMenu->addAction(projectAGAct);
    mainProjectMenu->addAction(projectCVAct);
    mainProjectMenu->addAction(projectMMAct);

    connect(viewSyncAct, SIGNAL(triggered(bool)), this, SLOT(toggleViewSync(bool)));
    connect(toggleToolAct, SIGNAL(triggered(bool)), this, SLOT(toggleTool(bool)));
    connect(projectHTAct, SIGNAL(triggered()), this, SLOT(loadModule()));
    connect(projectAGAct, SIGNAL(triggered()), this, SLOT(loadModule()));
    connect(projectCVAct, SIGNAL(triggered()), this, SLOT(loadModule()));
    connect(projectMMAct, SIGNAL(triggered()), this, SLOT(loadModule()));

    menuBar()->addMenu(mainFileMenu);
    menuBar()->addMenu(mainProjectMenu);

    //source
    openSrcAct  = new QAction(QIcon(":/resource/icon/blue-open.png"), tr("&Open a image"), sourceView);
    saveSrcAct = new QAction(QIcon(":/resource/icon/save.png"), tr("&Save"), sourceView);
    graySrcAct  = new QAction(QIcon(":/resource/icon/gray-scale.png"), tr("&Gray scale mode"), sourceView);
    histoRSrcAct = new QAction(QIcon(":/resource/icon/histo.png"), tr("&Red Channel"), sourceView);
    histoGSrcAct = new QAction(QIcon(":/resource/icon/histo.png"), tr("&Green Channel"), sourceView);
    histoBSrcAct = new QAction(QIcon(":/resource/icon/histo.png"), tr("&Blue Channel"), sourceView);
    histoASrcAct = new QAction(QIcon(":/resource/icon/histo.png"), tr("&Alpha Channel"), sourceView);
    histoSSrcAct = new QAction(QIcon(":/resource/icon/histo.png"), tr("Gray &Scale Channel"), sourceView);

    srcFileMenu = new QMenu(tr("&File"), sourceView);

    graySrcAct->setCheckable(true);
    graySrcAct->setChecked(false);
    histoRSrcAct->setCheckable(true);
    histoGSrcAct->setCheckable(true);
    histoBSrcAct->setCheckable(true);
    histoASrcAct->setCheckable(true);
    histoSSrcAct->setCheckable(true);
    histoRSrcAct->setChecked(false);
    histoGSrcAct->setChecked(false);
    histoBSrcAct->setChecked(false);
    histoASrcAct->setChecked(false);
    histoSSrcAct->setChecked(false);


    srcFileMenu->addAction(openSrcAct);
    srcFileMenu->addAction(saveSrcAct);
    srcFileMenu->addAction(graySrcAct);
    srcFileMenu->addSeparator();
    srcFileMenu->addAction(histoRSrcAct);
    srcFileMenu->addAction(histoGSrcAct);
    srcFileMenu->addAction(histoBSrcAct);
    srcFileMenu->addAction(histoASrcAct);
    srcFileMenu->addAction(histoSSrcAct);

    connect(openSrcAct, SIGNAL(triggered()), sourceView, SLOT(loadImageWithDialog()));
    connect(saveSrcAct, SIGNAL(triggered()), sourceView, SLOT(saveImageWithDialog()));
    connect(graySrcAct, SIGNAL(triggered(bool)), sourceView, SLOT(setGrayMode(bool)));
    connect(histoRSrcAct, SIGNAL(triggered()), this, SLOT(displayHistogram()));
    connect(histoGSrcAct, SIGNAL(triggered()), this, SLOT(displayHistogram()));
    connect(histoBSrcAct, SIGNAL(triggered()), this, SLOT(displayHistogram()));
    connect(histoASrcAct, SIGNAL(triggered()), this, SLOT(displayHistogram()));
    connect(histoSSrcAct, SIGNAL(triggered()), this, SLOT(displayHistogram()));


    sourceView->getMenuBar()->setStyleSheet("background-color: rgba(0,0,0,0%)");
    sourceView->getMenuBar()->addMenu(srcFileMenu);

    //result
    forwardAct = new QAction(QIcon(":/resource/icon/forward-left.png"), tr("&Forward to source"), resultView);
    saveResAct = new QAction(QIcon(":/resource/icon/save.png"), tr("&Save"), resultView);
    histoRResAct = new QAction(QIcon(":/resource/icon/histo.png"), tr("&Red Channel"), resultView);
    histoGResAct = new QAction(QIcon(":/resource/icon/histo.png"), tr("&Green Channel"), resultView);
    histoBResAct = new QAction(QIcon(":/resource/icon/histo.png"), tr("&Blue Channel"), resultView);
    histoAResAct = new QAction(QIcon(":/resource/icon/histo.png"), tr("&Alpha Channel"), resultView);
    histoSResAct = new QAction(QIcon(":/resource/icon/histo.png"), tr("Gray &Scale Channel"), resultView);
    resFileMenu = new QMenu(tr("&File"), resultView);

    histoRResAct->setCheckable(true);
    histoGResAct->setCheckable(true);
    histoBResAct->setCheckable(true);
    histoAResAct->setCheckable(true);
    histoSResAct->setCheckable(true);
    histoRResAct->setChecked(false);
    histoGResAct->setChecked(false);
    histoBResAct->setChecked(false);
    histoAResAct->setChecked(false);
    histoSResAct->setChecked(false);

    resFileMenu->addAction(forwardAct);
    resFileMenu->addAction(saveResAct);
    resFileMenu->addSeparator();
    resFileMenu->addAction(histoRResAct);
    resFileMenu->addAction(histoGResAct);
    resFileMenu->addAction(histoBResAct);
    resFileMenu->addAction(histoAResAct);
    resFileMenu->addAction(histoSResAct);

    connect(forwardAct, SIGNAL(triggered()), sourceView, SLOT(receiveImage()));
    connect(saveResAct, SIGNAL(triggered()), resultView, SLOT(saveImageWithDialog()));
    connect(histoRResAct, SIGNAL(triggered()), this, SLOT(displayHistogram()));
    connect(histoGResAct, SIGNAL(triggered()), this, SLOT(displayHistogram()));
    connect(histoBResAct, SIGNAL(triggered()), this, SLOT(displayHistogram()));
    connect(histoAResAct, SIGNAL(triggered()), this, SLOT(displayHistogram()));
    connect(histoSResAct, SIGNAL(triggered()), this, SLOT(displayHistogram()));

    resultView->getMenuBar()->setStyleSheet("background-color: rgba(0,0,0,0%)");
    resultView->getMenuBar()->addMenu(resFileMenu);


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
        }else if(sender() == projectAGAct){
            module = new DIPModuleAG(sourceView, resultView, toolDialog);
        }else if(sender() == projectCVAct){
            module = new DIPModuleCV(sourceView, resultView, toolDialog);
        }else if(sender() == projectMMAct){
            module = new DIPModuleMM(sourceView, resultView, toolDialog);
        }

        toolDialog->loadUI(module->getUI());
        toolDialog->setTitle(module->getModuleName());
        connect(module, SIGNAL(_loadUI(QWidget*)), toolDialog, SLOT(loadUI(QWidget*)));
        connect(module, SIGNAL(_console(const QString &)), this, SLOT(writeConsole(QString)));
        connect(module, SIGNAL(_resultImage(QImage*,DIPImageView*)), this, SLOT(applyResultImage(QImage*,DIPImageView*)));
        connect(module, SIGNAL(_adjustDialogSize()), toolDialog, SLOT(adjustDialogSize()));
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

void DIPKit::displayHistogram()
{
    int channelS = 0;
    int channelR = 0;
    if(histoRSrcAct->isChecked()){
        channelS |= DIPImageView::CHANNEL_R;
    }
    if(histoGSrcAct->isChecked()){
        channelS |= DIPImageView::CHANNEL_G;
    }
    if(histoBSrcAct->isChecked()){
        channelS |= DIPImageView::CHANNEL_B;
    }
    if(histoASrcAct->isChecked()){
        channelS |= DIPImageView::CHANNEL_A;
    }
    if(histoSSrcAct->isChecked()){
        channelS |= DIPImageView::CHANNEL_S;
    }
    if(channelS){
        sourceView->displayHistogram(channelS);
    }else{
        sourceView->hideHistogram();
    }

    if(histoRResAct->isChecked()){
        channelR |= DIPImageView::CHANNEL_R;
    }
    if(histoGResAct->isChecked()){
        channelR |= DIPImageView::CHANNEL_G;
    }
    if(histoBResAct->isChecked()){
        channelR |= DIPImageView::CHANNEL_B;
    }
    if(histoAResAct->isChecked()){
        channelR |= DIPImageView::CHANNEL_A;
    }
    if(histoSResAct->isChecked()){
        channelR |= DIPImageView::CHANNEL_S;
    }
    if(channelR){
        resultView->displayHistogram(channelR);
    }else{
        resultView->hideHistogram();
    }

}

void DIPKit::viewSRScrollSync(int value)
{
    if(QObject::sender() == NULL || isViewSync == false){
        return;
    }else{
        if(QObject::sender() == sourceView->verticalScrollBar()){
            resultView->verticalScrollBar()->setValue(value);
        }else if(QObject::sender() == sourceView->horizontalScrollBar()){
            resultView->horizontalScrollBar()->setValue(value);
        }else if(QObject::sender() == resultView->verticalScrollBar()){
            sourceView->verticalScrollBar()->setValue(value);
        }else if(QObject::sender() == resultView->horizontalScrollBar()){
            sourceView->horizontalScrollBar()->setValue(value);
        }
    }
}

void DIPKit::toggleViewSync(bool on)
{
    if(on){
        isViewSync = true;
        resultView->verticalScrollBar()->setValue(sourceView->verticalScrollBar()->value());
        resultView->horizontalScrollBar()->setValue(sourceView->horizontalScrollBar()->value());
    }else{
        isViewSync = false;
    }
}

void DIPKit::toggleTool(bool show)
{
    if(show){
        toolDialog->show();
    }else{
        toolDialog->hide();
    }
}

//void DIPKit::loadModule(QWidget *ui)
//{
//    //(this, Qt::Window | Qt::WindowTitleHint | Qt::CustomizeWindowHint);
//    // toolDialog

//    //toolDialog->loadUI(sourceView);
//    testModule = new Test(sourceView, resultView, this);
//    connect(testModule, SIGNAL(_loadUI(QWidget*),)
//}
