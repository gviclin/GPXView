#include "mainwidget.h"
#include "cdata.h"

#include <QtCharts/QValueAxis>
#include <QtCharts/QLegendMarker>
#include <QtCharts/QValueAxis>
#include <QtCharts/QDateTimeAxis>

#include <QtWidgets/QPushButton>
#include <QtWidgets/QLabel>
#include <QtCore/QDebug>

#include <QtWidgets/QFormLayout>
#include <QtCore/QtMath>


MainWidget::MainWidget(QWidget *parent) :
    QWidget(parent)
{
    CreateColor();

    // Create chart view with the chart
    m_chart = new QChart();
    m_chartView = new ChartView(m_chart, this);
    m_chartView->setRenderHint(QPainter::Antialiasing);

    // Create layout for grid and detached legend
    m_mainLayout = new QGridLayout();
    m_mainLayout->addWidget(m_chartView, 0, 1, 3, 1);
    setLayout(m_mainLayout);

    // Set the title and show legend
    m_chart->setTitle("GPX analysis");
    m_chart->setAnimationOptions(QChart::SeriesAnimations);
    m_chart->legend()->setVisible(true);
    m_chart->legend()->setAlignment(Qt::AlignRight);
    m_chart->legend()->show();

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

void MainWidget::addNewDatas(QList<CData> list, QString sName)
{
   m_GpxNameList.append(sName);

    QLineSeries* seriesSpeed = new QLineSeries();
    QLineSeries* seriesHR = new QLineSeries();
    QLineSeries* seriesAlt = new QLineSeries();

    seriesSpeed->setName("Speed - " + sName);
    seriesHR->setName("HRM - " + sName);
    seriesAlt->setName("Altitude - " + sName);

    QList<CData>::iterator it;
    for (it = list.begin(); it != list.end(); ++it) {
        CData data = *it;
 /*       QPointF p((qreal) data.distance/1000, data.speed);
        QPointF pHR((qreal) data.distance/1000, data.bpm);
        QPointF pAlt((qreal) data.distance/1000, data.alt);*/
        QPointF p((qreal) data.sec, data.speed);
        QPointF pHR((qreal) data.sec, data.bpm);
        QPointF pAlt((qreal) data.sec, data.alt);
        //QPointF p((qreal) i, qSin(M_PI / 50 * i) * 100);
        //p.ry() += std::rand()/((RAND_MAX)/50);
        *seriesSpeed << p;
        *seriesHR <<pHR;
        *seriesAlt << pAlt;

    }

    //m_EltNumbers = seriesAlt->points().last().rx();
    m_seriesSpeedMap.insert(sName,seriesSpeed);
    m_seriesHRMap.insert(sName,seriesHR);
    m_seriesAltMap.insert(sName,seriesAlt);

    addDatasToChart(sName);

    connectMarkers();
}

void MainWidget::addDatasToChart(QString str)
{

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
