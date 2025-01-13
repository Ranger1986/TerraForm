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
#include <cmath>
#include<QDebug>
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

    mountainsPoint = QVector<QPoint>();
    updatePen();
    curve = CatmullRom();
    curve.set_steps(20);

    filterNeighbor=1;
    mountainHeight=10;
    mountainWidth=10;
}

void PaintWidget::setMode(int index)
{
    mode = index;
    if (index != 0)previewCalque.fill(QColor(0,0,0,0));
    update();
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
    selectionCalque =  QImage(QSize(400,400), QImage::Format_ARGB32);
    selectionCalque.fill(QColor(0,0,0,0));
    previewCalque =  QImage(QSize(400,400), QImage::Format_ARGB32);
    previewCalque.fill(QColor(0,0,0,0));
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
    painter.drawImage(dirtyRect, selectionCalque, dirtyRect);
    painter.drawImage(dirtyRect, previewCalque, dirtyRect);

}

void PaintWidget::previewPen(const QPoint &point)
{

    // Used to draw on the widget
    previewCalque.fill(QColor(0,0,0,0));
    QPainter painter(&previewCalque);
    painter.setPen(*myPen);
    painter.setBrush(*myBrush);
    painter.drawEllipse(point.x()-width/2 , point.y()-width/2,width, width);
    update();
}

void PaintWidget::draw(const QPoint &endPoint)
{
    // Used to draw on the widget
    QImage tempImg = QImage(image);

    QPainter painter(&image);

    painter.setPen(*myPen);
    painter.setBrush(*myBrush);

    painter.drawEllipse(endPoint.x()-width/2 , endPoint.y()-width/2,width, width);

    for(int i = 0; i<400 ; i++){
        for(int j = 0; j<400 ; j++){
            if(! (i>startPoint.x()&&j>startPoint.y() && i<lastPoint.x()&&j<lastPoint.y())){
                image.setPixelColor(i,j,tempImg.pixelColor(i,j));
            }
        }
    }

    update();
}
void PaintWidget::drawMountains(const QPoint &endPoint)
{
    if (mountainsPoint.size()==0)return;
    // Used to draw on the widget
    calques[0].fill(QColor(0,0,0,0));
    QPainter painter(&calques[0]);

    QPen selecPen = QPen();
    selecPen.setWidth(5);
    selecPen.setColor(QColor(0, 0, 0, 200));
    painter.setPen(selecPen);
    for(int i = 0;  i<mountainsPoint.size()-1 ; i++)painter.drawLine(mountainsPoint[i],mountainsPoint[i+1]);
    painter.drawLine(mountainsPoint[mountainsPoint.size()-1],endPoint);

    selecPen.setColor(QColor(0, 0, 200,200));
    painter.setPen(selecPen);
    for(int i = 0;  i<mountainsPoint.size() ; i++)painter.drawEllipse(mountainsPoint[i],5,5);

    QImage newImage = image;
    if (mountainsPoint.size()==20){
        for(int i = 0; i<=400 ; i++){
            for(int j = 0; j<=400 ; j++){
                if(i>startPoint.x()&&j>startPoint.y() && i<lastPoint.x()&&j<lastPoint.y()){
                    int distance=999;
                    int closestPoint=0;
                    for(int k = 0; k<20 ; k++){
                        int tempDistance = sqrt(pow(mountainsPoint[k].x()-i,2) + pow(mountainsPoint[k].y()-j,2));
                        if (tempDistance < distance){
                            closestPoint = k;
                            distance = tempDistance;
                        }
                    }
                    if (distance < mountainWidth){
                        int colorSommet = image.pixelColor(mountainsPoint[closestPoint]).red()+mountainHeight;
                        int colorCurrent = image.pixelColor(i,j).red();
                        int colorDiff = colorSommet-colorCurrent;
                        if (colorDiff >=0){
                            int finalC = colorCurrent+mountainHeight/mountainWidth*(mountainWidth-distance);
                            newImage.setPixelColor(i,j,QColor(finalC,finalC,finalC));
                        }
                    }
                }
            }
        }
        image=newImage;
        emit image_changed(image);
        mountainsPoint.clear();
        curve = CatmullRom();
        curve.set_steps(20);
    }

    update();
}

