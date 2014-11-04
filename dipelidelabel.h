#ifndef DIPELIDELABEL_H
#define DIPELIDELABEL_H

#include <QLabel>
#include <QDebug>


class DIPElideLabel : public QLabel
{
public:
    DIPElideLabel(QWidget *parent = 0);
    DIPElideLabel(Qt::TextElideMode elideMode, const QString& text = 0, QWidget *parent = 0);
    Qt::TextElideMode elideMode();
    void setElideMode(Qt::TextElideMode elideMode);
    void setText(const QString &text);
    QString text()const;
protected:
    void paintEvent(QPaintEvent *event);
private:
    Qt::TextElideMode mode;
    QString str;

};

#endif // DIPELIDELABEL_H
