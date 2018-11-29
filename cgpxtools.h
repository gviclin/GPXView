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
    static void GetGPXData(QString sFile, QList<CData>& list,QList<CData>& listTransformed);

};

#endif // CGPXTOOLS_H
