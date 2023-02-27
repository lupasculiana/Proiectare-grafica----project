#include "Camera.hpp"

namespace gps {

	//Camera constructor
	Camera::Camera(glm::vec3 cameraPosition, glm::vec3 cameraTarget, glm::vec3 cameraUp) : MouseSensitivity(SENSITIVTY)
	{
		this->cameraPosition = cameraPosition;
		this->cameraTarget = cameraTarget;
		this->cameraUpDirection = cameraUp;
		this->cameraFrontDirection = glm::normalize(cameraTarget - cameraPosition);
		this->cameraRightDirection = glm::normalize(glm::cross(this->cameraFrontDirection, glm::vec3(0.0f, 1.0f, 0.0f)));
	}
	//return the view matrix, using the glm::lookAt() function
	glm::mat4 Camera::getViewMatrix()
	{
		return glm::lookAt(cameraPosition, cameraPosition + cameraFrontDirection, glm::vec3(0.0f, 1.0f, 0.0f));
	}
	//return camera target
	glm::vec3 Camera::getCameraTarget()
	{
		return cameraTarget;
	}
	//update the camera internal parameters following a camera move event

	void Camera::move(MOVE_DIRECTION direction, float speed) {

		if (direction == MOVE_FORWARD) {
			cameraPosition -= cameraFrontDirection * speed;
			cameraTarget -= cameraFrontDirection * speed;
		}

		if (direction == MOVE_BACKWARD) {
			cameraPosition += cameraFrontDirection * speed;
			cameraTarget += cameraFrontDirection * speed;
		}

		if (direction == MOVE_LEFT) {
			cameraPosition -= cameraRightDirection * speed;
			cameraTarget -= cameraRightDirection * speed;
		}

		if (direction == MOVE_RIGHT) {
			cameraPosition += cameraRightDirection * speed;
			cameraTarget += cameraRightDirection * speed;
		}

		if (direction == MOVE_UP) {
			cameraPosition += cameraUpDirection * speed;
			cameraTarget += cameraUpDirection * speed;
		}

		if (direction == MOVE_DOWN) {
			cameraPosition -= cameraUpDirection * speed;
			cameraTarget -= cameraUpDirection * speed;
		}


	}

	void Camera::rotate(float yaw, float pitch, bool constrainPitch)
	{
		yaw *= MouseSensitivity;
		pitch *= MouseSensitivity;

		Yaw += yaw;
		Pitch += pitch;

		if (constrainPitch)
		{
			if (Pitch < -89.0f) {
				Pitch = -89.0f;
			}
			if (Pitch == 89.0f) {
				Pitch = 89.0;
			}

			if (Pitch > 89.0f) {
				Pitch = 89.0f;
			}

		}

		glm::vec3 frontVector;
		frontVector.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
		frontVector.y = sin(glm::radians(Pitch));
		frontVector.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
		cameraFrontDirection = glm::normalize(frontVector);

		cameraRightDirection = glm::normalize(glm::cross(cameraFrontDirection, glm::vec3(0.0f, 1.0f, 0.0f)));

		cameraUpDirection = glm::normalize(glm::cross(cameraUpDirection, glm::vec3(0.0f, 0.0f, 1.0f)));
	}

	void Camera::actualizeazaPozitiaCamerei(int x, int y, int z)
	{
		this->cameraPosition.x = x;
		this->cameraPosition.y = y;
		this->cameraPosition.z = z;
	}


}