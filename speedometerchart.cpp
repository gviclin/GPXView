#include "speedometerchart.h"

#include <QtCharts/QAbstractAxis>
#include <QtCharts/QSplineSeries>
#include <QtCharts/QLineSeries>
#include <QtCharts/QValueAxis>
//#include <QtCore/QRandomGenerator>
#include <QtCore/QDebug>

SpeedometerChart::SpeedometerChart(QGraphicsItem *parent, Qt::WindowFlags wFlags):
    QChart(QChart::ChartTypeCartesian, parent, wFlags),
    m_series(0),
    m_axisX(new QValueAxis()),
    m_axisY(new QValueAxis()),
    m_step(0),
    m_x(0),
    m_y(0)
{
    QObject::connect(&m_timer, &QTimer::timeout, this, &SpeedometerChart::handleTimeout);
    m_timer.setInterval(150);

    m_series = new QSplineSeries(this);
    QPen green(Qt::red);
    green.setWidth(3);
    m_series->setPen(green);
    //m_series->append(m_x, m_y);

    addSeries(m_series);

    addAxis(m_axisX,Qt::AlignBottom);
    addAxis(m_axisY,Qt::AlignLeft);
    m_series->attachAxis(m_axisX);
    m_series->attachAxis(m_axisY);
    m_axisX->setTickCount(5);
    m_axisX->setRange(0,300);
    m_axisY->setRange(0, 60);

    m_timer.start();


}

SpeedometerChart::~SpeedometerChart()
{

}

void SpeedometerChart::addDatas(QList<QPointF>& list)
{
    m_list =list;

}

void SpeedometerChart::handleTimeout()
{
    //qreal x = plotArea().width() / m_axisX->tickCount();
    //qreal y = (m_axisX->max() - m_axisX->min()) / m_axisX->tickCount();

   // m_y += (qrand()%5-2);
//    m_y = QRandomGenerator::global()->bounded(5) - 2.5;

   // scroll(x, 0);
    //qDebug()<< m_x << " : " << m_y;

    m_x += 1;
    m_y = m_list.takeFirst().y();

    m_series->append(m_x, m_y);

    //qDebug()<< "count : " << m_series->points().size();
    for (int i = 0; i < m_series->count();i++)
    {
        QPointF point = m_series->at(i);
        m_series->replace(i,point.x(),point.y()+(qrand()%3-1));
    }
    //qDebug()<< " ";

 /*   if (m_x == 100)
        m_timer.stop();*/
}


