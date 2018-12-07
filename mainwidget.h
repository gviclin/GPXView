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

    typedef QMap<QString, QList<CData>*> MapDataList;
    typedef QMap<QString,QLineSeries *>  MapSerie;
public:
    explicit MainWidget(QWidget *parent = nullptr);
    bool addNewGPX(QList<CData>* plist, QString sName);
    void setVisible(QString sName, bool boVisible);
    void removeAllGPX();
    bool isKm() {return boIsKm;}

private:
    void addDatasToChart(QString str);
    void removeAllFromChart();

signals:

public slots:
     void connectMarkers();
     void handleMarkerClicked();
     void SelectAbscisse(QString string);

 private:
    void CreateColor();

    bool boIsKm;
    QMenu *m_fileMenu;
    QChart *m_chart;
    ChartView *m_chartView;

    MapDataList  m_GpxNameMap;

    MapSerie m_seriesSpeedMap;
    MapSerie m_seriesHRMap;
    MapSerie m_seriesAltMap;

    QVector<QColor> m_colorSpeedVector;
    QVector<QColor> m_colorHRVector;
    QVector<QColor> m_colorAltVector;

    QGridLayout *m_mainLayout;
    QGridLayout *m_fontLayout;

    Callout *m_tooltip;
    QList<Callout *> m_callouts;


};

#endif // MAINWIDGET_H
