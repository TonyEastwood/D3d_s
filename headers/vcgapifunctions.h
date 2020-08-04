#ifndef VCGAPIFUNCTIONS_H
#define VCGAPIFUNCTIONS_H

#include "headers/vcgUtils.h"
#include <QString>
#include <QMessageBox>
#include <QDebug>
class vcgApiFunctions
{
public:
    vcgApiFunctions();

    static int importMesh(MyMesh * ptr, const QString & path);

    static void normalizeMesh(MyMesh *ptr);

    static void exportStl(MyMesh * ptr, const QString & path);
    static void exportPly(MyMesh * ptr, const QString & path);

    static QString alignSecondMesh(MyMesh *mesh1, MyMesh *mesh2);

    static void mergeMesh2ToMesh1(MyMesh *mesh1, MyMesh *mesh2);
};

#endif // VCGAPIFUNCTIONS_H
