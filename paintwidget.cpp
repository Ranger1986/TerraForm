#include "paintwidget.h"
#include <iostream>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QString>
#include <QPainter>
#include <QSize>
#include <QMouseEvent>
#include <QPen>
#include <QFileDialog>
#include <QSpinBox>

PaintWidget::PaintWidget(QWidget *parent)
    : QWidget{parent}
{
    setMouseTracking(true);
    setMinimumHeight(400);
    setMinimumWidth(400);
    QVBoxLayout *mainLayout = new QVBoxLayout;
    //mainLayout->addWidget(testLabel);
    setLayout(mainLayout);
    scribbling = false;
    mode = 0;
    startPoint = QPoint(0,0);
    lastPoint = QPoint(399,399);

    // Set the current settings for the pen
    myPen = new QPen;
    myPen->setWidth(1);
    myPen->setColor(QColor(0, 0, 0, 5));
    myBrush = new QBrush(QColor(0, 0, 0, 5));
    setFocusPolicy( Qt::StrongFocus );
    generateFlatPlane();
}

void PaintWidget::setMode(int index)
{
    mode = index;
}
void PaintWidget::setOpacity(int opacity)
{
    this->opacity=opacity;
    updatePen();
}
void PaintWidget::setWidth(int width)
{
    this->width=width;
}

void PaintWidget::generateFlatPlane(){
    image = QImage(QSize(400,400), QImage::Format_ARGB32);
    image.fill(QColor(127,127,127,255));
    generateCalque();
    update();
    emit image_changed(image);
}
void PaintWidget::generateCalque(){
    calques = QVector<QImage>();
    QImage calque = QImage(QSize(400,400), QImage::Format_ARGB32);
    calque.fill(QColor(0,0,0,0));
    calques.append(calque);
}
void PaintWidget::openImage(){
    QImage loadedImage;
    if (!loadedImage.load(QFileDialog::getOpenFileName(this, tr("Open Image"), "../../", tr("Image Files (*.png *.jpg *.bmp)"))))
        return;
    image = loadedImage.scaled(400,400);
    generateCalque();
    update();
    emit image_changed(image);
    return;
}

void PaintWidget::paintEvent(QPaintEvent *_event){

    QPainter painter(this);

    // Returns the rectangle that needs to be updated
    QRect dirtyRect(0,0,400,400) ;

    // Draws the rectangle where the image needs to
    // be updated
    painter.drawImage(dirtyRect, image, dirtyRect);
    painter.drawImage(dirtyRect, calques[0], dirtyRect);

}

void PaintWidget::previewPen(const QPoint &point)
{

    // Used to draw on the widget
    calques[0].fill(QColor(0,0,0,0));
    QPainter painter(&calques[0]);
    painter.setPen(*myPen);
    painter.setBrush(*myBrush);
    painter.drawEllipse(point.x()-width/2 , point.y()-width/2,width, width);
    update();
}

void PaintWidget::drawLineTo(const QPoint &endPoint)
{
    // Used to draw on the widget
    QPainter painter(&image);

    painter.setPen(*myPen);
    painter.setBrush(*myBrush);

    painter.drawEllipse(endPoint.x()-width/2 , endPoint.y()-width/2,width, width);

    update();
}

void PaintWidget::showSquare(){
    // Used to draw on the widget
    QPainter painter(&calques[0]);
    int x1 =lastPoint.x();
    int x2 =startPoint.x();
    int y1 =lastPoint.y();
    int y2 =startPoint.y();
    // Set the current settings for the pen
    QPen selecPen = QPen();
    selecPen.setWidth(5);
    selecPen.setColor(QColor(0, 0, 0, 200));
    painter.setPen(selecPen);

    calques[0].fill(QColor(0, 0, 0, 0));
    // Draw a line from the last registered point to the current
    painter.drawLine(QPoint(x1,y1), QPoint(x1,y2));
    painter.drawLine(QPoint(x1,y2), QPoint(x2,y2));
    painter.drawLine(QPoint(x2,y2), QPoint(x2,y1));
    painter.drawLine(QPoint(x2,y1), QPoint(x1,y1));

    // Set that the image hasn't been saved
    //modified = true;

    // Call to update the rectangular space where we drew
    update();

}

void PaintWidget::updatePen()
{
    myPen->setWidth(1);
    myPen->setColor(QColor(0, 0, 0, opacity));
    myBrush->setColor(QColor(0, 0, 0, opacity));
}

void PaintWidget::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        lastPoint = event->pos();
        scribbling = true;
        switch (mode){
            case 1:
                startPoint = event->pos();
                break;
            default:
                break;
        }
    }
}
void PaintWidget::mouseMoveEvent(QMouseEvent *event)
{
    if ((event->buttons() & Qt::LeftButton) && scribbling && QRect(0,0,400,400).contains(event->pos())){
        switch (mode){
            case 0:
                drawLineTo(event->pos());
                break;

            case 1:
                lastPoint = event->pos();
                showSquare();
                break;
            default:
                break;
        }
    }
    else if(mode == 0)previewPen(event->pos());
        //emit modified_signal(image);
}

void PaintWidget::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton && scribbling ) {
        switch (mode){
            case 0:
                drawLineTo(event->pos());
                break;
            default:
                break;
        }
        scribbling = false;
        emit image_changed(image);
    }
}

void PaintWidget::keyPressEvent(QKeyEvent *event){
    std::cout<<"key pressed"<<std::endl;
    if(event->key() == Qt::Key_Shift)
    {
        myPen->setColor(QColor(255, 255, 255, myPen->color().alpha()));
        myBrush->setColor(QColor(255, 255, 255, myPen->color().alpha()));
    }
}

void PaintWidget::keyReleaseEvent(QKeyEvent *event){
    std::cout<<"key released"<<std::endl;
    if(event->key() == Qt::Key_Shift)
    {
        myPen->setColor(QColor(0, 0, 0, myPen->color().alpha()));
        myBrush->setColor(QColor(0, 0, 0, myPen->color().alpha()));
    }
}


void PaintWidget::randomNoise(){
    int alpha = parent()->findChild<QSpinBox*>("noiseAlpha")->value();
    if (alpha == 0)return;
    for(int i = 0; i<400 ; i++){
        for(int j = 0; j<400 ; j++){
            QColor color = image.pixelColor(i,j);
            int randomColor = random()%255;
            std::cout << randomColor << std::endl;
            color.setBlue(int((float(color.blue())/255+float(randomColor)/alpha)/2*255));
            color.setRed(int((float(color.red())/255+float(randomColor)/alpha)/2*255));
            color.setGreen(int((float(color.green())/255+float(randomColor)/alpha)/2*255));
            image.setPixelColor(i,j,color);
        }
    }
    update();
    emit image_changed(image);
}
