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

    enum LineMode {
        lmFree,
        lmFollowCursor,
        lmTracing
    };
    Q_ENUM(LineMode)

    explicit CrossLine(QCustomPlot *parentPlot, QCPGraph *targetGraph = Q_NULLPTR);
    ~CrossLine();

    void setLineMode(LineMode mode);
    LineMode lineMode() const { return mLineMode; }

    void setLineVisible(Qt::Orientation orientation, bool visible = true);
    bool lineVisible(Qt::Orientation orientation);

    void setGraph(QCPGraph *graph);

public Q_SLOTS:
    void onMouseMoved(QMouseEvent *event);
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

    LineMode mLineMode;
    double mKey, mValue;

protected:
    void updateTracer();
    void updateHLine(double value);
    void updateVLine(double key);
};

#endif // CROSSLINE_H
