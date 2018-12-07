#include "mainwidget.h"
#include "cdata.h"

#include <QtCharts/QValueAxis>
#include <QtCharts/QLegendMarker>
#include <QtCharts/QValueAxis>
#include <QtCharts/QDateTimeAxis>

#include <QtWidgets/QPushButton>
#include <QtWidgets/QLabel>
#include <QtCore/QDebug>
#include <QComboBox>

#include <QtWidgets/QFormLayout>
#include <QtCore/QtMath>


MainWidget::MainWidget(QWidget *parent) :
    QWidget(parent),
    boIsKm(true)
{
    CreateColor();

    // Create chart view with the chart
    m_chart = new QChart();
    m_chartView = new ChartView(m_chart, this);
    m_chartView->setRenderHint(QPainter::Antialiasing);

    // Create layout for grid and detached legend
    m_mainLayout = new QGridLayout();
    m_mainLayout->addWidget(m_chartView, 0, 0, 15, 15);

    QComboBox *combo = new QComboBox(this);
    m_mainLayout->addWidget(combo,16,0,Qt::AlignBottom);
    combo->addItem("Km");
    combo->addItem("Time");

    QObject::connect(combo, SIGNAL(currentIndexChanged(QString)), this, SLOT(SelectAbscisse(QString)));

    setLayout(m_mainLayout);

    // Set the title and show legend
    m_chart->setTitle("GPX analysis");
    m_chart->setAnimationOptions(QChart::SeriesAnimations);
    m_chart->legend()->setVisible(true);
    m_chart->legend()->setAlignment(Qt::AlignRight);
    m_chart->legend()->show();

}

void MainWidget::SelectAbscisse(QString string)
{

    if (0 == QString::compare(string,"Km"))
    {
        boIsKm=true;
    }
    else
    {
       boIsKm=false;
    }
    //qDebug()<<"SelectAbscisse"<<(boIsKm?"Km":"Time");
    removeAllFromChart();

    for(MapDataList::iterator it = m_GpxNameMap.begin();it != m_GpxNameMap.end();++it)
    {
          addDatasToChart(it.key());
    }
}

void MainWidget::connectMarkers()
{
    // Connect all markers to handler
    const auto markers = m_chart->legend()->markers();
    for (QLegendMarker *marker : markers) {
        // Disconnect possible existing connection to avoid multiple connections
        QObject::disconnect(marker, &QLegendMarker::clicked,
                            this, &MainWidget::handleMarkerClicked);
        QObject::connect(marker, &QLegendMarker::clicked, this, &MainWidget::handleMarkerClicked);
    }
}

void MainWidget::handleMarkerClicked()
{
    QLegendMarker* marker = qobject_cast<QLegendMarker*> (sender());
    Q_ASSERT(marker);

    switch (marker->type())
    {
        case QLegendMarker::LegendMarkerTypeXY:
        {
        // Toggle visibility of series
        marker->series()->setVisible(!marker->series()->isVisible());

        // Turn legend marker back to visible, since hiding series also hides the marker
        // and we don't want it to happen now.
        marker->setVisible(true);

        // Dim the marker, if series is not visible
        qreal alpha = 1.0;

        if (!marker->series()->isVisible())
            alpha = 0.5;

        QColor color;
        QBrush brush = marker->labelBrush();
        color = brush.color();
        color.setAlphaF(alpha);
        brush.setColor(color);
        marker->setLabelBrush(brush);

        brush = marker->brush();
        color = brush.color();
        color.setAlphaF(alpha);
        brush.setColor(color);
        marker->setBrush(brush);

        QPen pen = marker->pen();
        color = pen.color();
        color.setAlphaF(alpha);
        pen.setColor(color);
        marker->setPen(pen);

        //hide/show corresponding axes
    /*    QLineSeries* line = (QLineSeries *)marker->series();
        QAbstractAxis * axisYAlt = m_chart->axisY(m_seriesSpeed);
        axisYAlt->hide();*/

        break;
        }
    default:
        {
        qDebug() << "Unknown marker type";
        break;
        }
    }
}

bool MainWidget::addNewGPX(QList<CData>* plist, QString sName)
{
    if (m_GpxNameMap.find(sName) != m_GpxNameMap.end())
    {
        qDebug()<<"addNewGPX : Gpx file already imported! "  << sName;
        return false;
    }
    m_GpxNameMap.insert(sName,plist);

    addDatasToChart(sName);

    /*qDebug()<<"addNewGPX : "
           <<" , m_GpxNameMap "   << m_GpxNameMap.count()
           <<" , m_seriesSpeedMap "         << m_seriesSpeedMap.count()
           <<" , m_seriesHRMap "            << m_seriesHRMap.count()
           <<" , m_seriesAltMap "           << m_seriesAltMap.count()
           <<"     "<< sName;*/

    return true;
}

