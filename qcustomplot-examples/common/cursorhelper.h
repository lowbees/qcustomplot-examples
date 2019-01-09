#ifndef CURSORHELPER_H
#define CURSORHELPER_H

#include <QObject>
#include <QCursor>
#include <QMap>

class CursorHelper : public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY(CursorHelper)
public:
    static CursorHelper *instance();
    static void release();

    void setCursor(QObject *item, const QCursor &cursor);
    QCursor cursor(QObject *item);


private Q_SLOTS:
    void remove(QObject *item);

private:
    CursorHelper();
    ~CursorHelper();
private:
    static CursorHelper *mInstance;
    QMap<QObject*, QCursor> mCursors;
};

#endif // CURSORHELPER_H
