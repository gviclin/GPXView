#include "speedometermanager.h"

#include <QtCore/QDebug>
#include <QtCharts/QChartView>
#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QWindow>

#include "speedometerchart.h"



SpeedometerManager::SpeedometerManager()
{

}

void SpeedometerManager::addDatas(QList<QPointF>& list, QWidget* parent)
{
    m_list = list;

    qDebug()<< " Size : " << m_list.size();
   /* QList<QPointF>::iterator it;
    for (it = m_list.begin();it!=m_list.end();++it)
    {
         qDebug()<< (*it).x() << " : " << (*it).y();
    }*/


    QMainWindow*  window = new QMainWindow(parent);
    SpeedometerChart* chart = new SpeedometerChart;
    chart->addDatas(list);
    chart->setTitle("Dynamic spline chart");
    chart->legend()->hide();
    chart->setAnimationOptions(QChart::AllAnimations);
    QChartView* chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);
    window->setWindowTitle("Speedometer");
    window->setCentralWidget(chartView);
    window->resize(1024, 800);
    window->show();


    //chartView.show();



}

