#include "mainwindow.h"
#include "../common/customplot.h"
#include "../common/crossline.h"

#include <QDebug>
#include <QVBoxLayout>
#include <QComboBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    CustomPlot *customPlot = new CustomPlot;
    QComboBox *comboBox = new QComboBox;
    comboBox->addItems(QStringList() << "Free" << "Follow Cursor" << "Tracing");

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(comboBox);
    layout->addWidget(customPlot);

    QWidget *widget = new QWidget;

    widget->setLayout(layout);
    setCentralWidget(widget);

    customPlot->addGraph(customPlot->xAxis, customPlot->yAxis);
    customPlot->graph()->setPen(QPen(Qt::blue));
    customPlot->graph()->setBrush(QBrush(QColor(0, 0, 255, 20)));

    QVector<double> x(251), y0(251);
    for (int i = 0; i < 251; ++i) {
        x[i] = i;
        y0[i] = qExp(-i / 150.0) * qCos(i / 10.0);
    }
    customPlot->graph()->setData(x, y0);
    customPlot->graph()->rescaleAxes();
    customPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectItems);

    CrossLine *crossLine = new CrossLine(customPlot, customPlot->graph());
    connect(comboBox, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),  [crossLine](int index) {
        crossLine->setLineMode(CrossLine::LineMode(index));
    });
    connect(customPlot, SIGNAL(itemMoved(QCPAbstractItem*,QMouseEvent*)),
            crossLine, SLOT(onItemMoved(QCPAbstractItem*,QMouseEvent*)));

    // we don't want to drag when item is selected.
    connect(customPlot, &QCustomPlot::mousePress, [customPlot](QMouseEvent *event) {
        if (customPlot->itemAt(event->localPos()) && !customPlot->selectedItems().isEmpty())
            customPlot->setInteractions(QCP::iRangeZoom | QCP::iSelectItems);
    });
    connect(customPlot, &QCustomPlot::mouseRelease, [customPlot](){
        customPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectItems);
    });

    showMaximized();
}

MainWindow::~MainWindow()
{
}
