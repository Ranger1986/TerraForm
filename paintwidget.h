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
    void resizeImage(QImage *image, const QSize &newSize);
    virtual void paintEvent(QPaintEvent *event);
    void openImage(QString fileName);
    QImage image;
signals:

};

#endif // PAINTWIDGET_H
