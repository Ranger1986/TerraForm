#ifndef PAINTWIDGET_H
#define PAINTWIDGET_H

#include <QObject>
#include <QWidget>
#include <QLabel>
#include <QImage>

class PaintWidget : public QWidget
{
    Q_OBJECT
public:
    explicit PaintWidget(QWidget *parent = nullptr);
    QLabel *testLabel;

    virtual void paintEvent(QPaintEvent *event);

    QImage image;
signals:

};

#endif // PAINTWIDGET_H
