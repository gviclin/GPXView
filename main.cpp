#include "mainwidget.h"

#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtCore/QtMath>
#include <cstdlib>
#include <QDebug>

#include "speedometerchart.h"
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    MainWindow window;
    window.show();
/*
    QMainWindow*  window = new QMainWindow;
    SpeedometerChart *chart = new SpeedometerChart;
    chart->setTitle("Dynamic spline chart");
    chart->legend()->hide();
    chart->setAnimationOptions(QChart::AllAnimations);
    QChartView chartView(chart);
    chartView.setRenderHint(QPainter::Antialiasing);
    window->setWindowTitle("Speedometer");
    window->setCentralWidget(&chartView);
    window->resize(800, 600);
    window->show();*/



    return a.exec();
}



