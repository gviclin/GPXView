#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include <QtWidgets/QWidget>
#include <QtWidgets/QGraphicsWidget>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGraphicsGridLayout>

#include "chartview.h"

class CData;
class Callout;

class MainWidget : public QWidget
{
    Q_OBJECT
public:
    explicit MainWidget(QWidget *parent = nullptr);
    void addNewGPX(QList<CData> list, QString sName);
    void setVisible(QString sName, bool boVisible);


private:
    void addDatasToChart(QString str);

signals:

public slots:
     void connectMarkers();
     void handleMarkerClicked();

 private:
    void CreateColor();

    QMenu *m_fileMenu;
    QChart *m_chart;
    ChartView *m_chartView;

    QMap<QString,QLineSeries *>m_seriesSpeedMap;
    QMap<QString,QLineSeries *>m_seriesHRMap;
    QMap<QString,QLineSeries *>m_seriesAltMap;
    QList<QString>m_GpxNameList;

    QVector<QColor> m_colorSpeedVector;
    QVector<QColor> m_colorHRVector;
    QVector<QColor> m_colorAltVector;

    QGridLayout *m_mainLayout;
    QGridLayout *m_fontLayout;

    Callout *m_tooltip;
    QList<Callout *> m_callouts;


};

#endif // MAINWIDGET_H
