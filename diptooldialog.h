#ifndef DIPTOOLDIALOG_H
#define DIPTOOLDIALOG_H

#include <QWidget>
#include <QGridLayout>
#include <QLabel>

class DIPToolDialog : public QWidget
{
    Q_OBJECT
private:
    QWidget *rootUI;
    QWidget *defaultUI;
    QGridLayout *rootLayout;
    QGridLayout *tempLayout;
public:
    explicit DIPToolDialog(QWidget *parent = 0);

signals:

public slots:
    void setTitle(const QString *title);
    void loadUI(QWidget *ui);
};

#endif // DIPTOOLDIALOG_H
