#ifndef CDATA_H
#define CDATA_H

#include <QTime>

class CData
{
public:
    CData();
      CData& operator=(const CData& data);
      CData& operator+=(const CData& data);
      void DivideBy(const int number);

public:
    double lon;
    double lat;
    float bpm;
    float alt;
    float temp;
    float cad;
    float pwr;
    QDateTime datetime;

//computed values
    double distance; //in meters
    float speed; //in km/h
    int secDelta;
    int sec;
    bool bAddedPoint;
   };

#endif // CDATA_H
