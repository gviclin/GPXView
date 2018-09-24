#ifndef CHARTVIEW_H
#define CHARTVIEW_H

#include <QtCharts/QChartView>
#include <QtWidgets/QRubberBand>

QT_CHARTS_USE_NAMESPACE

class ChartView : public QChartView
{
    Q_OBJECT
public:
    ChartView(QChart *chart, QWidget *parent = 0);

public Q_SLOTS:
        void ResetNoUpdate();

protected:
    bool viewportEvent(QEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void keyPressEvent(QKeyEvent *event);
    void wheelEvent(QWheelEvent *event);




private:
    bool m_isTouching;
    QPointF m_lastMousePos;
    bool m_noUpdate;
    QGraphicsSimpleTextItem* m_coordX;
    QGraphicsSimpleTextItem* m_coordY;
};

#endif
