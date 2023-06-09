#include "CameraIso.h"
/*
* Tipos de cámara:
* 0. Cámara en 3ra persona.
* 1. Cámara aérea.
* 
* Referencias:
* https://learnopengl.com/Getting-started/Camera
*/
CameraIso::CameraIso() {}

CameraIso::CameraIso(
	glm::vec3 startPosition, glm::vec3 startUp,
	GLfloat startYaw, GLfloat startPitch,
	GLfloat startMoveSpeed, GLfloat startTurnSpeed,
	unsigned int startTipo)
{
	position = startPosition;
	worldUp = startUp;
	yaw = startYaw;
	pitch = startPitch;
	direction = glm::vec3(0.0f, 0.0f, -1.0f); //front

	moveSpeed = startMoveSpeed;
	turnSpeed = startTurnSpeed;
	tipo = startTipo;
	update();
}

void CameraIso::SetPos(glm::vec3 pos)
{
	position = pos; //+Z adelante de kirito,  -Z atrás de kirito
}

void CameraIso::SetTarget(glm::vec3 tar)
{
	target = tar;
	update();
}

void CameraIso::SetDir(glm::vec3 dir)
{
	direction = dir;
	update();
}

void CameraIso::keyControl(bool* keys, GLfloat deltaTime)
{
	GLfloat velocity = moveSpeed * deltaTime;
	//printf("MOVIMIENTO TIPO: %d\n", tipo);
	if (tipo == 1) 
	{
		if (keys[GLFW_KEY_W])
		{
			position.x += velocity;
			position.z -= velocity;
		}
		if (keys[GLFW_KEY_S])
		{
			position.x -= velocity;
			position.z += velocity;
		}
		if (keys[GLFW_KEY_A])
		{
			position.z -= velocity;
			position.x -= velocity;
		}
		if (keys[GLFW_KEY_D])
		{
			position.z += velocity;
			position.x += velocity;
		}
	}
	else if (tipo == 2)
	{
		if (keys[GLFW_KEY_W])
		{
			position += direction * velocity;
		}

		if (keys[GLFW_KEY_S])
		{
			position -= direction * velocity;
		}

		if (keys[GLFW_KEY_A])
		{
			position -= camRight * velocity;
		}

		if (keys[GLFW_KEY_D])
		{
			position += camRight * velocity;
		}
	}
}

void CameraIso::mouseControl(GLfloat xChange, GLfloat yChange)
{
	if (tipo == 2)
	{
		xChange *= turnSpeed;
		yChange *= turnSpeed;

		yaw += xChange;
		pitch += yChange;

		if (pitch > 89.0f)
		{
			pitch = 89.0f;
		}
		if (pitch < -89.0f)
		{
			pitch = -89.0f;
		}
		update();
	}
}

glm::mat4 CameraIso::calculateViewMatrix()
{

	//const float radius = 10.0f;
	//float camX = sin(glfwGetTime()) * radius;
	//float camZ = cos(glfwGetTime()) * radius;
	//glm::vec3 xx = glm::vec3(position.x + camX, position.y, position.z + camX);
	if (tipo == 0)
	{
		return glm::lookAt(position, target, camUp);
	}
	else if (tipo == 1)
	{
		return glm::lookAt(position, position + direction, camUp);
	}
	else if (tipo == 2)
	{
		return glm::lookAt(position, position + direction, camUp);
	}
	
}

glm::vec3 CameraIso::getCameraPosition()
{
	/*profesor*/return position;
}


glm::vec3 CameraIso::getCameraDirection()
{
	/*profesor*/return glm::normalize(direction);
}

GLfloat CameraIso::getYaw()
{
	return yaw;

}
GLfloat CameraIso::getPitch()
{
	return pitch;
}

void CameraIso::update()
{	
	if (tipo == 0)
	{
		/*learnopengl*/direction = glm::normalize(position - target);
		/*learnopengl*/camRight = glm::normalize(glm::cross(worldUp, direction));
		/*learnopengl*/camUp = glm::normalize(glm::cross(direction, camRight));
	}
	else if (tipo == 1)
	{
		direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
		direction.y = sin(glm::radians(pitch));
		direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
		direction = glm::normalize(direction);

		camRight = glm::normalize(glm::cross(direction, worldUp));
		camUp = glm::normalize(glm::cross(camRight, direction));
	}
	else if (tipo == 2)
	{
		direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
		direction.y = sin(glm::radians(pitch));
		direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
		direction = glm::normalize(direction);

		camRight = glm::normalize(glm::cross(direction, worldUp));
		camUp = glm::normalize(glm::cross(camRight, direction));
	}
}


CameraIso::~CameraIso()
{
}
