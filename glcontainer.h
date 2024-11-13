#ifndef GLCONTAINER_H
#define GLCONTAINER_H

#include <QWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QSlider>

#include "glwidget.h"

class GLContainer : public QWidget
{
    Q_OBJECT
public:
    explicit GLContainer(QWidget *parent = nullptr);
    GLWidget *glWidget;

    QSlider *createSlider();
    QSlider *xSlider;
    QSlider *ySlider;
    QSlider *zSlider;
signals:

};

#endif // GLCONTAINER_H
