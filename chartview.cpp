
#include "chartview.h"
#include <QtGui/QMouseEvent>
#include <QDebug>
#include <QTimer>

ChartView::ChartView(QChart *chart, QWidget *parent) :
    QChartView(chart, parent),
    m_isTouching(false),
    m_noUpdate(false),
    m_coordX(0),
    m_coordY(0)
{
    setRubberBand(QChartView::NoRubberBand);
    setDragMode(QGraphicsView::NoDrag);
    this->setMouseTracking(true);

    m_coordX = new QGraphicsSimpleTextItem(chart);
    m_coordY = new QGraphicsSimpleTextItem(chart);

}

bool ChartView::viewportEvent(QEvent *event)
{
    if (event->type() == QEvent::TouchBegin) {
        // By default touch events are converted to mouse events. So
        // after this event we will get a mouse event also but we want
        // to handle touch events as gestures only. So we need this safeguard
        // to block mouse events that are actually generated from touch.
        m_isTouching = true;

        // Turn off animations when handling gestures they
        // will only slow us down.
        chart()->setAnimationOptions(QChart::NoAnimation);
    }
    return QChartView::viewportEvent(event);
}

void ChartView::mousePressEvent(QMouseEvent *event)
{
    if (m_isTouching)
        return;
    if (event->button() == Qt::LeftButton)
    {
        QApplication::setOverrideCursor(QCursor(Qt::SizeAllCursor));
        m_lastMousePos = event->pos();
        event->accept();
    }
    QChartView::mousePressEvent(event);
}

void ChartView::mouseMoveEvent(QMouseEvent *event)
{
    if (m_isTouching)
        return;

    // pan the chart with a middle mouse drag
    if (event->buttons() & Qt::LeftButton)
    {
        auto dPos = event->pos() - m_lastMousePos;
        chart()->scroll(-dPos.x(), dPos.y());

        m_lastMousePos = event->pos();
        event->accept();

        //QApplication::restoreOverrideCursor();
    }
    /* Setting the mouse position label on the axis from value to position */
     qreal x = (event->pos()).x();
     qreal y = (event->pos()).y();

     qreal xVal = chart()->mapToValue(event->pos()).x();
     qreal yVal = chart()->mapToValue(event->pos()).y();
              qDebug()<<"x: "<<xVal<<", y: "<<yVal;

   /*qreal maxX = chart()->
     qreal minX = axisX->min();
     qreal maxY = axisY->max();
     qreal minY = axisY->min();*/

   //  if (xVal <= maxX && xVal >= minX && yVal <= maxY && yVal >= minY)
     {
         QPointF xPosOnAxis = chart()->mapToPosition(QPointF(x, 0));
         QPointF yPosOnAxis = chart()->mapToPosition(QPointF(0, y));


         /* m_coordX and m_coordY are `QGraphicsSimpleTextItem` */
         m_coordX->setPos(x, xPosOnAxis.y() + 5);
         m_coordY->setPos(x-4, y-15);

         /* Displaying value of the mouse on the label */
         m_coordX->setText(QString("%1").arg(xVal, 4, 'f', 1, '0'));
         m_coordY->setText(QString("%1").arg(yVal, 4, 'f', 1, '0'));
     }


    QChartView::mouseMoveEvent(event);

}

void ChartView::mouseReleaseEvent(QMouseEvent *event)
{
    if (m_isTouching)
        m_isTouching = false;

    // Because we disabled animations when touch event was detected
    // we must put them back on.
    chart()->setAnimationOptions(QChart::SeriesAnimations);

    QChartView::mouseReleaseEvent(event);
}

void ChartView::keyPressEvent(QKeyEvent *event)
{
    switch (event->key()) {
    case Qt::Key_Plus:
        chart()->zoomIn();
        break;
    case Qt::Key_Minus:
        chart()->zoomOut();
        break;
    case Qt::Key_Left:
        chart()->scroll(-100, 0);
        break;
    case Qt::Key_Right:
        chart()->scroll(100, 0);
        break;
    case Qt::Key_Up:
        chart()->scroll(0, 100);
        break;
    case Qt::Key_Down:
        chart()->scroll(0, -100);
        break;
    default:
        QGraphicsView::keyPressEvent(event);
        break;
    }
}

void ChartView::wheelEvent(QWheelEvent *event)
{
    if (m_noUpdate == false)
    {
        if ( event->delta()>0)
             chart()->zoomIn();
         else
             chart()->zoomOut();
        m_noUpdate = true;
        QTimer::singleShot(500,(ChartView *) this, SLOT(ResetNoUpdate()));
    }

    event->accept();
}
void ChartView::ResetNoUpdate()
{
    m_noUpdate=false;
}
