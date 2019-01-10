#ifndef CURSORHELPER_H
#define CURSORHELPER_H

#include <QObject>
#include <QCursor>
#include <QHash>

class CursorHelper : public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY(CursorHelper)
public:
    static CursorHelper *instance();
    static void release();

    void setCursor(QObject *item, const QCursor &cursor);
    QCursor cursor(QObject *item);


public Q_SLOTS:
    void remove(QObject *item);

private:
    CursorHelper();
    ~CursorHelper();
private:
    static CursorHelper *mInstance;
    QHash<QObject*, QCursor> mCursors;
};

#endif // CURSORHELPER_H
