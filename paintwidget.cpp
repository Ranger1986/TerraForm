#include "paintwidget.h"
#include <iostream>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QString>
#include <QPainter>
#include <QSize>

PaintWidget::PaintWidget(QWidget *parent)
    : QWidget{parent}
{
    //resize(400,400);
    /*
    testLabel = new QLabel(this);
    testLabel->setText("PLOUP");
    */
    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(testLabel);
    setLayout(mainLayout);
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

    QSize newSize = loadedImage.size().expandedTo(size());
    //resizeImage(&loadedImage, QSize(400,400));
    image = loadedImage.scaled(400,400);
    update();
    return;
}
void PaintWidget::paintEvent(QPaintEvent *event){

    QPainter painter(this);

    // Returns the rectangle that needs to be updated
    QRect dirtyRect(0,0,400,400) ;

    // Draws the rectangle where the image needs to
    // be updated
    painter.drawImage(dirtyRect, image, dirtyRect);

}