void MainWidget::removeAllGPX()
{
    //detruire tous les objets QList<CData>
    for (MapDataList::iterator itDataList=m_GpxNameMap.begin();itDataList!=m_GpxNameMap.end();)
    {
        QList<CData>* pLlist = itDataList.value();
        delete pLlist;

        itDataList = m_GpxNameMap.erase(itDataList);
    }
    removeAllFromChart();
    qDebug()<<"removeAllGPX : "
           <<" , m_GpxNameMap "   << m_GpxNameMap.count()
           <<" , m_seriesSpeedMap "         << m_seriesSpeedMap.count()
           <<" , m_seriesHRMap "            << m_seriesHRMap.count()
           <<" , m_seriesAltMap "           << m_seriesAltMap.count();
}

void MainWidget::removeAllFromChart()
{
    //Détruire les 3 listes
    for (MapSerie::iterator it = m_seriesSpeedMap.begin(); it != m_seriesSpeedMap.end();)
    {
        QLineSeries* pSerie = it.value();

        disconnect(pSerie, &QLineSeries::clicked, m_chartView, &ChartView::keepCallout);
        disconnect(pSerie, &QLineSeries::hovered, m_chartView, &ChartView::tooltipSpeed);
        disconnect(pSerie, &QLineSeries::doubleClicked, m_chartView, &ChartView::DoubleClicSpeed);

        delete pSerie;
        it = m_seriesSpeedMap.erase(it);
    }
    for (MapSerie::iterator it = m_seriesHRMap.begin(); it != m_seriesHRMap.end();)
    {
        QLineSeries* pSerie = it.value();

        disconnect(pSerie, &QLineSeries::clicked, m_chartView, &ChartView::keepCallout);
        disconnect(pSerie, &QLineSeries::hovered, m_chartView, &ChartView::tooltipHRM);

        delete pSerie;
        it = m_seriesHRMap.erase(it);
    }
    for (MapSerie::iterator it = m_seriesAltMap.begin(); it != m_seriesAltMap.end();)
    {
        QLineSeries* pSerie = it.value();

        disconnect(pSerie, &QLineSeries::clicked, m_chartView, &ChartView::keepCallout);
        disconnect(pSerie, &QLineSeries::hovered, m_chartView, &ChartView::tooltipAlt);

        delete pSerie;
        it = m_seriesAltMap.erase(it);
    }

    m_chart->removeAllSeries();
}

void MainWidget::setVisible(QString sName, bool boVisible)
{
    m_seriesSpeedMap[sName]->setVisible(boVisible);
    m_seriesHRMap[sName]->setVisible(boVisible);
    m_seriesAltMap[sName]->setVisible(boVisible);
}


