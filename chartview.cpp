#include <QtGui/QMouseEvent>
#include <QDebug>
#include <QTimer>
#include <QApplication>
#include <QTime>


#include "chartview.h"
#include "mainwidget.h"
#include "speedometermanager.h"

ChartView::ChartView(QChart *chart, QWidget *parent) :
    QChartView(chart, parent),
    m_noUpdate(false),
    m_coordX(0),
    m_coordY(0),
    m_tooltip(0),
    m_mouseDataType(No_data),
    m_beginTime(0),
    m_manager(NULL)
{
    pMainWidget = (MainWidget*) parent;
    setRubberBand(QChartView::HorizontalRubberBand);
    setDragMode(QGraphicsView::NoDrag);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    chart->setAcceptHoverEvents(true);
    m_coordX = new QGraphicsSimpleTextItem(chart);
    m_coordX->setPos(chart->size().width()/2 - 50, chart->size().height());
    m_coordX->setText("X: ");
    m_coordY = new QGraphicsSimpleTextItem(chart);
    m_coordY->setPos(chart->size().width()/2 + 50, chart->size().height());
    m_coordY->setText("Y: ");

    this->setMouseTracking(true);

}

ChartView::~ChartView()
{
    //delete m_manager;
}

void ChartView::resizeEvent(QResizeEvent *event)
{
    if (scene()) {
        scene()->setSceneRect(QRect(QPoint(0, 0), event->size()));
         chart()->resize(event->size());
         m_coordX->setPos(chart()->size().width()/2 - 50, chart()->size().height() - 20);
         m_coordY->setPos(chart()->size().width()/2 + 50, chart()->size().height() - 20);
        /* const auto callouts = m_callouts;
         for (Callout *callout : callouts)
             callout->updateGeometry();*/
    }
    QGraphicsView::resizeEvent(event);
}

void ChartView::mousePressEvent(QMouseEvent *event)
{
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
    if (m_mouseDataType != No_data)
    {
        QLineSeries* serie = GetCurrentSeries();
        qreal temp = chart()->mapToValue(event->pos(),serie).x();
        QString strValue = QString::number(temp, 'f', 1);
        m_coordX->setText(QString("X: %1").arg(strValue));
        m_coordY->setText(QString("Y: %1").arg(static_cast<int>(chart()->mapToValue(event->pos(),serie).y())));

        m_coordX->setVisible(true);
        m_coordY->setVisible(true);
     /*   m_coordX->setText(QString("X: %1").arg(event->pos().x()));
        m_coordY->setText(QString("Y: %1").arg(event->pos().y()));*/
    }
    else
    {
       m_coordX->setVisible(false);
       m_coordY->setVisible(false);
    }
    QChartView::mouseMoveEvent(event);

}

void ChartView::mouseReleaseEvent(QMouseEvent *event)
{
    // Because we disabled animations when touch event was detected
    // we must put them back on.
    chart()->setAnimationOptions(QChart::SeriesAnimations);

    QChartView::mouseReleaseEvent(event);

    QApplication::setOverrideCursor(QCursor(Qt::ArrowCursor));
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
    case Qt::Key_Space:
        if (m_beginTime !=0)
        {
           qDebug()<< " Key_Enter : " << m_beginTime;

           //recuperation de la liste à partir de l'encroit choisi
           QLineSeries* pCurrentSerie = GetCurrentSeries();
           if (NULL != pCurrentSerie)
           {
               QList<QPointF> list = pCurrentSerie->points();
               QList<QPointF>::iterator it;
               QList<QPointF> listFinal;
               for (it = list.begin();it!=list.end();++it)
               {
                   if ((*it).x() > m_beginTime)
                   {
                       listFinal.append((*it));
                   }
               }

               m_manager = new SpeedometerManager;
               m_manager->addDatas(listFinal, pMainWidget);

           }

        }
        else
        {
          qDebug()<< " Key_Enter  no data";
        }
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
 /*       if ( event->delta()>0)
             chart()->zoomIn();
         else
             chart()->zoomOut();*/
        if ( event->delta()>0)
             chart()->scroll(-200, 0);
        else
             chart()->scroll(200, 0);
        m_noUpdate = true;
        //time de 500ms pour ne pas zoomer durant cette période
        QTimer::singleShot(500,(ChartView *) this, SLOT(ResetNoUpdate()));
    }

    event->accept();
}
void ChartView::ResetNoUpdate()
{
    m_noUpdate=false;
}

