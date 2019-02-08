#ifndef CAVERAGEFACTORY_H
#define CAVERAGEFACTORY_H

#include <QList>

#define NO_VALUE 0xFFFFFFFF
#define AVG 200

class QPointF;

class CAverageFactory
{
public:
    CAverageFactory();

    void AddPoint(QPointF point);

    const QVector<QPointF>& GetAvgVector();
    const QList<QPointF>& GetAvgList();
    const QMap<int, QPointF>& GetAvgMap();

private:    
    QList<QPointF> list;

    QList<QPointF> averageList;
    QVector<QPointF> averageVector;
    QMap<int, QPointF> averageMap;

    qreal qAvg;

};

#endif // CAVERAGEFACTORY_H
