#ifndef SPEEDOMETERMANAGER_H
#define SPEEDOMETERMANAGER_H

#include <QList>
#include <QtCore/QTimer>

#include "caveragefactory.h"

class SpeedometerChart;
class QcGaugeWidget;
class QcNeedleItem;

class SpeedometerManager  : public QObject
{
    Q_OBJECT
public:
    SpeedometerManager();

    void addDatas(QList<QPointF>& list, QWidget* parent);

public slots:
    void handleTimeout();

private:
    QList<QPointF> m_list;
    QTimer m_timer;
    int m_t;

    CAverageFactory m_factory;

    //graph
    SpeedometerChart* m_pChart;
    QWidget * m_pChartWindow;

    //speedometer
    QcGaugeWidget* m_pSpeedGauge;
    QcNeedleItem* m_pSpeedNeedle;
    QcNeedleItem* m_pSpeedNeedle1;
    QcNeedleItem* m_pSpeedNeedle2;
    QcNeedleItem* m_pSpeedNeedle3;
    QcNeedleItem* m_pSpeedNeedleAvg;
};

#endif // SPEEDOMETERMANAGER_H
