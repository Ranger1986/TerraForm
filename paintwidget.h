#ifndef PAINTWIDGET_H
#define PAINTWIDGET_H

#include <QObject>
#include <QWidget>
#include <QLabel>
#include <QImage>
#include <QPoint>

enum class Action { None, Draw, DrawSquare, Line };
class PaintWidget : public QWidget
{
    Q_OBJECT
public:
    explicit PaintWidget(QWidget *parent = nullptr);
    QLabel *testLabel;
    void resizeImage(QImage *image, const QSize &newSize);
    void openImage(QString fileName);
    QImage image;
    QImage calque;
    Action state;

    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

    bool scribbling;
    QPoint lastPoint;
    QPoint startPoint;
    void drawLineTo(const QPoint &endPoint);
    void showSquare();
signals:
    void modified_signal(QImage heightMap);
public slots:
    void setDraw();
    void setDrawSquare();

};

#endif // PAINTWIDGET_H
