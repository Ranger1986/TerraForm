#include "paintwidget.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QString>
#include <QPainter>

PaintWidget::PaintWidget(QWidget *parent)
    : QWidget{parent}
{
    testLabel = new QLabel(this);
    testLabel->setText("PLOUP");
    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(testLabel);
    setLayout(mainLayout);
}
void PaintWidget::paintEvent(QPaintEvent *event){

    QPainter painter(this);
    //painter.drawRect(QRect(80,120,200,100));

}
