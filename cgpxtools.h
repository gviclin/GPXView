#ifndef CGPXTOOLS_H
#define CGPXTOOLS_H

#include <QXmlInputSource>
#include <QXmlStreamReader>
#include <QtDebug>
#include "cdata.h"


class CGpxTools
{
public:
    CGpxTools();
    static void DumpList(QList<CData>& list);
    static void DumpElement(CData& item);
    static void GetGPXData(QString sFile, QList<CData>& list,int smoothing, double factor);
    static QList<CData>& GetSmoothingList(int smoothing, QList<CData>& list);
    static QList<CData>& GetAccelerateList(QList<CData>& list, double factor /*1 no acceleration, 1.1 : 10% accelerate*/);
    static void SaveList(QString sFile, QList<CData>& list);
};

#endif // CGPXTOOLS_H
