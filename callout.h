#ifndef CALLOUT_H
#define CALLOUT_H

#include <QtCharts/QChartGlobal>
#include <QtCharts/QLineSeries>
#include <QtWidgets/QGraphicsItem>
#include <QtGui/QFont>

class QGraphicsSceneMouseEvent;

QT_CHARTS_BEGIN_NAMESPACE
class QChart;
QT_CHARTS_END_NAMESPACE

QT_CHARTS_USE_NAMESPACE

class Callout : public QGraphicsItem
{
public:
    Callout(QChart *parent);

    void setText(const QString &text);
    void setAnchor(QPointF point);
    void updateGeometry(QLineSeries* serie);

    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,QWidget *widget);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);

private:
    QString m_text;
    QRectF m_textRect;
    QRectF m_rect;
    QPointF m_anchor;
    QFont m_font;
    QChart *m_chart;
    QLineSeries* m_currentserie;
};

#endif // CALLOUT_H
