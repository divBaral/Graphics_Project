#pragma once
#include "Vector.hpp"
#include "glalib.hpp"


//define possible camera options
enum Camera_Movement
{
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT,
	UP,
	DOWN,
	//circular movement
	CRIGHT,
	CLEFT
};

//default camera values

const float YAW = -90.0f;
const float PITCH = 0.0f;
const float SPEED = 7.5f;
const float SENSITIVITY = 0.05f;
const float ZOOM = 45.0f;
const float EULERFACTOR = 30.f;


class Camera
{

private:
//camera attributes
		Vector Position;
		Vector Front;
		Vector Up;
		Vector Right;
		Vector WorldUp;

		//euler angles;
		float Yaw;
		float Pitch;

		//camera options
		float Zoom;
        float MovementSpeed;

public:
    Camera();
    const Matrix4f GetViewMatrix();
    void  ProcessKeyboard(Camera_Movement direction, float deltaTime);
    const Matrix4f update(Point CameraPosition, Point TargetPosition);
    void  updateCameraVectors();

    const Vector getPosition() const { return Position;} 


};