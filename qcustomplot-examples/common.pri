QT       += core gui printsupport
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

PROJECT_BINDIR = $$PWD/bin
PROJECT_LIBDIR = $$PWD/lib
PROJECT_COMMON = $$PWD/common

HEADERS += $$PROJECT_LIBDIR/qcustomplot.h \
                     $$PROJECT_COMMON/cursorhelper.h \
                     $$PROJECT_COMMON/customplot.h \
                     $$PROJECT_COMMON/crossline.h

SOURCES += $$PROJECT_LIBDIR/qcustomplot.cpp \
                     $$PROJECT_COMMON/cursorhelper.cpp \
                     $$PROJECT_COMMON/customplot.cpp \
                     $$PROJECT_COMMON/crossline.cpp

CONFIG += debug_and_release build_all
