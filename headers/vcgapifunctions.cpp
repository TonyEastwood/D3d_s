#include "vcgapifunctions.h"
std::vector<vcg::Point3d>* vcg::PointMatchingScale::fix;
std::vector<vcg::Point3d>* vcg::PointMatchingScale::mov;
vcg::Box3d vcg::PointMatchingScale::b;

vcgApiFunctions::vcgApiFunctions()
{

}

int vcgApiFunctions::importMesh(MyMesh *ptr, const QString &path)
{
    return vcg::tri::io::Importer<MyMesh>::Open(*ptr,path.toLocal8Bit());
}

void vcgApiFunctions::normalizeMesh(MyMesh *ptr)
{
    vcg::tri::UpdateNormal<MyMesh>::PerVertexNormalizedPerFace(*ptr);
}

void vcgApiFunctions::exportStl(MyMesh *ptr, const QString &path)
{
    vcg::tri::io::ExporterSTL<MyMesh>::Save(*ptr ,path.toLocal8Bit());
}

void vcgApiFunctions::exportPly(MyMesh *ptr, const QString &path)
{
    vcg::tri::io::ExporterPLY<MyMesh>::Save(*ptr ,path.toLocal8Bit());
}

QString vcgApiFunctions::alignSecondMesh(MyMesh *mesh1, MyMesh *mesh2)
{
    const uint COUNT_ALIGN_CYCLES=5;
    const double ERROR_ALIGN=0.021f;

   if(mesh1->fn==0 || mesh2->fn==0)

       return "Warning","Please, choose two objects";




   vcg::AlignPair::Result result;

   vcg::AlignPair::Result previousResult;

   vcg::AlignPair::Param ap;
   vcg::AlignPair::A2Mesh fix;
   vcg::AlignPair aa;
   vcg::AlignPair::A2Grid UG;
   vcg::AlignPair::A2GridVert VG;
   std::vector<vcg::AlignPair::A2Vertex> tmpmv;


   // 1) Convert fixed mesh and put it into the grid.





   aa.convertMesh<MyMesh>(*mesh1,fix);



   if((*mesh1).fn==0 || ap.UseVertexOnly) {
       fix.initVert(vcg::Matrix44d::Identity());
       vcg::AlignPair::InitFixVert(&fix,ap,VG);
   }
   else {
       fix.init(vcg::Matrix44d::Identity());
       vcg::AlignPair::initFix(&fix, ap, UG);
   }



   // 2) Convert the second mesh and sample a <ap.SampleNum> points on it.
   float previousError=100000;
   std::pair<double,double> distance;
   for(uint i=0;i<COUNT_ALIGN_CYCLES;++i)
   {
       aa.convertVertex((*mesh2).vert,tmpmv);




       aa.sampleMovVert(tmpmv, ap.SampleNum, ap.SampleMode);

       aa.mov=&tmpmv;
       aa.fix=&fix;
       aa.ap = ap;


       //use identity as first matrix
       vcg::Matrix44d In;
       In.SetIdentity();


       // Perform the ICP algorithm
       aa.align(In,UG,VG,result);

       //rotate m2 using the resulting transformation
       vcg::tri::UpdatePosition<MyMesh>::Matrix(*mesh2, result.Tr, true);
       vcg::tri::UpdateBounding<MyMesh>::Box(*mesh2);



       distance=result.computeAvgErr();

       if(distance.second<previousError)
       {
           //  qDebug()<<"prev x="<<(*drawSecondObject).face[10523].P(0).X();
           previousResult=result;
           previousError=distance.second;
           // qDebug()<<"Prev value"<<distance.second;
       }
       else{
           //  qDebug()<<"new Prev Value="<<distance.second;
           vcg::tri::UpdatePosition<MyMesh>::Matrix(*mesh2,vcg::Inverse(result.Tr), true);
           vcg::tri::UpdatePosition<MyMesh>::Matrix(*mesh2, previousResult.Tr, true);
           vcg::tri::UpdateBounding<MyMesh>::Box(*mesh2);
           //  qDebug()<<"new x="<<(*drawSecondObject).face[10523].P(0).X();
           distance=previousResult.computeAvgErr();
           // qDebug()<<"New value value"<<distance.second;
           break;
       }
       //   qDebug()<<"iteration="<<i;

   }
   if(distance.second>ERROR_ALIGN)
       return QString("Defective mesh\nDistance="+QString::number(distance.second)+">"+QString::number(ERROR_ALIGN));
   else
       return QString("Distance:\ndd="+QString::number(distance.first)+"\ndd="+QString::number(distance.second));

}

void vcgApiFunctions::mergeMesh2ToMesh1(MyMesh *mesh1, MyMesh *mesh2)
{
    if((*mesh1).fn==0 || (*mesh2).fn==0)
    {
        qDebug()<<"Error fn==0 || fn2==0";
        return;
    }

    vcg::tri::Append<MyMesh, MyMesh>::Mesh(*mesh1,*mesh2);

    normalizeMesh(mesh1);

    (*mesh2).fn=0;
    (*mesh2).face.clear();
    (*mesh2).vert.clear();
}



