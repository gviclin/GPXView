#ifndef SPEEDOMETERMANAGER_H
#define SPEEDOMETERMANAGER_H

#include <QList>

class SpeedometerManager
{
public:
    SpeedometerManager();

    void addDatas(QList<QPointF>& list, QWidget* parent);

private:
    QList<QPointF> m_list;
};

#endif // SPEEDOMETERMANAGER_H
