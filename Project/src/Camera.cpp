#include "Camera.hpp"
#include "MatrixTransformation.hpp"
#include "Vector.hpp"

Camera::Camera()
{

    Front = Vector(0.0f,0.0f,-1.f);
    Position = {0,0,30};
    WorldUp = {0,1,0};
    Yaw = YAW;
    Pitch = PITCH;
    MovementSpeed = SPEED;
    updateCameraVectors();
}
// pass world coordinates ranges i
const Matrix4f Camera::GetViewMatrix()
{
   return update(Position,Position+Front );
}
const Matrix4f Camera::update(Point CameraPosition, Point TargetPosition)
{
  
    auto look = (TargetPosition - CameraPosition).normalize();
    auto w = look.scale(1.0f);
    auto v = WorldUp - w * (WorldUp.dot(w));
    v = v.normalize();
    Vector u = (v.cross(w)).normalize();
    Matrix4f view( u.x , u.y, u.z, -CameraPosition.x, 
                       v.x, v.y, v.z,-CameraPosition.y,
                       w.x, w.y, w.z, -CameraPosition.z,
                       0, 0, 0, 1);
 
    return view;
}


void Camera::ProcessKeyboard(Camera_Movement direction, float deltaTime)
{
    float velocity = MovementSpeed * deltaTime;
    if(direction == FORWARD) {
        Position += Front * velocity ;
        
    }
    if(direction == BACKWARD)
        Position -= Front * velocity;
    if(direction == LEFT)
        Position -= Right * velocity;
    if(direction == RIGHT)
        Position += Right * velocity ;
    if(direction == CLEFT) {
        Yaw -= deltaTime*EULERFACTOR;
        updateCameraVectors();
    }
    if(direction == CRIGHT) {
        
        Yaw += deltaTime*EULERFACTOR;
        updateCameraVectors();
    }
    if(direction == UP) {
        Position.y += velocity;
        //Pitch += deltaTime*EULERFACTOR;
        //updateCameraVectors();
    }
    if(direction == DOWN) {
        Position.y -= velocity;
        //Pitch -= deltaTime*EULERFACTOR;
        //updateCameraVectors();
    }
}
inline float radians(float &x) {
    return x*PI/180;
}
void Camera::updateCameraVectors()
{
    Vector front;
    front.x = cos(radians(Yaw)) * cos(radians(Pitch));
    front.y = sin(radians(Pitch));
    front.z = sin(radians(Yaw)) * cos(radians(Pitch));

    Front = (front).normalize();

    Right = Front.cross(WorldUp).normalize();
    Up =  Right.cross(Front).normalize();
}
