#include "chart.h"
#include "chartview.h"
#include "cgpxtools.h"

#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtCore/QtMath>
#include <cstdlib>
#include <QtCharts/QLineSeries>
#include <QtCharts/QValueAxis>

QT_CHARTS_USE_NAMESPACE

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QList<CData> list;
    CGpxTools tools;
    tools.GetGPXData("../GPXView/Sortie_v_lo_le_midi.gpx",list);


    QLineSeries *series = new QLineSeries();
    QLineSeries *seriesHR = new QLineSeries();
    QList<CData>::iterator it;

    for (it = list.begin(); it != list.end(); ++it) {
        CData data = *it;
        QPointF p((qreal) data.distance/1000, data.speed);
        QPointF pHR((qreal) data.distance/1000, data.bpm);
        //QPointF p((qreal) i, qSin(M_PI / 50 * i) * 100);
        //p.ry() += std::rand()/((RAND_MAX)/50);
        *series << p;
        *seriesHR <<pHR;
    }

    // Customize color
    QPen pen(QColor(Qt::blue));
    pen.setWidth(1);
    series->setPen(pen);

    QPen pen2(QColor(Qt::red));
    pen2.setWidth(1);
    seriesHR->setPen(pen2);

    Chart *chart = new Chart();
   // chart->addSeries(series);
    chart->addSeries(seriesHR);
    chart->setTitle("Zoom in/out example");
    chart->setAnimationOptions(QChart::SeriesAnimations);
    chart->legend()->hide();
    //chart->createDefaultAxes();

    ChartView *chartView = new ChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);

    QValueAxis *axisX = new QValueAxis;
    axisX->setRange(0, 50);
    axisX->setTickCount(10);
    axisX->setLabelFormat("%.0d");
    chartView->chart()->setAxisX(axisX, seriesHR);

    QValueAxis *axisY = new QValueAxis;
    axisY->setRange(50, 200);
    axisY->setTickCount(10);
    axisY->setLabelFormat("%.0d");
    chartView->chart()->setAxisY(axisY, seriesHR);

    QMainWindow window;
    window.setCentralWidget(chartView);
    window.resize(1024, 800);
    window.grabGesture(Qt::PanGesture);
    window.grabGesture(Qt::PinchGesture);
    window.show();

    return a.exec();
}
