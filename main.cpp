#include "mainwindow.h"
#include "loginwindow.h"
#include <QApplication>
#include <QCoreApplication>
#include <QtCore>
#include <QtSql>
#include <QtDebug>
#include <QDialog>
#include <QStyle>
#include <QDesktopWidget>

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    MainWindow w;
    w.show();


    return a.exec();
}
