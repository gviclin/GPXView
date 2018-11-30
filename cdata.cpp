#include "cdata.h"

CData::CData():
    lon(0),
    lat(0),
    bpm(0),
    alt(0),
    temp(0),
    cad(0),
    pwr(0),
    datetime(QDate(0,0,0),QTime(0,0,0)),
    distance(0),
    speed(0),
    secDelta(0),
    sec(0),
    bAddedPoint(false)
{

}

CData& CData::operator=(const CData& data)
{
    lon=data.lon;
    lat=data.lat;
    bpm=data.bpm;
    alt=data.alt;
    temp=data.temp;
    pwr=data.pwr;
    distance=data.distance;
    speed=data.speed;
    datetime=data.datetime;
    sec=data.sec;
    secDelta=data.secDelta;
    bAddedPoint=data.bAddedPoint;

    return *this;
}

CData& CData::operator+=(const CData& data)
{
    lon+=data.lon;
    lat+=data.lat;
    bpm+=data.bpm;
    alt+=data.alt;
    temp+=data.temp;
    pwr+=data.pwr;
    distance+=data.distance;
    speed+=data.speed;
    return *this;
}

void CData::DivideBy(const int number)
{
    lon=lon/number;
    lat=lat/number;
    bpm=bpm/number;
    alt=alt/number;
    temp=temp/number;
    pwr=pwr/number;
    distance=distance/number;
    speed=speed/number;
}
