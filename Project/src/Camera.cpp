#include "Camera.hpp"

Camera::Camera()
{
}
// pass world coordinates ranges i
const Matrix4f Camera::update(Point CameraPosition, Point TargetPosition)
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


    // calcuate field of view in radian
    fv = fv * PI / 180;
    // Transformation Parameters
    Matrix4f Translate(1, 0, 0, -CameraPosition.x, 
                       0, 1, 0, -CameraPosition.y,
                       0, 0, 1, -CameraPosition.z,
                       0, 0, 0, 1);
    Matrix4f Rotate( u.x , u.y, u.z, 0, 
                       v.x, v.y, v.z,0,
                       w.x, w.y, w.z, 0,
                       0, 0, 0, 1);
//     Vector P = {CameraPosition.x, CameraPosition.y, CameraPosition.z};
//     auto A = P - w.scale(f);
//     auto B = A + u.scale(f * tan(fv / 2));
//     auto C = A + v.scale(f);

//     // generate persceptive matrix
//     Matrix4f Tper = af::PointsToPoints(
//         Point(P), Point(A), Point(B), Point(C),
//         Point(0, 0, 0), Point(0, 0, -1), Point(1, 0, -1), Point(0, 1, -1));

//     //opening the frustum
//    Matrix4f Z(f - n, 0, 0, 0,
//                0, f - n, 0, 0,
//                0, 0, f, n,
//                0, 0, n - f, 0); 
 
    return Rotate*Translate;
}
