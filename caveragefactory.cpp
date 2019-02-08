#include "caveragefactory.h"

#include <QPointF>
#include <QtCore/QDebug>


CAverageFactory::CAverageFactory()
    :qAvg(0)
{

}

void CAverageFactory::AddPoint(QPointF point)
{
    qDebug()<< "AddPoint : " << point.x() << " : " << point.y();

    //calcul de la moyenne totale
    qAvg = (qAvg*list.size() + point.y())/(list.size()+1);
    //qDebug()<< "Total avg :" << qAvg;

    list.push_back(point);
/*
    for (QList<QPointF>::iterator it = list.begin(); it != list.end();it++)
    {
        qDebug()<< "List : " << (*it).x() << " : "  << (*it).y();
    }*/

    unsigned int iLastIndex = point.x();

    averageList.clear();

    averageList.push_back(QPointF(0, point.y()));

    int iTemp;
    qreal avg_y;
    QList<QPointF>::reverse_iterator r_itList;

    //calcul de la moyenne pour chaque intervalle
    for (unsigned short Ind = 2; Ind <=(AVG-20); Ind=1.2*Ind+15)
    {
        if (iLastIndex < Ind)
        {
            //données insuffuisante pour effectuer la moyenne
            /*pointTemp.rx() = byInd;
            pointTemp.ry() = NO_VALUE;   */
           //qDebug()<< "Avg " << Ind << " : no_enough_data";
        }
        else
        {
            iTemp =0;
            avg_y=0;
            for (r_itList = list.rbegin();iTemp < Ind;r_itList++)
            {
                avg_y += (*r_itList).y();
                iTemp++;
            }
            avg_y/=iTemp;
            //qDebug()<< "Avg " << Ind << " : " << avg_y;
            averageList.push_back(QPointF(Ind, avg_y));
        }

    }

   if (iLastIndex >= AVG)
   {
       averageList.push_back(QPointF(AVG, qAvg));
   }

    return;
}

const QList<QPointF>& CAverageFactory::GetAvgList()
{
    return averageList;
}

const QVector<QPointF>& CAverageFactory::GetAvgVector()
{
    return averageVector;
}

const QMap<int, QPointF>& CAverageFactory::GetAvgMap()
{
    return averageMap;
}


