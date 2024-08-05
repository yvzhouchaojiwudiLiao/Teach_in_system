QT       += core gui
QT += multimedia multimediawidgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    camerawidget.cpp \
    main.cpp \
    mainwindow.cpp \
    videoplayer.cpp

HEADERS += \
    CameraWidget.h \
    mainwindow.h \
    videoplayer.h

FORMS += \
    mainwindow.ui

# 设置HKVISION库的路径
HKVISION_DIR = "C:\\Program Files\\HKVISION"
HKVISION_INCLUDE_DIR = $$HKVISION_DIR\\include
HKVISION_LIB_DIRS = $$HKVISION_DIR\\lib

# 包含库的头文件目录
INCLUDEPATH += $$HKVISION_INCLUDE_DIR

# 链接库的目录和库文件
LIBS += -L$$HKVISION_LIB_DIRS -lHCNetSDK -lPlayCtrl


# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    img.qrc
