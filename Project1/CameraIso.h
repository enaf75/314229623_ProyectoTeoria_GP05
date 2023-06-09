#pragma once

#include <glew.h>

#include <glm.hpp>
#include <gtc\matrix_transform.hpp>

#include <glfw3.h>

class CameraIso
{
public:
	CameraIso();
	CameraIso(glm::vec3 startPosition, glm::vec3 startUp,
		GLfloat startYaw, GLfloat startPitch,
		GLfloat startMoveSpeed, GLfloat startTurnSpeed,
		unsigned int tipo);

	void keyControl(bool* keys, GLfloat deltaTime);
	void mouseControl(GLfloat xChange, GLfloat yChange);


	glm::vec3 getCameraPosition();
	glm::vec3 getCameraDirection();
	GLfloat getYaw();
	GLfloat getPitch();
	glm::mat4 calculateViewMatrix();
	void SetPos(glm::vec3 pos);
	void SetTarget(glm::vec3 tar);
	void SetDir(glm::vec3 dir);
	~CameraIso();

private:
	glm::vec3 position; //Posición de la cámara XYZ.
	glm::vec3 target; //Punto hacia el que apunta la cámara XYZ.
	glm::vec3 direction; //Vector de dirección de la cámara, no se define directamente, se calcula.
	glm::vec3 camUp; //Vector de dirección hacia el arriba de la cámara.
	glm::vec3 camRight;
	glm::vec3 worldUp;//Vector de dirección hacia el arriba del mundo.

	GLfloat yaw; //Ángulo de la visión en horizontal//(- izq, + der)
	GLfloat pitch; //Ángulo de la visión en vertical (- abajo, + arriba)

	GLfloat moveSpeed; //Velocidad de movimiento con las teclas.
	GLfloat turnSpeed; //Velocidad de movimiento con el mouse.
	unsigned int tipo; //Tipo de cámara (3ra persona, aerea)
	void update();
};

