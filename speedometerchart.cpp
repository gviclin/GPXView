#include "speedometerchart.h"

#include <QtCharts/QAbstractAxis>
#include <QtCharts/QSplineSeries>
#include <QtCharts/QLineSeries>
#include <QtCharts/QValueAxis>
//#include <QtCore/QRandomGenerator>
#include <QtCore/QDebug>

#include "caveragefactory.h"


SpeedometerChart::SpeedometerChart(QGraphicsItem *parent, Qt::WindowFlags wFlags):
    QChart(QChart::ChartTypeCartesian, parent, wFlags),
    m_series(0),
    m_axisX(new QValueAxis()),
    m_axisY(new QValueAxis())
{

    m_series = new QLineSeries(this);
    QPen green(Qt::blue);
    green.setWidth(10);
    m_series->setPen(green);

    addSeries(m_series);

    addAxis(m_axisX,Qt::AlignBottom);
    addAxis(m_axisY,Qt::AlignLeft);
    m_series->attachAxis(m_axisX);
    m_series->attachAxis(m_axisY);
    m_axisX->setTickCount(5);
    m_axisX->setRange(0,AVG);
    m_axisY->setRange(90,180);



}

SpeedometerChart::~SpeedometerChart()
{

}


void SpeedometerChart::setList(const QList<QPointF>& list)
{
    m_series->clear();
    for (QList<QPointF>::const_iterator it = list.begin(); it != list.end();it++)
    {
        //qDebug()<< "Avg list : " << (*it).x() << " : "  << (*it).y();
        m_series->append((*it));
    }
   // qDebug()<<" ";


    //m_series->append(avglist);

/*    QList<QPointF>::const_iterator it;
    for (it = m_series->points().begin();it!=m_series->points().end();it++)
    {
        m_series->replace
    }*/



   // m_series->append(m_x, m_y);
/*
    //qDebug()<< "count : " << m_series->points().size();
    for (int i = 0; i < m_series->count();i++)
    {
        QPointF point = m_series->at(i);
        m_series->replace(i,point.x(),point.y()+(qrand()%3-1));
    }
    //qDebug()<< " ";
*/


 /*   if (m_x == 100)
        m_timer.stop();*/
}


