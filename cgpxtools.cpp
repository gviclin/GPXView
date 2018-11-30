#include "cgpxtools.h"
#include "cgeoconvert.h"

CGpxTools::CGpxTools()
{

}

void CGpxTools::GetGPXData(QString sFile, QList<CData>& list,int smoothing)
{
    //open GPX File
    QFile file(sFile);
    if(!file.open(QFile::ReadOnly | QFile::Text)){
        qDebug() << "Cannot read file :" << file.errorString();
        exit(0);
    }

    QXmlStreamReader reader(&file);
    QDateTime firstDateTime;
    int ind=0;
    if (reader.readNextStartElement())
    {
        qDebug()<< "first start element" <<reader.name().toString();
        if (reader.name() == "gpx")
        {
            while (!reader.atEnd())
            {
                if (reader.isStartElement() && reader.name()== "trkpt")
                {
                    CData item;
                    QString attribute_value;

                    //Recuperation des 2 attributs
                    foreach(const QXmlStreamAttribute &attr, reader.attributes())
                    {
                        if (attr.name().toString() == QLatin1String("lat"))
                        {
                            attribute_value = attr.value().toString();
                            //qDebug() << " - lat : " << qPrintable(attribute_value);
                            item.lat = attribute_value.toDouble();
                        }
                        if (attr.name().toString() == QLatin1String("lon"))
                        {
                            attribute_value = attr.value().toString();
                            item.lon = attribute_value.toDouble();
                        }
                     }
                    //recuperation des infos à l'intérieur de l'élément
                    while (!(reader.isEndElement() && reader.name()== "trkpt"))
                    {
                        if (reader.isStartElement() && reader.name()== "ele")
                        {
                           item.alt = reader.readElementText().toFloat();
                        }
                        if (reader.isStartElement() && reader.name()== "time")
                        {
                           QString txt = reader.readElementText();
                           QStringList lst = txt.split("T");
                           item.datetime = QDateTime::fromString(txt,Qt::ISODate);
                           item.sec=item.datetime.currentSecsSinceEpoch();
                           //qDebug()<<item.hours.toString()<<" "<<dt.toString();
                           if (ind==0)
                           {
                               firstDateTime=item.datetime;
                               item.sec=0;
                           }
                           else
                           {
                              item.sec= (int) firstDateTime.secsTo(item.datetime);
                           }
                        }
                        if (reader.isStartElement() && reader.name()== "atemp")
                        {
                           item.temp =  reader.readElementText().toInt();
                        }
                        if (reader.isStartElement() && reader.name()== "hr")
                        {
                           item.bpm = reader.readElementText().toInt();
                        }
                        if (reader.isStartElement() && reader.name()== "cad")
                        {
                           item.cad = reader.readElementText().toInt();
                        }
                        reader.readNext();
                    }
                   //add element in the list
                   ind++;
                    list.append(item);
                }
                reader.readNext();
            }

            double N,E, Nold=0,Eold=0, distOld=0,temp;
            int secOld;
            int Z;

            QList<CData>::iterator i;
            qDebug()<<"Number of elements <"<<list.size()<<">";
            //float distance =0;
            for (i = list.begin(); i!=list.end();)
            {
                CData& item = *i;

                  CGeoConvert::LLtoUTM(eWGS84,item.lat,item.lon,N,E,Z);
               // qDebug()<<"x, y : <"<<N<<" , "<<E<< ". Zone : "<<Z;

                if (i == list.begin())
                {
                    item.distance = 0;
                    item.speed  = 0;
                    item.secDelta =0;
                }
                else
                {
                    temp = sqrt((N-Nold)*(N-Nold) + (E-Eold)*(E-Eold));
                    item.distance = temp + distOld;
                    item.secDelta = item.sec-secOld;
                    item.speed = 3.6*temp/(item.secDelta);

                    //DumpElement(item);
                    //correction garmin : un point si il n'a pas bouger par rapport à l'ancien
                    if (0==temp)
                    {
                        i = list.erase(i);
                        continue;
                    }
                }
                Nold = N;
                Eold = E;
                distOld = item.distance;
                secOld=item.sec;
                 ++i;
            }


        }
        else
          qDebug("not gpx element");
    }


    //DumpList(list);

    if (true)
    {
        float speedOld=0,speedOldOld=0;
        QList<CData> newList;
        //correction des points aberrants
        for (QList<CData>::iterator i = list.begin(); i != list.end(); ++i)
        {
            CData& item =*i;

            qDebug()<<"point : " << item.sec << "speed :" <<speedOldOld << " " << speedOld << " " << item.speed;


            //correction points abérants
            float delta = item.speed-speedOld;
            float DeltaOld= speedOld-speedOldOld;
            if (delta>10 && DeltaOld < -10)
            {
                //pb
                QList<CData>::iterator it_temp = i;
                if (it_temp != list.begin())
                {
                    it_temp--;
                    if (it_temp!= list.begin())
                    {
                        it_temp--;
                        newList.erase(--newList.end());
                        newList.erase(--newList.end());
                    }
                }
            }
            if (delta<-10 && DeltaOld >10)
            {
                //pb
                QList<CData>::iterator it_temp = i;
                if (it_temp != list.begin())
                {
                    it_temp--;
                    if (it_temp!= list.begin())
                    {
                        it_temp--;
                        newList.erase(--newList.end());
                    }
                }
            }
            else
            {
               newList.append(item);

            }
            speedOldOld=speedOld;
            speedOld = item.speed;

        }
        list=newList;
    }

    //upscaling
    CData Olditem(*list.begin());
    QList<CData> listUpscaling;
    listUpscaling.append(Olditem);
    for (QList<CData>::iterator i = list.begin(); i != list.end(); ++i)
    {
        CData& item = *i;

        if (list.begin() != i)
        {
            if (item.secDelta > 1)
            {
                //réechantilloner,
                //Calcul des pentes de chaque type d'info
                double bpmSlope = (item.bpm-Olditem.bpm)/item.secDelta;
                double vitesseSlope = (item.speed-Olditem.speed)/item.secDelta;
                double altSlope = (item.alt-Olditem.alt)/item.secDelta;
                double tempSlope = (item.temp-Olditem.temp)/item.secDelta;
                double tempCad = (item.cad-Olditem.cad)/item.secDelta;
                double tempDist = (item.distance-Olditem.distance)/item.secDelta;
                double tempLon = (item.lon-Olditem.lon)/item.secDelta;
                double tempLat = (item.lat-Olditem.lat)/item.secDelta;

                for (int ind =1;ind <item.secDelta;++ind)
                {
                   CData newItem;
                   newItem.lon=0;
                   newItem.lat=0;
                   newItem.secDelta=1;
                   newItem.sec=Olditem.sec+ind;
                   newItem.datetime = Olditem.datetime.addSecs(ind);
                   newItem.bpm= Olditem.bpm + ind*bpmSlope;
                   newItem.speed= Olditem.speed + ind*vitesseSlope;
                   newItem.alt= Olditem.alt + ind*altSlope;
                   newItem.temp= Olditem.temp + ind*tempSlope;
                   newItem.cad= Olditem.cad + ind*tempCad;
                   newItem.distance= Olditem.distance + ind*tempDist;
                   newItem.lon= Olditem.lon + ind*tempLon;
                   newItem.lat= Olditem.lat + ind*tempLat;
                   newItem.bAddedPoint=true;
                   listUpscaling.append(newItem);
                }
                item.secDelta=1;
            }
            listUpscaling.append(item);
        }
        Olditem = item;
    }

  //  DumpList(listUpscaling);
    if (smoothing > 0)
    {
        list = GetSmoothingList(smoothing,listUpscaling);
    }

}

