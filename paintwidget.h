#ifndef PAINTWIDGET_H
#define PAINTWIDGET_H

#include <QObject>
#include <QWidget>
#include <QLabel>
#include <QImage>
#include <QPoint>

class PaintWidget : public QWidget
{
    Q_OBJECT
public:
    explicit PaintWidget(QWidget *parent = nullptr);
    QLabel *testLabel;
    void resizeImage(QImage *image, const QSize &newSize);
    void openImage(QString fileName);
    QImage image;

    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

    bool scribbling;
    QPoint lastPoint;
    void drawLineTo(const QPoint &endPoint);
signals:
    void modified_signal(QImage heightMap);

};

#endif // PAINTWIDGET_H
