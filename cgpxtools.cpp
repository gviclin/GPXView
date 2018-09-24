#include "cgpxtools.h"
#include "cgeoconvert.h"

CGpxTools::CGpxTools()
{

}

void CGpxTools::GetGPXData(QString sFile, QList<CData>& list)
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
                           item.alt = reader.readElementText().toInt();
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
            double N,E, Nold,Eold, distOld,temp;
            long timeOld;
            int Z;

            QList<CData>::iterator i;
            qDebug()<<"Number of elements <"<<list.size()<<">";
            //float distance =0;
            for (i = list.begin(); i!=list.end(); ++i)
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
                }
                else
                {
                    temp = sqrt((N-Nold)*(N-Nold) + (E-Eold)*(E-Eold));
                    item.distance = temp + distOld;
                    item.secDelta = timeOld-item.hours.secsTo(QTime(0,0,0));
                    item.speed = 3.6*temp/(item.secDelta);

                }
                Nold = N;
                Eold = E;
                distOld = item.distance;
                timeOld = item.hours.secsTo(QTime(0,0,0));

            }
/*
            for (i = list.begin(); i != list.end(); ++i)
            {
                CData& item = *i;
                qDebug()<< "Point : lat "<< (item.lat)
                                        << ", lon "<< (item.lon)
                                        << ", hr "<< (item.bpm)
                                        << ", alt "<< (item.alt)
                                        << ", temp "<< (item.temp)
                                        << ", cad "<< (item.cad)
                                        << ", day "<< (item.day.toString())
                                        << ", hours "<< (item.hours.toString())
                                        << ", distance "<< (item.distance)
                                        << ", secDelta "<< (item.secDelta)
                                        << ", speed "<< (item.speed);


            }*/
        }
        else
          qDebug("not gpx element");
    }

}
