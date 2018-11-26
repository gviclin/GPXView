#include "mainwidget.h"
#include "cgpxtools.h"

#include <QtCharts/QValueAxis>
#include <QtCharts/QLegendMarker>

#include <QtWidgets/QPushButton>
#include <QtWidgets/QLabel>
#include <QtCore/QDebug>

#include <QtWidgets/QFormLayout>

#include <QtCore/QtMath>
#include <QtCore/QtMath>
#include <cstdlib>

MainWidget::MainWidget(QWidget *parent) : QWidget(parent)
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
/*        QPointF p((qreal) data.distance/1000, data.speed);
        QPointF pHR((qreal) data.distance/1000, data.bpm);
        QPointF pAlt((qreal) data.distance/1000, data.alt);*/
        QPointF p((qreal) data.sec, data.speed);
        QPointF pHR((qreal) data.sec, data.bpm);
        QPointF pAlt((qreal) data.sec, data.alt);
        //QPointF p((qreal) i, qSin(M_PI / 50 * i) * 100);
        //p.ry() += std::rand()/((RAND_MAX)/50);
        *m_seriesSpeed << p;
        *m_seriesHR <<pHR;
        *m_seriesAlt << pAlt;
    }
}

void MainWidget::AddDatasToChart()
{
    // Customize color
    QPen pen(QColor(Qt::blue));
    pen.setWidth(1);
    m_seriesSpeed->setPen(pen);

    QPen pen2(QColor(Qt::red));
    pen2.setWidth(1);
    m_seriesHR->setPen(pen2);

    QPen pen3(QColor(Qt::black));
    pen3.setWidth(1);
    m_seriesAlt->setPen(pen3);

    //chart->addSeries(m_seriesSpeed);
    m_chart->addSeries(m_seriesHR);
    m_chart->addSeries(m_seriesAlt);
    m_chart->addSeries(m_seriesSpeed);


    //axes
    QValueAxis *axisX = new QValueAxis;
    //axisX->setRange(0, 50);
    axisX->setTickCount(10);
    axisX->setLabelFormat("%.0d");
    m_chartView->chart()->setAxisX(axisX, m_seriesHR);

    QValueAxis *axisY = new QValueAxis;
    //axisY->setRange(0, 150);
    axisY->setTickCount(10);
    axisY->setLabelFormat("%.0d");
    m_chartView->chart()->setAxisY(axisY, m_seriesAlt);

}
