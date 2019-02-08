#include "speedometermanager.h"

#include <QtCore/QDebug>
#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QWindow>
#include <QVBoxLayout>
#include <QtCharts/QChartView>

#include "qcgaugewidget.h"
#include "speedometerchart.h"

SpeedometerManager::SpeedometerManager()
    :m_t(0)
{

}


void SpeedometerManager::addDatas(QList<QPointF>& list, QWidget* parent)
{
    Q_UNUSED(parent);
    m_list = list;

    QObject::connect(&m_timer, &QTimer::timeout, this, &SpeedometerManager::handleTimeout);
    m_timer.setInterval(100);

    qDebug()<< " Size : " << m_list.size();
   /* QList<QPointF>::iterator it;
    for (it = m_list.begin();it!=m_list.end();++it)
    {
         qDebug()<< (*it).x() << " : " << (*it).y();
    }*/


    m_pChart = new SpeedometerChart;

    m_pChart->setTitle("Dynamic chart");
    m_pChart->legend()->hide();
    m_pChart->setAnimationOptions(QChart::AllAnimations);
    QChartView* m_pChartView = new QChartView(m_pChart);
    m_pChartView->setRenderHint(QPainter::Antialiasing);

    m_pChartWindow = new QWidget(0);
    QVBoxLayout *layout = new QVBoxLayout(m_pChartWindow);
    layout->addWidget(m_pChartView);
    //setLayout(layout);
    layout->activate();

    m_pChartWindow->resize(1024, 800);
    m_pChartWindow->show();

    //speed meter
    m_pSpeedGauge = new QcGaugeWidget;
    m_pSpeedGauge->addBackground(99);
    QcBackgroundItem *bkg1 = m_pSpeedGauge->addBackground(92);
    bkg1->clearrColors();
    bkg1->addColor(0.1,Qt::black);
    bkg1->addColor(1.0,Qt::white);

    QcBackgroundItem *bkg2 = m_pSpeedGauge->addBackground(88);
    bkg2->clearrColors();
    bkg2->addColor(0.1,Qt::gray);
    bkg2->addColor(1.0,Qt::white);

    //graduation instantanée
    m_pSpeedGauge->addArc(63);
    m_pSpeedGauge->addDegrees(74)->setValueRange(80,180);
    m_pSpeedGauge->addColorBand(60);

    //graduation moyenne
    m_pSpeedGauge->addArc(34);
    m_pSpeedGauge->addDegrees(40)->setValueRange(80,180);

    //Valeur de la graduation
    m_pSpeedGauge->addValues(80)->setValueRange(80,180);

    m_pSpeedGauge->addLabel(70)->setText("Bpm");
    QcLabelItem *lab = m_pSpeedGauge->addLabel(40);
    lab->setText("0");

    //Aiguilles
    m_pSpeedNeedleAvg = m_pSpeedGauge->addNeedle(40);
    m_pSpeedNeedleAvg->setLabel(lab);
    m_pSpeedNeedleAvg->setColor(Qt::blue);
    m_pSpeedNeedleAvg->setValueRange(80,180);

    m_pSpeedNeedle3 = m_pSpeedGauge->addNeedle(46);
    m_pSpeedNeedle3->setLabel(lab);
    m_pSpeedNeedle3->setColor(Qt::white);
    m_pSpeedNeedle3->setValueRange(80,180);

    m_pSpeedNeedle2 = m_pSpeedGauge->addNeedle(52);
    m_pSpeedNeedle2->setLabel(lab);
    m_pSpeedNeedle2->setColor(Qt::white);
    m_pSpeedNeedle2->setValueRange(80,180);

    m_pSpeedNeedle1 = m_pSpeedGauge->addNeedle(58);
    m_pSpeedNeedle1->setLabel(lab);
    m_pSpeedNeedle1->setColor(Qt::white);
    m_pSpeedNeedle1->setValueRange(80,180);

    m_pSpeedNeedle = m_pSpeedGauge->addNeedle(70);
    m_pSpeedNeedle->setLabel(lab);
    m_pSpeedNeedle->setColor(Qt::blue);
    m_pSpeedNeedle->setValueRange(80,180);


    m_pSpeedGauge->addBackground(7);
   // mSpeedGauge->addGlass(88);
    m_pSpeedGauge->resize(800,600);
    m_pSpeedGauge->show();

    m_timer.start();


}

void SpeedometerManager::handleTimeout()
{
    m_t++;

    if (m_list.isEmpty())
    {
        return;
    }
    qreal y = m_list.takeFirst().y();

    QPointF point(m_t, y);
    m_factory.AddPoint(point);

    const QList<QPointF>& avglist = m_factory.GetAvgList();

    float value =0;
    float value1 =0;
    float value2 =0;
    float value3 =0;
    float valueAvg = 0;

    for (QList<QPointF>::const_iterator it = avglist.begin(); it != avglist.end();it++)
    {
        qDebug()<< "Avg list : " << (*it).x() << " : "  << (*it).y();

        switch ((int)(*it).x())
        {
            case 0: value = (*it).y(); break;
            case 17: value1 = (*it).y(); break;
            case 82: value2 = (*it).y(); break;
            case 150: value3 = (*it).y(); break;
            case 200: valueAvg = (*it).y(); break;
        }

    }
    qDebug()<<" ";

    m_pChart->setList(avglist);

    m_pSpeedNeedle->setCurrentValue(value);
    m_pSpeedNeedle1->setCurrentValue(value1);
    m_pSpeedNeedle2->setCurrentValue(value2);
    m_pSpeedNeedle3->setCurrentValue(value3);
    m_pSpeedNeedleAvg->setCurrentValue(valueAvg);

}




