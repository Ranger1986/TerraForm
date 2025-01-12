#ifndef PAINTWIDGET_H
#define PAINTWIDGET_H

#include <QObject>
#include <QWidget>
#include <QLabel>
#include <QImage>
#include <QPoint>

#include "CatmullRom.h"

class PaintWidget : public QWidget
{
    Q_OBJECT
public:
    explicit PaintWidget(QWidget *parent = nullptr);
    QLabel *testLabel;
    void resizeImage(QImage *image, const QSize &newSize);
    QImage image;
    int mode;
    void previewPen(const QPoint &point);

    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;
    //bool event(QEvent *event) override;

    bool scribbling;
    QPoint lastPoint;
    QPoint startPoint;
    QVector<QPoint> mountainsPoint;
    QPoint previousPoint;
    void draw(const QPoint &endPoint);
    void drawMountains(const QPoint &endPoint);
    void showSquare();

    int opacity;
    int width;
    QPen *myPen;
    QBrush *myBrush;
    void updatePen();
    QVector<QImage> calques;
    QImage selectionCalque;
    QImage previewCalque;
    void generateCalque();
    void updateSelection();
    void start();
    CatmullRom curve;
    int filterNeighbor;
signals:
    void image_changed(QImage heightMap);
public slots:
    void generateFlatPlane();
    void openImage();

    void setMode(int index);
    void setOpacity(int index);
    void setWidth(int index);

    void randomNoise();
    void perlinNoise();
    void randomPerlinNoise();

    void setFilterNeighbor(int n);
    void meanFilter();
    void medianFilter();



};

#endif // PAINTWIDGET_H
