QT       += core gui multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    confirmpage.cpp \
    main.cpp \
    mainwindow.cpp \
    openprothread.cpp \
    picanimationwgt.cpp \
    picbutton.cpp \
    picshow.cpp \
    picstatebtn.cpp \
    previewlistitem.cpp \
    previewlistwgt.cpp \
    prosetpage.cpp \
    protree.cpp \
    protreeitem.cpp \
    protreethread.cpp \
    protreewidget.cpp \
    removeprodialog.cpp \
    slideshow.cpp \
    wizard.cpp

HEADERS += \
    confirmpage.h \
    const.h \
    mainwindow.h \
    openprothread.h \
    picanimationwgt.h \
    picbutton.h \
    picshow.h \
    picstatebtn.h \
    previewlistitem.h \
    previewlistwgt.h \
    prosetpage.h \
    protree.h \
    protreeitem.h \
    protreethread.h \
    protreewidget.h \
    removeprodialog.h \
    slideshow.h \
    wizard.h

FORMS += \
    confirmpage.ui \
    mainwindow.ui \
    picshow.ui \
    prosetpage.ui \
    protree.ui \
    removeprodialog.ui \
    slideshow.ui \
    wizard.ui

#  设置程序窗口图标
RC_ICONS = "res/icon/bulm.ico"

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    res.qrc
