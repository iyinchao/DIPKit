#ifndef DIPMODULEBASE_H
#define DIPMODULEBASE_H

#include <QWidget>
#include <QGridLayout>
#include <QtMath>
#include "../dipimageview.h"

class DIPModuleBase: public QObject
{
    Q_OBJECT
public:
    DIPModuleBase(DIPImageView *source, DIPImageView *result, QWidget *parent = 0);
    QWidget *getUI();
    const QString *getModuleName();
private:


protected:
    DIPImageView *sourceView;
    DIPImageView *resultView;
    QWidget *parent;
    QWidget *ui;
    QString *moduleName;

    void initUI();

signals:
    void _resultImage(QImage *result, DIPImageView *dest);
    void _console(const QString &);
    void _loadUI(QWidget *);
    void _adjustDialogSize();
};

#endif // DIPMODULEBASE_H
