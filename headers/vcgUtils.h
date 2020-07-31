#ifndef VCGUTILS_H
#define VCGUTILS_H
#include <vcg/complex/complex.h>

   #include <vcg/complex/algorithms/align_pair.h>

   #include <wrap/io_trimesh/import.h>
   #include <wrap/io_trimesh/export_ply.h>
#include <wrap/io_trimesh/export_stl.h>
class MyFace;
class MyVertex;

struct MyUsedTypes :
                public vcg::UsedTypes<  vcg::Use<MyVertex>::AsVertexType,
                vcg::Use<MyFace>::AsFaceType>
{};

class MyVertex :
                public vcg::Vertex< MyUsedTypes, vcg::vertex::Coord3d, vcg::vertex::Normal3d, vcg::vertex::Color4b, vcg::vertex::BitFlags>
{};

class MyFace :
                public vcg::Face < MyUsedTypes, vcg::face::VertexRef, vcg::face::Normal3d, vcg::face::FFAdj, vcg::face::Mark, vcg::face::BitFlags >
{};

class MyMesh :
                public vcg::tri::TriMesh< std::vector<MyVertex>, std::vector<MyFace>>
{
public:
};



#endif // VCGUTILS_H