void ChartView::keepCallout()
{
   /* m_callouts.append(m_tooltip);
    m_tooltip = new Callout(chart());*/
}

void ChartView::tooltipSpeed(QPointF point, bool state)
{
    m_mouseDataType = Data_speed;
    tooltip(point, state);

}
void ChartView::tooltipHRM(QPointF point, bool state)
{
    m_mouseDataType = Data_HRM;
    tooltip(point, state);
}
void ChartView::tooltipAlt(QPointF point, bool state)
{
    m_mouseDataType = Data_Alt;
    tooltip(point, state);
}

void ChartView::DoubleClicSpeed(QPointF point)
{
    //m_mouseDataType = Data_speed;
    qDebug()<<"DoubleClicSpeed " << point.x();




}



void ChartView::tooltip(QPointF point, bool state)
{
    if (m_tooltip == 0)
        m_tooltip = new Callout(chart());

    if (state) {
        QLineSeries* serie = GetCurrentSeries();

        m_tooltip->setText(GetToolTipString(point));
        m_tooltip->setAnchor(point);
        m_tooltip->setZValue(11);
        m_tooltip->updateGeometry(serie);
        m_tooltip->show();
    } else {
        m_tooltip->hide();
        m_mouseDataType = No_data;
        m_beginTime = 0;
    }
}
QLineSeries* ChartView::GetCurrentSeries()
{
    QList<QAbstractSeries *> list = chart()->series();

    QAbstractSeries* serie;
    QString strKey = GetCurrentSeriesString();
    foreach( serie, list )
    {
        if (serie->name().contains(strKey))
        {
            return (QLineSeries*)serie;
        }
    }

    return NULL;
}

QString ChartView::GetCurrentSeriesString()
{
    QString str;
    switch (m_mouseDataType){
        case No_data:str="No_data";break;
        case Data_speed:str="Speed";break;
        case Data_HRM:str="HRM";break;
        case Data_Alt:str="Altitude";break;
        default:str="Error";break;
    }
    return str;
}

QString ChartView::GetToolTipString(QPointF point)
{
    QString str="";

    QString strValueX = QString::number(point.x(), 'f', 1);
    QString strValueY;

    //retreive abscisse type
    bool boIskm = pMainWidget->isKm();

    QString abs;
    if (boIskm)
    {
        abs = QString("%1 km").arg(strValueX);
    }
    else
    {   QTime time(0,0,0);
        int sec = (int)(point.x());
        //qDebug()<<"sec : " << sec;
        time = time.addSecs(sec);
        abs = QString("time %1").arg(time.toString("h:mm:ss"));
        m_beginTime = sec;
    }

    switch (m_mouseDataType){
        case Data_speed:
            strValueY = QString::number(point.y(), 'f', 1);
            str = QString("%1 %2 km/h\n %3").arg("Speed").arg(strValueY).arg(abs);
            break;
        case Data_HRM:
            strValueY = QString::number(point.y(), 'f', 0);
            str = QString("%1 %2 bpm\n %3").arg("HR").arg(strValueY).arg(abs);
            break;
        case Data_Alt:
            strValueY = QString::number(point.y(), 'f', 0);
            str = QString("%1 %2 m\n %3").arg("Altitude").arg(strValueY).arg(abs);
            break;
        case No_data:
        default:
            str="";
            m_beginTime = 0;
    }
    return str;
}
