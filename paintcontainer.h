#ifndef PAINTCONTAINER_H
#define PAINTCONTAINER_H

#include <QWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QSlider>

#include "paintwidget.h"

class PaintContainer : public QWidget
{
    Q_OBJECT
public:
    explicit PaintContainer(QWidget *parent = nullptr);

    PaintWidget *paintWidget;
    QLabel *currentMode;

signals:

};

#endif // PAINTCONTAINER_H
