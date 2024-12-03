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
    void drawLineTo(const QPoint &endPoint);
    void showSquare();

    int opacity;
    int width;
    QPen *myPen;
    QBrush *myBrush;
    void updatePen();
    QVector<QImage> calques;
    void generateCalque();
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



};

#endif // PAINTWIDGET_H
