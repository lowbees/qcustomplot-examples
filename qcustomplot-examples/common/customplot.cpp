#include "customplot.h"
#include "cursorhelper.h"

CustomPlot::CustomPlot(QWidget *parent)
    : QCustomPlot(parent)
{
    setSelectionTolerance(6);
}

CustomPlot::~CustomPlot()
{
    CursorHelper::release();
}


void CustomPlot::mousePressEvent(QMouseEvent *event)
{
    QCPAbstractItem *item = itemAt(event->localPos());
    if (item && item->selectable()) {
        item->setSelected(true);
        item->layer()->replot();
    }
    QCustomPlot::mousePressEvent(event);
}

void CustomPlot::mouseReleaseEvent(QMouseEvent *event)
{
    QCustomPlot::mouseReleaseEvent(event);
    foreach (QCPAbstractItem *item, selectedItems()) {
        item->setSelected(false);
        item->layer()->replot();
    }
}

void CustomPlot::mouseMoveEvent(QMouseEvent *event)
{
    QCustomPlot::mouseMoveEvent(event);

    QPointF localPos = event->localPos();
    QCPAbstractItem *item = itemAt(localPos);

    QCPAbstractItem *selectedItem = (selectedItems().isEmpty() ? Q_NULLPTR : selectedItems().at(0));

    // set item cursor
    if (item || selectedItem) {
        QCPAbstractItem *obj = selectedItem ? selectedItem : item;
        QCursor itemCursor = CursorHelper::instance()->cursor(obj);
        if (itemCursor.shape() != cursor().shape())
            setCursor(itemCursor);
    } else {
        if (cursor().shape() != Qt::ArrowCursor)
            unsetCursor();
    }

    if (selectedItem && (event->buttons() & Qt::LeftButton)) {
        Q_EMIT itemMoved(selectedItem, event);
        selectedItem->layer()->replot();
    }
}


QSize CustomPlot::sizeHint() const
{
    return QSize(400, 300);
}
