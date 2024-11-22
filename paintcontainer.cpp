#include "paintcontainer.h"
#include <QPushButton>
PaintContainer::PaintContainer(QWidget *parent)
    : QWidget{parent}
{
    paintWidget = new PaintWidget;
    currentMode = new QLabel;
    currentMode->setText("None");
    QPushButton *draw_button = new QPushButton("Draw", this);
    QPushButton *square_button = new QPushButton("Square", this);

    QVBoxLayout *mainLayout = new QVBoxLayout;

    mainLayout->addWidget(paintWidget);
    mainLayout->addWidget(currentMode);
    mainLayout->addWidget(draw_button);
    mainLayout->addWidget(square_button);

    setLayout(mainLayout);

    connect(draw_button, &QPushButton::released, this, &PaintContainer::setDraw);
    connect(square_button, &QPushButton::released, this, &PaintContainer::setDrawSquare);
}

void PaintContainer::setDraw(){
    paintWidget->state=Action::Draw;
    currentMode->setText("Draw");
}
void PaintContainer::setDrawSquare(){
    paintWidget->state=Action::DrawSquare;
    currentMode->setText("DrawSquare");
}
