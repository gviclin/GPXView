#ifndef CHARTVIEW_H
#define CHARTVIEW_H

#include <QtCharts/QChartView>
#include <QtWidgets/QRubberBand>
#include <QtCharts/QLineSeries>

#include "callout.h"

class MainWidget;

QT_CHARTS_USE_NAMESPACE

class ChartView : public QChartView
{
    Q_OBJECT
public:
    ChartView(QChart *chart, QWidget *parent = 0);

private:
    QLineSeries* GetCurrentSeries();
    QString GetCurrentSeriesString();
    QString GetToolTipString(QPointF point);
    void tooltip(QPointF point, bool state);

    enum MouseDataType
    {
        No_data=0,
        Data_speed,
        Data_HRM,
        Data_Alt
    };

public Q_SLOTS:
        void ResetNoUpdate();
        void keepCallout();
        void tooltipSpeed(QPointF point, bool state);
        void tooltipHRM(QPointF point, bool state);
        void tooltipAlt(QPointF point, bool state);
        void DoubleClicSpeed(QPointF point);

protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void keyPressEvent(QKeyEvent *event);
    void wheelEvent(QWheelEvent *event);
    void resizeEvent(QResizeEvent *event);



private:
    MainWidget* pMainWidget;
    QPointF m_lastMousePos;
    bool m_noUpdate;

    QGraphicsSimpleTextItem *m_coordX;
    QGraphicsSimpleTextItem *m_coordY;
    Callout *m_tooltip;
    QList<Callout *> m_callouts;

    MouseDataType m_mouseDataType;

    //pour le module d'affichage de la vitesse
    int m_beginTime;

};

#endif
