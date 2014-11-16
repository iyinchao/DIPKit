#ifndef DIPKIT_H
#define DIPKIT_H

#include <QMainWindow>
#include <QPlainTextEdit>
#include <QSplitter>
#include <QSizePolicy>
#include <QScrollBar>
#include "dipimageview.h"
#include "diptooldialog.h"

#include "module/dipmodulebase.h"
#include "module/dipmoduleht.h"
#include "module/dipmoduleag.h"

namespace Ui {
class DIPKit;
}

class DIPKit : public QMainWindow
{
    Q_OBJECT

public:
    explicit DIPKit(QWidget *parent = 0);
    ~DIPKit();

private:
    //Ui::DIPKit *ui;
    bool isViewSync;
    QWidget *mainWidget;
    QGridLayout *mainLayout;
    QSplitter *mainSplitter;
    QWidget *viewWidget;
    QGridLayout *viewLayout;
    DIPImageView *sourceView;
    DIPImageView *resultView;
    QPlainTextEdit *console;

    QMenu *mainFileMenu;
    QMenu *mainProjectMenu;
    QMenu *srcFileMenu;
    QMenu *resFileMenu;

    QAction *viewSyncAct;
    QAction *projectHTAct;
    QAction *projectAGAct;
    QAction *openSrcAct;
    QAction *graySrcAct;
    QAction *histoRSrcAct;
    QAction *histoGSrcAct;
    QAction *histoBSrcAct;
    QAction *histoASrcAct;
    QAction *histoSSrcAct;
    QAction *saveResAct;
    QAction *histoRResAct;
    QAction *histoGResAct;
    QAction *histoBResAct;
    QAction *histoAResAct;
    QAction *histoSResAct;

    DIPModuleBase *module;

    DIPToolDialog *toolDialog;


    void initUI();
    void initMenu();


public slots:
    void loadModule();
    void writeConsole(const QString &text);
    void consoleScrollBottom();
    void clearConsole();
    void applyResultImage(QImage *result, DIPImageView *dest);
    void displayHistogram();
    void viewSRScrollSync(int value);
    void toggleViewSync(bool on);

};



#endif // DIPKit_H
