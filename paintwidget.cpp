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
    setMinimumHeight(400);
    setMinimumWidth(400);
    QVBoxLayout *mainLayout = new QVBoxLayout;
    //mainLayout->addWidget(testLabel);
    setLayout(mainLayout);
    scribbling = false;
    state = Action::None;
}
void PaintWidget::openImage(QString fileName){
    // Holds the image
    QImage loadedImage;
    std::cout << "openImage" << std::endl;
    // If the image wasn't loaded leave this function
    if (!loadedImage.load(fileName))
        return;

    //QSize newSize = loadedImage.size().expandedTo(size());
    //resizeImage(&loadedImage, QSize(400,400));
    image = loadedImage.scaled(400,400);
    calque = QImage(QSize(400,400), QImage::Format_ARGB32);
    calque.fill(QColor(0, 0, 0, 0));
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
    painter.drawImage(dirtyRect, calque, dirtyRect);

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

void PaintWidget::showSquare(){
    // Used to draw on the widget
    QPainter painter(&calque);
    int x1 =lastPoint.x();
    int x2 =startPoint.x();
    int y1 =lastPoint.y();
    int y2 =startPoint.y();
    // Set the current settings for the pen
    QPen myPen = QPen();
    myPen.setWidth(5);
    myPen.setColor(QColor(0, 0, 0, 200));
    painter.setPen(myPen);/*myPenColor, myPenWidth, Qt::SolidLine, Qt::RoundCap,
                       Qt::RoundJoin));*/

    calque.fill(QColor(0, 0, 0, 0));
    // Draw a line from the last registered point to the current
    painter.drawLine(QPoint(x1,y1), QPoint(x1,y2));
    painter.drawLine(QPoint(x1,y2), QPoint(x2,y2));
    painter.drawLine(QPoint(x2,y2), QPoint(x2,y1));
    painter.drawLine(QPoint(x2,y1), QPoint(x1,y1));

    // Set that the image hasn't been saved
    //modified = true;

    int rad = (myPen.width() / 2) + 2;

    // Call to update the rectangular space where we drew
    update(QRect(QPoint(0,0), QPoint(400,400)).normalized()
                                     .adjusted(-rad, -rad, +rad, +rad));

}

void PaintWidget::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        lastPoint = event->pos();
        scribbling = true;
        switch (state){
            case Action::DrawSquare:
                startPoint = event->pos();
            default:
                break;
        }
    }
}

// When the mouse moves if the left button is clicked
// we call the drawline function which draws a line
// from the last position to the current
void PaintWidget::mouseMoveEvent(QMouseEvent *event)
{
    if ((event->buttons() & Qt::LeftButton) && scribbling){
        switch (state){
            case Action::Draw:
                drawLineTo(event->pos());

        case Action::DrawSquare:
                lastPoint = event->pos();
                showSquare();
            default:
                break;
        }

    }
        //emit modified_signal(image);
}

// If the button is released we set variables to stop drawing
void PaintWidget::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton && scribbling ) {
        switch (state){
            case Action::Draw:
                drawLineTo(event->pos());
            default:
            break;
        }
        scribbling = false;
        emit modified_signal(image);
    }
}
void PaintWidget::setDraw(){
    state=Action::Draw;
}
void PaintWidget::setDrawSquare(){
    state=Action::DrawSquare;
}
