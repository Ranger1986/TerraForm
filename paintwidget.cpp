#include "paintwidget.h"
#include <iostream>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QString>
#include <QPainter>
#include <QSize>
#include <QMouseEvent>
#include <QPen>

PaintWidget::PaintWidget(QWidget *parent)
    : QWidget{parent}
{
    //resize(400,400);
    /*
    testLabel = new QLabel(this);
    testLabel->setText("PLOUP");
    */
    QVBoxLayout *mainLayout = new QVBoxLayout;
    //mainLayout->addWidget(testLabel);
    setLayout(mainLayout);
    scribbling = false;
}
void PaintWidget::resizeImage(QImage *image, const QSize &newSize)
{
    // Check if we need to redraw the image
    if (image->size() == newSize)
        return;

    // Create a new image to display and fill it with white
    QImage newImage(newSize, QImage::Format_RGB32);
    newImage.fill(qRgb(255, 255, 255));

    // Draw the image
    QPainter painter(&newImage);
    painter.drawImage(QPoint(0, 0), *image);
    *image = newImage;
}
void PaintWidget::openImage(QString fileName){
    // Holds the image
    QImage loadedImage;

    // If the image wasn't loaded leave this function
    if (!loadedImage.load(fileName))
        return;

    //QSize newSize = loadedImage.size().expandedTo(size());
    //resizeImage(&loadedImage, QSize(400,400));
    image = loadedImage.scaled(400,400);
    emit modified_signal(image);
    update();
    return;
}
void PaintWidget::paintEvent(QPaintEvent *_event){

    QPainter painter(this);

    // Returns the rectangle that needs to be updated
    QRect dirtyRect(0,0,400,400) ;

    // Draws the rectangle where the image needs to
    // be updated
    painter.drawImage(dirtyRect, image, dirtyRect);

}

void PaintWidget::drawLineTo(const QPoint &endPoint)
{
    // Used to draw on the widget
    QPainter painter(&image);

    // Set the current settings for the pen
    QPen myPen = QPen();
    myPen.setWidth(25);
    myPen.setColor(QColor(0, 0, 0, 150));
    painter.setPen(myPen);/*myPenColor, myPenWidth, Qt::SolidLine, Qt::RoundCap,
                       Qt::RoundJoin));*/

    // Draw a line from the last registered point to the current
    painter.drawLine(lastPoint, endPoint);

    // Set that the image hasn't been saved
    //modified = true;

    int rad = (myPen.width() / 2) + 2;

    // Call to update the rectangular space where we drew
    update(QRect(lastPoint, endPoint).normalized()
                                     .adjusted(-rad, -rad, +rad, +rad));

    // Update the last position where we left off drawing
    lastPoint = endPoint;
}

void PaintWidget::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        lastPoint = event->pos();
        scribbling = true;
    }
}

// When the mouse moves if the left button is clicked
// we call the drawline function which draws a line
// from the last position to the current
void PaintWidget::mouseMoveEvent(QMouseEvent *event)
{
    if ((event->buttons() & Qt::LeftButton) && scribbling)
        drawLineTo(event->pos());
}

// If the button is released we set variables to stop drawing
void PaintWidget::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton && scribbling) {
        drawLineTo(event->pos());
        scribbling = false;
        emit modified_signal(image);
    }
}

