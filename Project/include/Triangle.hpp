#pragma once
#include "glalib.hpp"
#include "Material.hpp"


class Triangle
{
public:
    Triangle() {}
    Triangle(Point vv0, Point vv1, Point vv2) {
        v0=vv0;
        v1=vv1;
        v2=vv2;
    }
    Point v0, v1, v2;
    Vector n0, n1, n2;
    Vector tv0, tv1, tv2;
    Material mtl;
    bool hasnormal = false;
};