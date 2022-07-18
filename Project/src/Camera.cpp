#include "Camera.hpp"

Camera::Camera()
{
}
// pass world coordinates ranges in cuboid 100
Matrix4f Camera::update(Point CameraPosition, Point TargetPosition)
{
    // calculate uvw of camera
    fv = 120;        // field of view
    vup = {0, 1, 0}; // vector up assume
    f = 100.f;       // far plane
    n = 10.f;        // near plane
    look = (TargetPosition - CameraPosition).normalize();
    auto w = look.scale(-1.0f);
    auto v = vup - w * (vup.dot(w));
    v = v.normalize();
    Vector u = (v.cross(w)).normalize();

    // std::cerr<<"U: "<<u.x<<" "<<u.y<<" "<<u.z<<std::endl;
    // std::cerr<<"U: "<<v.x<<" "<<v.y<<" "<<v.z<<std::endl;
    // std::cerr<<"U: "<<w.x<<" "<<w.y<<" "<<w.z<<std::endl;

    // calcuate field of view in radian
    fv = fv * PI / 180;
    // Transformation Parameters
    Vector P = {CameraPosition.x, CameraPosition.y, CameraPosition.z};
    auto A = P - w.scale(f);
    auto B = A + u.scale(f * tan(fv / 2));
    auto C = A + v.scale(f);

    // generate persceptive matrix
    Matrix4f Tper = af::PointsToPoints(
        Point(P), Point(A), Point(B), Point(C),
        Point(0, 0, 0), Point(0, 0, -1), Point(1, 0, -1), Point(0, 1, -1));

    Matrix4f Z(f - n, 0, 0, 0,
               0, f - n, 0, 0,
               0, 0, f, n,
               0, 0, n - f, 0); // implement if you care near view

    return Z * Tper;
}
