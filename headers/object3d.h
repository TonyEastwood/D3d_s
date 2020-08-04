#ifndef OBJECT3D_H
#define OBJECT3D_H
#include "vcgUtils.h"
#include <QString>
#include <tuple>
class Object3d
{
private:
    bool visible;
    bool selected;

    QString pathToMesh;

    MyMesh * currentMesh;

    std::tuple<float,float,float> colorFaces;
    std::tuple<float,float,float> colorGrid;
public:
    Object3d();
    ~Object3d();



    MyMesh * getMeshPtr();


    bool isSelected() const;
    void setSelected(bool value);
    bool isVisible() const;
    void setVisible(bool value);

    QString getFileName();
    std::tuple<float, float, float> getColorFaces() const;
    void setColorFaces(const std::tuple<float, float, float> &value);
    std::tuple<float, float, float> getColorGrid() const;
    void setColorGrid(const std::tuple<float, float, float> &value);
};

#endif // OBJECT3D_H
