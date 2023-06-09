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
	glm::vec3 position; //Posici�n de la c�mara XYZ.
	glm::vec3 target; //Punto hacia el que apunta la c�mara XYZ.
	glm::vec3 direction; //Vector de direcci�n de la c�mara, no se define directamente, se calcula.
	glm::vec3 camUp; //Vector de direcci�n hacia el arriba de la c�mara.
	glm::vec3 camRight;
	glm::vec3 worldUp;//Vector de direcci�n hacia el arriba del mundo.

	GLfloat yaw; //�ngulo de la visi�n en horizontal//(- izq, + der)
	GLfloat pitch; //�ngulo de la visi�n en vertical (- abajo, + arriba)

	GLfloat moveSpeed; //Velocidad de movimiento con las teclas.
	GLfloat turnSpeed; //Velocidad de movimiento con el mouse.
	unsigned int tipo; //Tipo de c�mara (3ra persona, aerea)
	void update();
};

