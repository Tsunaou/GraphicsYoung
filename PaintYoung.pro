#-------------------------------------------------
#
# Project created by QtCreator 2018-10-09T17:00:20
#
#-------------------------------------------------

QT       += core gui

#add the 2 lines to tell complier to use openGL @young
QT +=core gui opengl
LIBS+=-lopengl32 -lglu32


greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = PaintYoung
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11

SOURCES += \
        main.cpp \
        mainwindow.cpp \
    Canvas_GL.cpp \
    Figure.cpp \
    Line.cpp \
    ColorPanel.cpp \
    CycleController.cpp \
    EllipseController.cpp \
    FigureController.cpp \
    LineController.cpp \
    Cycle.cpp \
    Ellipse.cpp \
    Point.cpp \
    Polygon.cpp \
    PolygonController.cpp \
    CurveController.cpp \
    Cavans_3DGL.cpp \
    Curve.cpp

HEADERS += \
        mainwindow.h \
    Canvas_GL.h \
    Figure.h \
    Line.h \
    ColorPanel.h \
    constParams.h \
    CycleController.h \
    EllipseController.h \
    FigureController.h \
    LineController.h \
    Cycle.h \
    Ellipse.h \
    Point.h \
    Polygon.h \
    PolygonController.h \
    CurveController.h \
    Cavans_3DGL.h \
    Curve.h

FORMS += \
        mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    image.qrc

RC_FILE = YoungPaint.rc
