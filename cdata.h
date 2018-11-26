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
    float alt;
    int temp;
    int cad;
    int pwr;
    QDate day;
    QTime hours;

//computed values
    double distance; //in meters
    float speed; //in km/h
    int secDelta;
    int sec;
   };

#endif // CDATA_H