QList<CData>& CGpxTools::GetSmoothingList(int smoothing/*0 pas de moyenne, 1 : moyenne sur 3, 2 : moyenne sur 5, etc*/,
                                          QList<CData>& list)
{
    //Moyenne mobile centrée
    QList<CData>* pLlistTransformed = new QList<CData>;
    QList<CData>::iterator i,i_internal_plus,i_internal_moins;

    CData data;
    short ind;
    short number =0;
    int test=0;
    for (i = list.begin(); i != list.end(); i++)
    {
        number=1; //1 element minimum dans chaque moyenne
        data = (*i);

        i_internal_plus=i;
        if (i_internal_plus!=list.end())
        {
            i_internal_plus++;
            for (ind=0;i_internal_plus!=list.end() && ind<smoothing ;i_internal_plus++)
            {
               data+=(*i_internal_plus);
               number++;
               ind++;
            }
        }


        i_internal_moins=i;
        if (i_internal_moins!=list.begin())
        {
            i_internal_moins--;
            for (ind=0;i_internal_moins!=list.begin() && ind<smoothing ;i_internal_moins--)
            {
               data+=(*i_internal_moins);
               number++;
               ind++;
            }
        }

        data.DivideBy(number);

        pLlistTransformed->append(data);
        test++;
    }


    //DumpList(*pLlistTransformed);
    return *pLlistTransformed;

}

void CGpxTools::DumpList(QList<CData>& list)
{
    QList<CData>::iterator i;
    for (i = list.begin(); i != list.end(); ++i)
    {
        DumpElement(*i);
    }
    qDebug()<< "Element number :" << list.count();
}

void CGpxTools::DumpElement(CData& item)
{
    qDebug()<< (item.bAddedPoint?"  ":"")<<"Point : sec"    << (item.sec)
                                << ", secDelta "<< (item.secDelta)
                                << ", lat "<< (item.lat)
                                << ", lon "<< (item.lon)
                                << ", hr "<< (item.bpm)
                                << ", alt "<< (item.alt)
                                << ", temp "<< (item.temp)
                                << ", cad "<< (item.cad)
                                << ", datetime "<< (item.datetime.toString())
                                << ", distance "<< (item.distance)
                                << ", speed "<< (item.speed);
}