void PaintWidget::showSquare(){
    // Used to draw on the widget
    QPainter painter(&selectionCalque);
    int x1 =lastPoint.x();
    int x2 =startPoint.x();
    int y1 =lastPoint.y();
    int y2 =startPoint.y();
    // Set the current settings for the pen
    QPen selecPen = QPen();
    selecPen.setWidth(5);
    selecPen.setColor(QColor(0, 0, 0, 200));
    painter.setPen(selecPen);

    selectionCalque.fill(QColor(0, 0, 0, 0));
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
        previousPoint = event->pos();
        scribbling = true;
        switch (mode){
            case 1:
                startPoint = event->pos();
                break;
            case 3:
                mountainsPoint.append(event->pos());
                curve.add_way_point(Vector(event->pos().x(), event->pos().y(), 0));
                std::cout << curve.node_count() << std::endl;
                if (curve.node_count()>0){
                    mountainsPoint.clear();
                    for (int i = 0 ; i<20 ; i++)mountainsPoint.append(QPoint(curve.node(i).x,curve.node(i).y));
                }
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
                draw(event->pos());
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
    else if(mode == 3)drawMountains(event->pos());
        //emit modified_signal(image);
}

void PaintWidget::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton && scribbling ) {
        switch (mode){
            case 0:
                draw(event->pos());
                break;
            case 1:
                updateSelection();
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
    for(int i = 0; i<=400 ; i++){
        for(int j = 0; j<=400 ; j++){
            if(i>startPoint.x()&&j>startPoint.y() && i<lastPoint.x()&&j<lastPoint.y()){
                QColor color = image.pixelColor(i,j);
                int randomColor = random()%255;
                color.setBlue(int(float(color.blue()*255+randomColor*alpha)/(255+alpha)));
                color.setRed(int(float(color.blue()*255+randomColor*alpha)/(255+alpha)));
                color.setGreen(int(float(color.blue()*255+randomColor*alpha)/(255+alpha)));
                image.setPixelColor(i,j,color);
            }
        }
    }
    update();
    emit image_changed(image);
}

float Get2DPerlinNoiseValue(float x, float y, float res)
{
    float tempX,tempY;
    int x0,y0,ii,jj,gi0,gi1,gi2,gi3;
    float unit = 1.0f/sqrt(2);
    float tmp,s,t,u,v,Cx,Cy,Li1,Li2;
    float gradient2[][2] = {{unit,unit},{-unit,unit},{unit,-unit},{-unit,-unit},{1,0},{-1,0},{0,1},{0,-1}};

    unsigned int perm[] =
        {151,160,137,91,90,15,131,13,201,95,96,53,194,233,7,225,140,36,103,30,69,
         142,8,99,37,240,21,10,23,190,6,148,247,120,234,75,0,26,197,62,94,252,219,
         203,117,35,11,32,57,177,33,88,237,149,56,87,174,20,125,136,171,168,68,175,
         74,165,71,134,139,48,27,166,77,146,158,231,83,111,229,122,60,211,133,230,220,
         105,92,41,55,46,245,40,244,102,143,54,65,25,63,161,1,216,80,73,209,76,132,
         187,208,89,18,169,200,196,135,130,116,188,159,86,164,100,109,198,173,186,3,
         64,52,217,226,250,124,123,5,202,38,147,118,126,255,82,85,212,207,206,59,227,
         47,16,58,17,182,189,28,42,223,183,170,213,119,248,152,2,44,154,163,70,221,
         153,101,155,167,43,172,9,129,22,39,253,19,98,108,110,79,113,224,232,178,185,
         112,104,218,246,97,228,251,34,242,193,238,210,144,12,191,179,162,241,81,51,145,
         235,249,14,239,107,49,192,214,31,181,199,106,157,184,84,204,176,115,121,50,45,
         127,4,150,254,138,236,205,93,222,114,67,29,24,72,243,141,128,195,78,66,215,61,
         156,180};

    //Adapter pour la résolution
    x /= res;
    y /= res;

    //On récupère les positions de la grille associée à (x,y)
    x0 = (int)(x);
    y0 = (int)(y);

    //Masquage
    ii = x0 & 255;
    jj = y0 & 255;

    //Pour récupérer les vecteurs
    gi0 = perm[ii + perm[jj]] % 8;
    gi1 = perm[ii + 1 + perm[jj]] % 8;
    gi2 = perm[ii + perm[jj + 1]] % 8;
    gi3 = perm[ii + 1 + perm[jj + 1]] % 8;

    //on récupère les vecteurs et on pondère
    tempX = x-x0;
    tempY = y-y0;
    s = gradient2[gi0][0]*tempX + gradient2[gi0][1]*tempY;

    tempX = x-(x0+1);
    tempY = y-y0;
    t = gradient2[gi1][0]*tempX + gradient2[gi1][1]*tempY;

    tempX = x-x0;
    tempY = y-(y0+1);
    u = gradient2[gi2][0]*tempX + gradient2[gi2][1]*tempY;

    tempX = x-(x0+1);
    tempY = y-(y0+1);
    v = gradient2[gi3][0]*tempX + gradient2[gi3][1]*tempY;


    //Lissage
    tmp = x-x0;
    Cx = 3 * tmp * tmp - 2 * tmp * tmp * tmp;

    Li1 = s + Cx*(t-s);
    Li2 = u + Cx*(v-u);

    tmp = y - y0;
    Cy = 3 * tmp * tmp - 2 * tmp * tmp * tmp;

    return Li1 + Cy*(Li2-Li1);
}

void PaintWidget::perlinNoise(){
    int alpha = parent()->findChild<QSpinBox*>("noiseAlpha")->value();
    if (alpha == 0)return;
    for(int i = 0; i<=400 ; i++){
        for(int j = 0; j<=400 ; j++){
            if(i>startPoint.x()&&j>startPoint.y() && i<lastPoint.x()&&j<lastPoint.y()){
                QColor color = image.pixelColor(i,j);
                int randomColor = (Get2DPerlinNoiseValue(i,j, 100.0)+1)*0.5*255;
                color.setBlue(int(float(color.blue()*255+randomColor*alpha)/(255+alpha)));
                color.setRed(int(float(color.blue()*255+randomColor*alpha)/(255+alpha)));
                color.setGreen(int(float(color.blue()*255+randomColor*alpha)/(255+alpha)));
                image.setPixelColor(i,j,color);
            }
        }
    }
    update();
    emit image_changed(image);
}
void PaintWidget::randomPerlinNoise(){
    int alpha = parent()->findChild<QSpinBox*>("noiseAlpha")->value();
    if (alpha == 0)return;
    int xOffset = random()%400;
    int yOffset = random()%400;
    for(int i = 0; i<=400 ; i++){
        for(int j = 0; j<=400 ; j++){
            if(i>startPoint.x()&&j>startPoint.y() && i<lastPoint.x()&&j<lastPoint.y()){
                QColor color = image.pixelColor(i,j);
                int randomColor = (Get2DPerlinNoiseValue(i+xOffset,j+yOffset, 100.0)+1)*0.5*255;
                color.setBlue(int(float(color.blue()*255+randomColor*alpha)/(255+alpha)));
                color.setRed(int(float(color.blue()*255+randomColor*alpha)/(255+alpha)));
                color.setGreen(int(float(color.blue()*255+randomColor*alpha)/(255+alpha)));
                image.setPixelColor(i,j,color);
            }
        }
    }
    update();
    emit image_changed(image);
}
void PaintWidget::updateSelection(){
    int x1,x2,y1,y2;
    if (startPoint.x()<lastPoint.x()){
        x1=startPoint.x();
        x2=lastPoint.x();
    }
    else{
        x2=startPoint.x();
        x1=lastPoint.x();
    }
    if (startPoint.y()<lastPoint.y()){
        y1=startPoint.y();
        y2=lastPoint.y();
    }
    else{
        y2=startPoint.y();
        y1=lastPoint.y();
    }
    startPoint.setX(x1);
    startPoint.setY(y1);
    lastPoint.setX(x2);
    lastPoint.setY(y2);
}
void distancePointSegment(QPoint a, QPoint b, QPoint c){
    QPoint projete = ((b-a) * QPoint::dotProduct(a,c));
}
void PaintWidget::start(){
    emit image_changed(image);
}

void PaintWidget::meanFilter(){
    qDebug() << "function meanFilter called";
    QImage newImage = image;
    for(int i = filterNeighbor; i<=400-filterNeighbor ; i++){
        for(int j = filterNeighbor; j<=400-filterNeighbor ; j++){
            if(i>startPoint.x()&&j>startPoint.y() && i<lastPoint.x()&&j<lastPoint.y()){
                //qDebug() << i << "|" << j;
                int r=0;
                for(int i2 = i-filterNeighbor; i2<=i+filterNeighbor ; i2++){
                    for(int j2 = j-filterNeighbor; j2<=j+filterNeighbor ; j2++){
                        r += image.pixelColor(i2,j2).red();
                    }
                }
                r/=pow(2*filterNeighbor+1,2);
                newImage.setPixelColor(i,j,QColor(r,r,r));
            }
        }
    }
    image = newImage;
    update();
    emit image_changed(image);
    qDebug() << "function meanFilter ended";
}
void PaintWidget::medianFilter(){
    qDebug() << "function medianFilter called";
    QImage newImage = image;
    for(int i = filterNeighbor; i<=400-filterNeighbor ; i++){
        for(int j = filterNeighbor; j<=400-filterNeighbor ; j++){
            if(i>startPoint.x()&&j>startPoint.y() && i<lastPoint.x()&&j<lastPoint.y()){
                //qDebug() << i << "|" << j;
                QVector<int> ndgList = QVector<int>();
                for(int i2 = i-filterNeighbor; i2<=i+filterNeighbor ; i2++){
                    for(int j2 = j-filterNeighbor; j2<=j+filterNeighbor ; j2++){
                        ndgList.append(image.pixelColor(i2,j2).red());
                        std::sort(ndgList.begin(), ndgList.end());
                    }
                }
                int indice= (1+filterNeighbor)*2*filterNeighbor;
                newImage.setPixelColor(i,j,QColor(ndgList[indice],ndgList[indice],ndgList[indice]));
            }
        }
    }
    image = newImage;
    update();
    emit image_changed(image);
    qDebug() << "function medianFilter ended";
}

void PaintWidget::setFilterNeighbor(int n)
{
    this->filterNeighbor=n;
}

void PaintWidget::setMountainHeight(int h)
{
    this->mountainHeight=h;
}
void PaintWidget::setMountainWidth(int w)
{
    this->mountainWidth=w;
}
