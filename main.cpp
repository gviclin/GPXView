#include "mainwidget.h"

#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtCore/QtMath>
#include <cstdlib>

QT_CHARTS_USE_NAMESPACE

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    MainWidget w;
    w.resize(1024, 800);
    w.show();

    return a.exec();
}


