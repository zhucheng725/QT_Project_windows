#-------------------------------------------------
#
# Project created by QtCreator 2021-03-24T15:50:47
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = OpenCam
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


SOURCES += \
        main.cpp \
        widget.cpp \
        tinyxml2.cpp

HEADERS += \
        widget.h \
        tinyxml2.h

FORMS += \
        widget.ui

## debug mode:
#INCLUDEPATH += D:\procedure\opencv\build\install\include \
#               D:\procedure\opencv\build\install\include\opencv2
#LIBS += -LD:\procedure\opencv\build\install\x64\vc14\lib -lopencv_core450d -lopencv_calib3d450d -lopencv_imgcodecs450d -lopencv_highgui450d -lopencv_imgproc450d -lopencv_videoio450d -lopencv_dnn450d -lopencv_features2d450d -lopencv_flann450d -lopencv_gapi450d -lopencv_ml450d -lopencv_objdetect450d -lopencv_photo450d -lopencv_stitching450d -lopencv_video450d
#DEPENDPATH += D:\procedure\opencv\build\install\include

# release mode:
INCLUDEPATH += D:\procedure\opencv\build\install\include \
               D:\procedure\opencv\build\install\include\opencv2
LIBS += -LD:\procedure\opencv\build\install\x64\vc14\lib -lopencv_core450 -lopencv_calib3d450 -lopencv_imgcodecs450 -lopencv_highgui450 -lopencv_imgproc450 -lopencv_videoio450 -lopencv_dnn450 -lopencv_features2d450 -lopencv_flann450 -lopencv_gapi450 -lopencv_ml450 -lopencv_objdetect450 -lopencv_photo450 -lopencv_stitching450 -lopencv_video450
DEPENDPATH += D:\procedure\opencv\build\install\include

