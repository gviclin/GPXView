#include "mainwidget.h"
#include "cgpxtools.h"

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
    QWidget(parent),
    m_EltNumbers(0)
{
    // Create chart view with the chart
    m_chart = new QChart();
    m_chartView = new ChartView(m_chart, this);
    m_chartView->setRenderHint(QPainter::Antialiasing);

    // Create layout for grid and detached legend
    m_mainLayout = new QGridLayout();
    m_mainLayout->addWidget(m_chartView, 0, 1, 3, 1);
    setLayout(m_mainLayout);

    // Add few series
    RetreiveDatas();

    AddDatasToChart();

    connectMarkers();

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

        break;
        }
    default:
        {
        qDebug() << "Unknown marker type";
        break;
        }
    }
}

void MainWidget::RetreiveDatas()
{
    QList<CData> list;
    CGpxTools::GetGPXData("../GPXView/Sortie_v_lo_le_midi.gpx",list);

    m_seriesSpeed = new QLineSeries();
    m_seriesHR = new QLineSeries();
    m_seriesAlt = new QLineSeries();

    m_seriesSpeed->setName("Speed");
    m_seriesHR->setName("HRM");
    m_seriesAlt->setName("Altitude");

    QList<CData>::iterator it;

    for (it = list.begin(); it != list.end(); ++it) {
        CData data = *it;
 /*       QPointF p((qreal) data.distance/1000, data.speed);
        QPointF pHR((qreal) data.distance/1000, data.bpm);
        QPointF pAlt((qreal) data.distance/1000, data.alt);*/
        QPointF p((qreal) 1000*data.sec, data.speed);
        QPointF pHR((qreal) 1000*data.sec, data.bpm);
        QPointF pAlt((qreal) 1000*data.sec, data.alt);
        //QPointF p((qreal) i, qSin(M_PI / 50 * i) * 100);
        //p.ry() += std::rand()/((RAND_MAX)/50);
        *m_seriesSpeed << p;
        *m_seriesHR <<pHR;
        *m_seriesAlt << pAlt;
        m_EltNumbers++;
    }
}

void MainWidget::AddDatasToChart()
{
    // Customize color
    QPen pen(QColor(Qt::blue));
    pen.setWidth(2);
    m_seriesSpeed->setPen(pen);

    QPen pen2(QColor(Qt::red));
    pen2.setWidth(2);
    m_seriesHR->setPen(pen2);

    QPen pen3(QColor(Qt::black));
    pen3.setWidth(2);
    m_seriesAlt->setPen(pen3);


    connect(m_seriesHR, &QLineSeries::clicked, m_chartView, &ChartView::keepCallout);
    connect(m_seriesHR, &QLineSeries::hovered, m_chartView, &ChartView::tooltipHRM);

    connect(m_seriesAlt, &QLineSeries::clicked, m_chartView, &ChartView::keepCallout);
    connect(m_seriesAlt, &QLineSeries::hovered, m_chartView, &ChartView::tooltipAlt);

    connect(m_seriesSpeed, &QLineSeries::clicked, m_chartView, &ChartView::keepCallout);
    connect(m_seriesSpeed, &QLineSeries::hovered, m_chartView, &ChartView::tooltipSpeed);


    //axes
    QDateTime a,b;
    a.setTime(QTime(0,0,0));
    b.setTime(QTime(1,0,0));
    QDateTimeAxis *axisX = new QDateTimeAxis;
    axisX->setFormat("hh mm ss");
  //  axisX->setRange(0, m_EltNumbers);
    axisX->setMin(a);
    axisX->setMax(b);
    axisX->setTickCount(10);
 //   axisX->setLabelFormat("%.0d");
    axisX->setTitleText("Duration");
    m_chartView->chart()->addAxis(axisX, Qt::AlignTop);
    m_seriesAlt->attachAxis(axisX);
    m_seriesHR->attachAxis(axisX);
    m_seriesSpeed->attachAxis(axisX);



    QValueAxis *axisYAlt = new QValueAxis;
    axisYAlt->setRange(0, 150);
    axisYAlt->setTickCount(10);
    axisYAlt->setLabelFormat("%.0d");
    axisYAlt->setTitleText("Altitude");
    axisYAlt->setLinePenColor(m_seriesAlt->pen().color());
    m_chartView->chart()->addAxis(axisYAlt, Qt::AlignLeft);
    m_seriesAlt->attachAxis(axisYAlt);


    QValueAxis *axisYSpeed = new QValueAxis;
    axisYSpeed->setRange(0, 90);
    axisYSpeed->setTickCount(10);
    axisYSpeed->setLabelFormat("%.0d");
    axisYSpeed->setTitleText("Speed");
    axisYSpeed->setLinePenColor(m_seriesSpeed->pen().color());
    m_chartView->chart()->addAxis(axisYSpeed, Qt::AlignRight);
    m_seriesAlt->attachAxis(axisYSpeed);

    QValueAxis *axisYHRM = new QValueAxis;
    axisYHRM->setRange(0, 200);
    axisYHRM->setTickCount(10);
    axisYHRM->setLabelFormat("%.0d");
    axisYHRM->setTitleText("HRM");
    axisYHRM->setLinePenColor(m_seriesHR->pen().color());
    m_chartView->chart()->addAxis(axisYHRM, Qt::AlignRight);
    m_seriesAlt->attachAxis(axisYHRM);

    //chart->addSeries(m_seriesSpeed);
    m_chart->addSeries(m_seriesHR);
    m_chart->addSeries(m_seriesAlt);
    m_chart->addSeries(m_seriesSpeed);

}
