#include "diptooldialog.h"

DIPToolDialog::DIPToolDialog(QWidget *parent) :
    QWidget(parent)
{
    setWindowTitle(tr("Tool"));
    setWindowFlags(Qt::Window | Qt::WindowTitleHint | Qt::CustomizeWindowHint);

    rootUI = defaultUI = new QLabel(tr("Select a project"));
    ((QLabel *)defaultUI)->setAlignment(Qt::AlignCenter);
    rootUI->setFont(QFont(tr("Microsoft YaHei"), 15));
    rootUI->setMinimumSize(180,250);

    rootLayout = new QGridLayout(this);
    rootLayout->addWidget(rootUI,0, 0, 1, 1, Qt::AlignCenter);
    rootLayout->setContentsMargins(0,0,0,0);

    setLayout(rootLayout);
    setFont(QFont(tr("Microsoft YaHei"),9));

    tempLayout = new QGridLayout(0);
}

void DIPToolDialog::setTitle(const QString *title)
{
    setWindowTitle(*title);
}

void DIPToolDialog::loadUI(QWidget *ui)
{
    //delete rootLayout;
    //rootLayout = new QGridLayout(this);;
    rootLayout->removeWidget(rootUI);
    delete rootUI;
    rootUI = ui;
    rootLayout->addWidget(rootUI,0,0);
    //rootLayout->activate();
    this->adjustSize();
    //setLayout(rootLayout);
    //update();
    //rootLayout->
}

void DIPToolDialog::adjustDialogSize()
{
    this->adjustSize();
    //qDebug()<<"ok"<<sizeHint()<<", "<<minimumSizeHint();
}