void MainWidget::addDatasToChart(QString str)
{
    QLineSeries* seriesSpeed = new QLineSeries();
    QLineSeries* seriesHR = new QLineSeries();
    QLineSeries* seriesAlt = new QLineSeries();

    seriesSpeed->setName("Speed - " + str);
    seriesHR->setName("HRM - " + str);
    seriesAlt->setName("Altitude - " + str);

    QList<CData>::iterator it;
    QPointF p;
    QPointF pHR;
    QPointF pAlt;
    for (it = m_GpxNameMap[str]->begin(); it != m_GpxNameMap[str]->end(); ++it) {
        CData data = *it;
        if (boIsKm)
        {
            p = QPointF((qreal) data.distance/1000, data.speed);
            pHR = QPointF((qreal) data.distance/1000, data.bpm);
            pAlt = QPointF((qreal) data.distance/1000, data.alt);
        }
        else
        {
            p = QPointF((qreal) data.sec, data.speed);
            pHR = QPointF((qreal) data.sec, data.bpm);
            pAlt = QPointF((qreal) data.sec, data.alt);
        }

        //QPointF p((qreal) i, qSin(M_PI / 50 * i) * 100);
        //p.ry() += std::rand()/((RAND_MAX)/50);
        *seriesSpeed << p;
        *seriesHR <<pHR;
        *seriesAlt << pAlt;

    }

    //m_EltNumbers = seriesAlt->points().last().rx();
    m_seriesSpeedMap.insert(str,seriesSpeed);
    m_seriesHRMap.insert(str,seriesHR);
    m_seriesAltMap.insert(str,seriesAlt);

    // Customize color
    int ColorNumber = m_seriesSpeedMap.count()-1;
//    qDebug() << "addDatasToChart: ColorNumber " << ColorNumber;

    QPen pen(m_colorSpeedVector[ColorNumber]);
    pen.setWidth(2);
    m_seriesSpeedMap[str]->setPen(pen);

    QPen pen2(m_colorHRVector[ColorNumber]);
    pen2.setWidth(2);
    m_seriesHRMap[str]->setPen(pen2);

    QPen pen3(m_colorAltVector[ColorNumber]);
    pen3.setWidth(2);
    m_seriesAltMap[str]->setPen(pen3);

    connect(m_seriesSpeedMap[str], &QLineSeries::clicked, m_chartView, &ChartView::keepCallout);
    connect(m_seriesSpeedMap[str], &QLineSeries::hovered, m_chartView, &ChartView::tooltipSpeed);
    connect(m_seriesSpeedMap[str], &QLineSeries::doubleClicked, m_chartView, &ChartView::DoubleClicSpeed);

    connect(m_seriesHRMap[str], &QLineSeries::clicked, m_chartView, &ChartView::keepCallout);
    connect(m_seriesHRMap[str], &QLineSeries::hovered, m_chartView, &ChartView::tooltipHRM);

    connect(m_seriesAltMap[str], &QLineSeries::clicked, m_chartView, &ChartView::keepCallout);
    connect(m_seriesAltMap[str], &QLineSeries::hovered, m_chartView, &ChartView::tooltipAlt);



/*
    //axes
    QDateTime a,b;
    a.setTime(QTime(1,0,0));
    b.setTime(QTime(2,0,0));
    qint64 d = b.toMSecsSinceEpoch();
    //QDateTimeAxis *axisX = new QDateTimeAxis;
    QValueAxis *axisX = new QValueAxis;
   // axisX->setFormat("hh:mm:ss");
    axisX->setRange(0, m_EltNumbers);
 //   axisX->setMin(a);
 //   axisX->setMax(b);
    axisX->setTickCount(10);
 //   axisX->setLabelFormat("%.0d");
    axisX->setTitleText("Duration");
  //  m_chartView->chart()->addAxis(axisX, Qt::AlignBottom);
    //m_seriesAlt->attachAxis(axisX);
   // m_seriesHR->attachAxis(axisX);
  //  m_seriesSpeed->attachAxis(axisX);
    m_chart->setAxisX(axisX,m_seriesSpeed);
*/
/*
    QValueAxis *axisYAlt = new QValueAxis;
    axisYAlt->setRange(0, 150);
    axisYAlt->setTickCount(10);
    axisYAlt->setLabelFormat("%.0d");
    axisYAlt->setTitleText("Altitude");
    axisYAlt->setLinePenColor(m_seriesAlt->pen().color());
    m_chartView->chart()->addAxis(axisYAlt, Qt::AlignLeft);
    m_seriesAlt->attachAxis(axisYAlt);
*//*
    QValueAxis *axisYSpeed = new QValueAxis;
    //axisYSpeed->setRange(0, 90);
    axisYSpeed->setMax(50);
    axisYSpeed->setTickCount(10);
    axisYSpeed->setLabelFormat("%.0d");
    axisYSpeed->setTitleText("Speed");
    axisYSpeed->setLinePenColor(m_seriesSpeed->pen().color());
 //   m_chart->addAxis(axisYSpeed, Qt::AlignRight);
  //  m_seriesAlt->attachAxis(axisYSpeed);
    m_chart->setAxisY(axisYSpeed,m_seriesSpeed);*/
/*
    QValueAxis *axisYHRM = new QValueAxis;
    axisYHRM->setRange(0, 200);
    axisYHRM->setTickCount(10);
    axisYHRM->setLabelFormat("%.0d");
    axisYHRM->setTitleText("HRM");
    axisYHRM->setLinePenColor(m_seriesHR->pen().color());
    m_chartView->chart()->addAxis(axisYHRM, Qt::AlignRight);
    m_seriesAlt->attachAxis(axisYHRM);*/

    m_chart->addSeries(m_seriesSpeedMap[str]);
    m_chart->addSeries(m_seriesHRMap[str]);
    m_chart->addSeries(m_seriesAltMap[str]);

    m_chart->createDefaultAxes();

    connectMarkers();
}

void MainWidget::CreateColor()
{
    m_colorSpeedVector.append(QColor(Qt::blue));
    m_colorSpeedVector.append(QColor(Qt::darkBlue));
    m_colorSpeedVector.append(QColor(Qt::darkCyan));
    m_colorSpeedVector.append(QColor(Qt::cyan));
    m_colorSpeedVector.append(QColor(Qt::blue));
    m_colorSpeedVector.append(QColor(Qt::blue));
    m_colorSpeedVector.append(QColor(Qt::blue));
    m_colorSpeedVector.append(QColor(Qt::blue));
    m_colorSpeedVector.append(QColor(Qt::blue));


    m_colorHRVector.append(QColor(Qt::red));
    m_colorHRVector.append(QColor(Qt::darkRed));
    m_colorHRVector.append(QColor(Qt::darkMagenta));
    m_colorHRVector.append(QColor(Qt::magenta));
    m_colorHRVector.append(QColor(Qt::red));
    m_colorHRVector.append(QColor(Qt::red));
    m_colorHRVector.append(QColor(Qt::red));
    m_colorHRVector.append(QColor(Qt::red));

    m_colorAltVector.append(QColor(Qt::black));
    m_colorAltVector.append(QColor(Qt::darkGray));
    m_colorAltVector.append(QColor(Qt::gray));
    m_colorAltVector.append(QColor(Qt::lightGray));
    m_colorAltVector.append(QColor(Qt::black));
    m_colorAltVector.append(QColor(Qt::black));
    m_colorAltVector.append(QColor(Qt::black));
    m_colorAltVector.append(QColor(Qt::black));



}
