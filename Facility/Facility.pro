QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17
INCLUDEPATH += /usr/include/opencv4
LIBS += `pkg-config --cflags --libs opencv4`

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    afficheurdescripteur.cpp \
    biblioinputdialog.cpp \
    descripteurdialog.cpp \
    gallerie.cpp \
    loginwindow.cpp \
    main.cpp \
    mainwindow.cpp \
    ../routines/Bibliotheque.cpp \
    ../routines/Descripteur.cpp \
    ../routines/Image.cpp \
    ../routines/Utilisateur.cpp \
    processingwindow.cpp

HEADERS += \
    afficheurdescripteur.h \
    biblioinputdialog.h \
    descripteurdialog.h \
    gallerie.h \
    loginwindow.h \
    mainwindow.h \
    ../routines/Bibliotheque.h \
    ../routines/Descripteur.h \
    ../routines/Image.h \
    ../routines/Utilisateur.h \
    processingwindow.h

FORMS += \
    afficheurdescripteur.ui \
    descripteurdialog.ui \
    loginwindow.ui \
    mainwindow.ui \
    processingwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    FacilityLogo.qrc
