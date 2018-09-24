#ifndef CDATA_H
#define CDATA_H

#include <QTime>

class CData
{
public:
    CData();

public:
    double lon;
    double lat;
    int bpm;
    int alt;
    int temp;
    int cad;
    QDate day;
    QTime hours;

//computed values
    double distance; //in meters
    float speed; //in km/h
    int secDelta;
   };

#endif // CDATA_H
