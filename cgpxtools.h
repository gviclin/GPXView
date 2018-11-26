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

    static void GetGPXData(QString sFile, QList<CData>& list);
};

#endif // CGPXTOOLS_H
