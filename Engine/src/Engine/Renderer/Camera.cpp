#include "epch.h"
#include "Camera.h"

namespace Engine
{
	Camera::Camera(glm::vec3 position, glm::vec3 up, float yaw, float pitch, float aspectratio)
		: Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(2.5f), MouseSensitivity(0.1f), Zoom(45.0f), Position(position), WorldUp(up), Yaw(yaw), Pitch(pitch), aspectRatio(aspectratio)
	{
		updateCameraVectors();
	}
	
	glm::mat4 Camera::GetViewMatrix()
	{
		return glm::lookAt(Position, Position + Front, Up);
	}
	
	glm::mat4 Camera::GetProjectionMatrix(float nearPlane, float farPlane)
	{
		return glm::perspective(glm::radians(Zoom), aspectRatio, nearPlane, farPlane);
	}
	
	glm::mat4 Camera::GetViewProjectionMatrix()
	{
		return GetProjectionMatrix(0.1f, 100.0f) * GetViewMatrix();
	}
	
	void Camera::ProcessKeyboard(ECameraDirection direction, float deltaTime)
	{
		float velocity = MovementSpeed * deltaTime;
		switch (direction)
		{
		case ECameraDirection::FORWARD:
			Position += Front * velocity;
			break;
		case ECameraDirection::BACKWARD:
			Position -= Front * velocity;
			break;
		case ECameraDirection::LEFT:
			Position -= Right * velocity;
			break;
		case ECameraDirection::RIGHT:
			Position += Right * velocity;
			break;
		default:
			break;
		}
	}
	
	void Camera::ProcessMouseMovement(float xoffset, float yoffset, bool constrainPitch /*= true*/)
	{
		xoffset *= MouseSensitivity;
		yoffset *= MouseSensitivity;
	
		Yaw += xoffset;
		Pitch += yoffset;
	
		if (constrainPitch) {
			if (Pitch > 89.0f) {
				Pitch = 89.0f;
			}
			if (Pitch < -89.0f) {
				Pitch = -89.0f;
			}
		}
	
		updateCameraVectors();
	}
	
	void Camera::ProcessMouseScroll(float yoffset)
	{
		Zoom -= (float)yoffset;
		if (Zoom < 1.0f) {
			Zoom = 1.0f;
		}
		if (Zoom > 45.0f) {
			Zoom = 45.0f;
		}
	}
	
	void Camera::updateCameraVectors()
	{
		glm::vec3 front;
		front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
		front.y = sin(glm::radians(Pitch));
		front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
		Front = glm::normalize(front);
	
		Right = glm::normalize(glm::cross(Front, WorldUp));
		Up = glm::normalize(glm::cross(Right, Front));
	}
}
