#include "glcontainer.h"

GLContainer::GLContainer(QWidget *parent)
    : QWidget{parent}
{
    glWidget = new GLWidget;

    xSlider = createSlider();
    ySlider = createSlider();
    zSlider = createSlider();


    //A completer, connecter les sliders de cette classe avec le glWidget pour mettre à jour la rotation
    QObject::connect(xSlider, &QSlider::valueChanged, glWidget, &GLWidget::setXRotation);
    QObject::connect(ySlider, &QSlider::valueChanged, glWidget, &GLWidget::setYRotation);
    QObject::connect(zSlider, &QSlider::valueChanged, glWidget, &GLWidget::setZRotation);
    // et inversement
    QObject::connect(glWidget,&GLWidget::setXRotation_signal, xSlider, &QSlider::setValue);
    QObject::connect(glWidget,&GLWidget::setYRotation_signal, ySlider, &QSlider::setValue);
    QObject::connect(glWidget,&GLWidget::setZRotation_signal, zSlider, &QSlider::setValue);

    QVBoxLayout *mainLayout = new QVBoxLayout;

    mainLayout->addWidget(glWidget);
    mainLayout->addWidget(xSlider);
    mainLayout->addWidget(ySlider);
    mainLayout->addWidget(zSlider);

    setLayout(mainLayout);

    glWidget->setXRotation(90 * 16);
    glWidget->setYRotation(0);
    glWidget->setZRotation(0);
}

QSlider *GLContainer::createSlider()
{
    QSlider *slider = new QSlider(Qt::Horizontal);
    slider->setRange(0, 360 * 16);
    slider->setSingleStep(16);
    slider->setPageStep(15 * 16);
    slider->setTickInterval(15 * 16);
    slider->setTickPosition(QSlider::TicksRight);
    return slider;
}
