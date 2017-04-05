#-------------------------------------------------
#
# Project created by QtCreator 2017-03-03T14:25:00
#
#-------------------------------------------------

QT       += core gui
QT       += sql
QT       += widgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Course_Work
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += main.cpp\
        mainwindow.cpp \
    loginwindow.cpp \
    addcontractor.cpp \
    interviewwindow.cpp \
    moveposotionwindow.cpp \
    addposition.cpp \
    addcontractortostaff.cpp \
    addabsencewindow.cpp

HEADERS  += mainwindow.h \
    loginwindow.h \
    addcontractor.h \
    interviewwindow.h \
    moveposotionwindow.h \
    addposition.h \
    addcontractortostaff.h \
    addabsencewindow.h

FORMS    += mainwindow.ui \
    loginwindow.ui \
    addcontractor.ui \
    interviewwindow.ui \
    moveposotionwindow.ui \
    addposition.ui \
    addcontractortostaff.ui \
    addabsencewindow.ui
