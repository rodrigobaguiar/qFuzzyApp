QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

CONFIG += c++11

RC_ICONS = qFuzzyAppIcon.ico

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    CAboutScreen.cpp \
    CColumnNames.cpp \
    CItemNames.cpp \
    CRowNames.cpp \
    main.cpp \
    CMainWindow.cpp \
    qcustomplot.cpp

HEADERS += \
    CAboutScreen.h \
    CColumnNames.h \
    CItemNames.h \
    CMainWindow.h \
    CRowNames.h \
    qcustomplot.h

FORMS += \
    CAboutScreen.ui \
    CItemNames.ui \
    CMainWindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
