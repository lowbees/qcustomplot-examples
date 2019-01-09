#include "crossline.h"
#include "cursorhelper.h"
#include "../lib/qcustomplot.h"
#include <QDebug>
#include <QMouseEvent>

CrossLine::CrossLine(QCustomPlot *parentPlot, QCPGraph *targetGraph)
    : QObject(parentPlot)
    , mParentPlot(parentPlot)
    , mHLine(new QCPItemLine(parentPlot))
    , mVLine(new QCPItemLine(parentPlot))
    , mHText(new QCPItemText(parentPlot))
    , mVText(new QCPItemText(parentPlot))
    , mTracer(new QCPItemTracer(parentPlot))
//    , mTracerText(new QCPItemText(parentPlot))
//    , mTracerArrow(new QCPItemCurve(parentPlot))
    , mTargetGraph(Q_NULLPTR)
{
    const QString layer(QStringLiteral("overlay"));
    const QMargins margins(6, 6, 6, 6);

    targetGraph = targetGraph ? targetGraph : parentPlot->graph();
    setGraph(targetGraph);

    mTracer->setBrush(Qt::red);
    mTracer->setInterpolating(true);
    mTracer->setStyle(QCPItemTracer::tsCircle);


    mHLine->start->setType(QCPItemPosition::ptAbsolute);
    mHLine->end->setType(QCPItemPosition::ptAbsolute);
    mVLine->start->setType(QCPItemPosition::ptAbsolute);
    mVLine->end->setType(QCPItemPosition::ptAbsolute);

    mHLine->setLayer(layer);
    mVLine->setLayer(layer);
    mTracer->setLayer(layer);

    mHText->setPadding(margins);
    mHText->setLayer(layer);
    mVText->setPadding(margins);
    mVText->setLayer(layer);

    connect(parentPlot, SIGNAL(afterReplot()), this, SLOT(update()));
}

CrossLine::~CrossLine()
{

}

void CrossLine::setGraph(QCPGraph *graph)
{
    if (graph == Q_NULLPTR) {
        qDebug() << "graph is null";
        return;
    }
    if (mTargetGraph == graph)
        return;

    // don't use rangeChanged signal because rangeChanged signal emit early than plot replot
    // use afterReplot signal instead.
    // disconnect old signals and slots first
//    if (mTargetGraph) {
//        disconnect(mTargetGraph->keyAxis(), SIGNAL(rangeChanged(QCPRange)), this, SLOT(update()));
//        disconnect(mTargetGraph->valueAxis(), SIGNAL(rangeChanged(QCPRange)), this, SLOT(update()));
//    } else {
//        connect(graph->keyAxis(), SIGNAL(rangeChanged(QCPRange)), this, SLOT(update()));
//        connect(graph->valueAxis(), SIGNAL(rangeChanged(QCPRange)), this, SLOT(update()));
//    }

    mTracer->setGraph(graph);
    mTracer->setGraphKey(0);

    CursorHelper *helper = CursorHelper::instance();

    if (graph->keyAxis()->orientation() == Qt::Horizontal) {
        helper->setCursor(mVLine, QCursor(Qt::SizeHorCursor));
        helper->setCursor(mHLine, QCursor(Qt::SizeVerCursor));

        mHText->position->setParentAnchor(mHLine->start);
        mVText->position->setParentAnchor(mVLine->end);
    }
    else {
        helper->setCursor(mVLine, QCursor(Qt::SizeVerCursor));
        helper->setCursor(mHLine, QCursor(Qt::SizeHorCursor));

        mHText->position->setParentAnchor(mHLine->end);
        mVText->position->setParentAnchor(mVLine->start);
    }

    if (mTargetGraph)  // don't update first time for some cases
        update();
    mTargetGraph = graph;
}

void CrossLine::onItemMoved(QCPAbstractItem *item, QMouseEvent *event)
{
    if (item == Q_NULLPTR || (item != mVLine && item != mHLine))
        return;

    QCPAxis *keyAxis = mTargetGraph->keyAxis();
    QPointF localPos = event->localPos();
    double key;

    if (keyAxis->orientation() == Qt::Horizontal)
        key = keyAxis->pixelToCoord(localPos.x());
    else
        key = keyAxis->pixelToCoord(localPos.y());

    mTracer->setGraphKey(key);
    update();
}

void CrossLine::update()
{
    Q_ASSERT(mTargetGraph);

    QCPAxis *keyAxis = mTargetGraph->keyAxis();

    QCPAxisRect *axisRect = keyAxis->axisRect();
    QRect rect = axisRect->rect();
    QPointF center = axisRect->center();

    mTracer->updatePosition();
    QPointF pixel = mTracer->position->pixelPosition();

    double key, value;
    mTargetGraph->pixelsToCoords(pixel, key, value);

    mHText->setText(QString::number(value, 'f', 2));
    mVText->setText(QString::number(key, 'f', 2));

    // make mVLine always vertical to the keyAxis
    if (keyAxis->orientation() == Qt::Horizontal) {
        mHLine->start->setCoords(rect.left(), pixel.y());
        mHLine->end->setCoords(rect.right(), pixel.y());
        mVLine->start->setCoords(pixel.x(), rect.top());
        mVLine->end->setCoords(pixel.x(), rect.bottom());

        Qt::Alignment alignment = (pixel.y() >= center.y() ? Qt::AlignBottom : Qt::AlignTop) | Qt::AlignLeft;
        mHText->setPositionAlignment(alignment);
        alignment = (pixel.x() >= center.x() ? Qt::AlignRight : Qt::AlignLeft) | Qt::AlignBottom;
        mVText->setPositionAlignment(alignment);
    } else {
        mHLine->start->setCoords(pixel.x(), rect.top());
        mHLine->end->setCoords(pixel.x(), rect.bottom());
        mVLine->start->setCoords(rect.left(), pixel.y());
        mVLine->end->setCoords(rect.right(), pixel.y());

        Qt::Alignment alignment = (pixel.x() >= center.x() ? Qt::AlignRight : Qt::AlignLeft) | Qt::AlignBottom;
        mHText->setPositionAlignment(alignment);
        alignment = (pixel.y() >= center.y() ? Qt::AlignBottom : Qt::AlignTop) | Qt::AlignLeft;
        mVText->setPositionAlignment(alignment);
    }

    // we just want replot individual by use overlay layer
    mParentPlot->layer("overlay")->replot();
}
