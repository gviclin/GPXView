#include "cgpxtools.h"
#include "cgeoconvert.h"

CGpxTools::CGpxTools()
{

}

void CGpxTools::GetGPXData(QString sFile, QList<CData>& list,QList<CData>& listTransformed)
{
    //open GPX File
    QFile file(sFile);
    if(!file.open(QFile::ReadOnly | QFile::Text)){
        qDebug() << "Cannot read file :" << file.errorString();
        exit(0);
    }

    QXmlStreamReader reader(&file);
    qDebug() << "Start Parsing";
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
                           QStringList lst = reader.readElementText().split("T");
                           item.day =  QDate::fromString(lst.at(0),"yyyy-MM-dd");  //2018-09-17T10:19:56Z
                           item.hours = QTime::fromString(lst.at(1).split("Z").at(0),"hh:mm:ss");
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
                    list.append(item);
                }
                reader.readNext();
            }
            double N,E, Nold=0,Eold=0, distOld=0,temp;
            long timeOld=0;
            int Z,secOld=0;
            double speedOld=0;
            double speedOldOld=0;

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
                    timeOld = item.hours.secsTo(QTime(0,0,0));
                    item.secDelta =0;
                    item.sec=0;                    
                }
                else
                {
                    temp = sqrt((N-Nold)*(N-Nold) + (E-Eold)*(E-Eold));
                    item.distance = temp + distOld;
                    item.secDelta = timeOld-item.hours.secsTo(QTime(0,0,0));
                    item.speed = 3.6*temp/(item.secDelta);
                    item.sec=item.secDelta+secOld;

                    //correction garmin : un point si il n'a pas bouger par rapport à l'ancien
                    if (0==temp)
                    {
                        i = list.erase(i);
                        continue;
                    }
                    //DumpElement(item);

                    //correction points abérants
                    float delta = item.speed-speedOld;
                    float DeltaOld= speedOld-speedOldOld;/*
                    if (delta>10 && DeltaOld < -10)
                    {
                        //pb
                        qDebug()<<"point : " << item.sec << "speed :" <<speedOldOld << " " << speedOld << " " << item.speed;

                        QList<CData>::iterator it_temp = i;
                        if (it_temp != list.begin())
                        {
                            it_temp--;
                            if (it_temp!= list.begin())
                            {
                                it_temp = i--;
                                list.erase(it_temp);
                            }
                        }
                        speedOld = item.speed;
                        speedOldOld = item.speed;
                        secOld=item.sec;
                        Nold = N;
                        Eold = E;
                        distOld = item.distance;
                        timeOld = item.hours.secsTo(QTime(0,0,0));
                         ++i;
                        continue;
                    }
*/
                }
                secOld=item.sec;
                Nold = N;
                Eold = E;
                distOld = item.distance;
                timeOld = item.hours.secsTo(QTime(0,0,0));
                speedOldOld=speedOld;
                speedOld = item.speed;
                 ++i;
            }

            DumpList(list);
        }
        else
          qDebug("not gpx element");
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
                   newItem.day = Olditem.day;
                   newItem.hours = Olditem.hours.addSecs(ind);
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

    //Moyenne mobile centrée
    QList<CData>::iterator i,i_internal_plus,i_internal_moins;
    int AverageType = 3; //0 pas de moyenne, 1 : moyenne sur 3, 2 : moyenne sur 5, etc
    CData data;
    short ind;
    short number =0;
    int test=0;
    for (i = listUpscaling.begin(); i != listUpscaling.end(); i++)
    {
        number=1; //1 element minimum dans chaque moyenne
        data = (*i);

        i_internal_plus=i;
        if (i_internal_plus!=listUpscaling.end())
        {
            i_internal_plus++;
            for (ind=0;i_internal_plus!=listUpscaling.end() && ind<AverageType ;i_internal_plus++)
            {
               data+=(*i_internal_plus);
               number++;
               ind++;
            }
        }


        i_internal_moins=i;
        if (i_internal_moins!=listUpscaling.begin())
        {
            i_internal_moins--;
            for (ind=0;i_internal_moins!=listUpscaling.begin() && ind<AverageType ;i_internal_moins--)
            {
               data+=(*i_internal_moins);
               number++;
               ind++;
            }
        }

        data.DivideBy(number);

        listTransformed.append(data);
        test++;
    }


    //DumpList(listTransformed);
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
                                << ", day "<< (item.day.toString())
                                << ", hours "<< (item.hours.toString())
                                << ", distance "<< (item.distance)
                                << ", speed "<< (item.speed);
}
