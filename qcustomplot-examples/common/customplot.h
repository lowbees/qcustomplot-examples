#ifndef CUSTOMPLOT_H
#define CUSTOMPLOT_H

#include "../lib/qcustomplot.h"

class CustomPlot : public QCustomPlot
{
    Q_OBJECT
public:
    CustomPlot(QWidget *parent = Q_NULLPTR);

Q_SIGNALS:
    void sizeChanged(const QSize &size);
    void itemMoved(QCPAbstractItem *item, QMouseEvent *event);

    // QWidget interface
protected:
    virtual void mousePressEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    virtual void mouseReleaseEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    virtual void mouseMoveEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    virtual void resizeEvent(QResizeEvent *event) Q_DECL_OVERRIDE;

    // QWidget interface
public:
    virtual QSize sizeHint() const Q_DECL_OVERRIDE;

};

#endif // CUSTOMPLOT_H
