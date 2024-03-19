QT       += core
QT       += gui
QT       += network
QT       += multimedia multimediawidgets
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    addphoto.cpp \
    chat.cpp \
    logout.cpp \
    main.cpp \
    login.cpp \
    oflinechat.cpp \
    serverthread.cpp \
    signup.cpp

HEADERS += \
    addphoto.h \
    chat.h \
    listviewdelegate.h \
    login.h \
    logout.h \
    oflinechat.h \
    serverthread.h \
    signup.h

FORMS +=  \
    addphoto.ui \
    chat.ui \
    login.ui \
    logout.ui \
    oflinechat.ui \
    signup.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resources.qrc
