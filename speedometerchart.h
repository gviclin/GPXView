#ifndef SPEEDOMETERCHART_H
#define SPEEDOMETERCHART_H

#include <QtCharts/QChart>
#include <QtCore/QTimer>

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

    void addDatas(QList<QPointF>& list);

public slots:
    void handleTimeout();

private:
    QTimer m_timer;
    QLineSeries *m_series;
    QStringList m_titles;
    QValueAxis *m_axisX;
    QValueAxis *m_axisY;
    qreal m_step;
    qreal m_x;
    qreal m_y;

    QList<QPointF> m_list;
};

#endif // SPEEDOMETERCHART_H
