#ifndef CROSSLINE_H
#define CROSSLINE_H

#include <QObject>

class QCustomPlot;
class QCPItemLine;
class QCPItemText;
class QCPItemTracer;
class QCPItemCurve;
class QCPAbstractItem;
class QMouseEvent;
class QCPGraph;

class CrossLine : public QObject
{
    Q_OBJECT
public:
    explicit CrossLine(QCustomPlot *parentPlot, QCPGraph *targetGraph = Q_NULLPTR);
    ~CrossLine();

    void setLineVisible(Qt::Orientation orientation, bool visible = true);
    bool lineVisible(Qt::Orientation orientation);

    void setGraph(QCPGraph *graph);

public Q_SLOTS:
    void onItemMoved(QCPAbstractItem *item, QMouseEvent *event);
    void update();

protected:
    QCustomPlot *mParentPlot;
    QCPItemLine *mHLine, *mVLine;
    QCPItemText *mHText, *mVText;
    QCPItemTracer *mTracer;
    QCPItemText *mTracerText;
    QCPItemCurve *mTracerArrow;
    QCPGraph *mTargetGraph;
};

#endif // CROSSLINE_H
