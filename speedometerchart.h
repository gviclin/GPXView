#ifndef SPEEDOMETERCHART_H
#define SPEEDOMETERCHART_H

#include <QtCharts/QChart>

QT_CHARTS_BEGIN_NAMESPACE
class QLineSeries;
class QValueAxis;

QT_CHARTS_END_NAMESPACE

QT_CHARTS_USE_NAMESPACE

class SpeedometerChart: public QChart
{
    Q_OBJECT
public:
    SpeedometerChart(QGraphicsItem *parent = 0, Qt::WindowFlags wFlags = 0);
    virtual ~SpeedometerChart();

    void setList(const QList<QPointF>& list);

private:
    QLineSeries *m_series;
    QStringList m_titles;
    QValueAxis *m_axisX;
    QValueAxis *m_axisY;
};

#endif // SPEEDOMETERCHART_H
