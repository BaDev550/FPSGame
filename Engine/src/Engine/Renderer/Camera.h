#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace Engine
{
	enum class ECameraDirection {
		FORWARD,
		BACKWARD,
		LEFT,
		RIGHT
	};;
	class Camera
	{
	private:
		glm::vec3 Position;
		glm::vec3 Front;
		glm::vec3 Up;
		glm::vec3 Right;
		glm::vec3 WorldUp;
	
		float Yaw;
		float Pitch;
	
		float MovementSpeed;
		float MouseSensitivity;
		float Zoom;
	
		float aspectRatio;
	public:
		Camera(glm::vec3 position, glm::vec3 up, float yaw, float pitch, float aspectratio);
	
		glm::mat4 GetViewMatrix();
		glm::mat4 GetProjectionMatrix(float nearPlane, float farPlane);
		glm::mat4 GetViewProjectionMatrix();
	
		void ProcessKeyboard(ECameraDirection direction, float deltaTime);
		void ProcessMouseMovement(float xoffset, float yoffset, bool constrainPitch = true);
		void ProcessMouseScroll(float yoffset);

		inline glm::vec3 GetPosition() const { return Position; }
		inline glm::vec3 GetFront() const { return Front; }
		inline glm::vec3 GetUp() const { return Up; }
		inline glm::vec3 GetRight() const { return Right; }
	private:
		void updateCameraVectors();
	};
}

