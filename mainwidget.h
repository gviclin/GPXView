#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include <QtWidgets/QWidget>
#include <QtWidgets/QGraphicsWidget>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGraphicsGridLayout>

#include "chartview.h"

class MainWidget : public QWidget
{
    Q_OBJECT
public:
    explicit MainWidget(QWidget *parent = nullptr);
    void RetreiveDatas();
    void AddDatasToChart();

signals:

public slots:
     void connectMarkers();
     void handleMarkerClicked();

 private:
    QChart *m_chart;
    ChartView *m_chartView;

    QLineSeries *m_seriesSpeed;
    QLineSeries *m_seriesHR;
    QLineSeries *m_seriesAlt;


    QGridLayout *m_mainLayout;
    QGridLayout *m_fontLayout;
};

#endif // MAINWIDGET_H
