#pragma once
#include "glalib.hpp"
class Camera
{
public:
    Camera();
    Matrix4f GetViewMatrix();
    const Matrix4f update(Point CameraPosition, Point TargetPosition);


private:
    Point pos;
    Vector look,vup;
    float fv;
    float f; //far field
    float n; //near field

};