#include "object3d.h"
std::tuple<float, float, float> Object3d::getColorFaces() const
{
    return colorFaces;
}

void Object3d::setColorFaces(const std::tuple<float, float, float> &value)
{
    colorFaces = value;
}

std::tuple<float, float, float> Object3d::getColorGrid() const
{
    return colorGrid;
}

void Object3d::setColorGrid(const std::tuple<float, float, float> &value)
{
    colorGrid = value;
}

Object3d::Object3d():visible(true)
{
    currentMesh= new MyMesh();
    currentMesh->fn=0;
    colorFaces={0.5f,0.5f,0.5f};
    colorGrid={0.8f,0.9f,0.8f};
}
bool Object3d::isSelected() const
{
    return selected;
}

void Object3d::setSelected(bool value)
{
    selected = value;
}

bool Object3d::isVisible() const
{
    return visible;
}

void Object3d::setVisible(bool value)
{
    visible = value;
}



Object3d::~Object3d()
{
    delete  currentMesh;
}



QString Object3d::getFileName()
{
    pathToMesh;//NEEEEEED TO CHANGE
}

MyMesh *Object3d::getMeshPtr()
{
    return currentMesh;
}


