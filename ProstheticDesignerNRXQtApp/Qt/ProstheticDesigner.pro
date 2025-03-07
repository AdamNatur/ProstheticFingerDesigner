QT       += core gui quick openglwidgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets openglwidgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    mechanismform.cpp \
    plateform.cpp \
    ringform.cpp

HEADERS += \
    mainwindow.h \
    mechanismform.h \
    objloader.h \
    plateform.h \
    ringform.h

FORMS += \
    mainwindow.ui \
    mechanismform.ui \
    plateform.ui \
    ringform.ui

TRANSLATIONS += \
    ProstheticDesigner_en_US.ts
CONFIG += lrelease
CONFIG += embed_translations


# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
