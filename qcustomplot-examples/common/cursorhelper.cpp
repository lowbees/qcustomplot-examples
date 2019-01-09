#include "cursorhelper.h"

CursorHelper *CursorHelper::mInstance = Q_NULLPTR;

CursorHelper *CursorHelper::instance()
{
    if (Q_NULLPTR == mInstance)
        mInstance = new CursorHelper;
    return mInstance;
}

void CursorHelper::release()
{
    delete mInstance;
    mInstance = Q_NULLPTR;
}

void CursorHelper::setCursor(QObject *item, const QCursor &cursor)
{
    if (Q_NULLPTR == item)
        return;
    mCursors[item] = cursor;
    connect(item, SIGNAL(destroyed(QObject*)), this, SLOT(remove(QObject*)));
}

QCursor CursorHelper::cursor(QObject *item)
{
    if (Q_NULLPTR == item || !mCursors.contains(item))
        return QCursor();
    return mCursors[item];
}

void CursorHelper::remove(QObject *item)
{
    mCursors.remove(item);
}

CursorHelper::CursorHelper()
{

}

CursorHelper::~CursorHelper()
{

}
