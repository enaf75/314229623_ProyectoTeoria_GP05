#pragma once
#include<stdio.h>
#include<glew.h>
#include<glfw3.h>
#include <glm.hpp>

class Window
{
public:
	Window();
	Window(GLint windowWidth, GLint windowHeight);
	int Initialise();
	GLfloat getBufferWidth() { return bufferWidth; }
	GLfloat getBufferHeight() { return bufferHeight; }
	GLfloat getXChange();
	GLfloat getYChange();
	//Movimiento (+-X +-Y +-Z)
	GLfloat getmuevex() { return muevex; }
	GLfloat getmuevey() { return muevey; }
	GLfloat getmuevez() { return muevez; }
	GLfloat getangulo() { return angulo; }
	int getdestructor() { return destructor; }
	int getDirCarro() { return dirCarro; }
	//Dirección de la luz
	glm::vec3 getLightDir() { return direction; }
	//Color de la luz
	glm::vec3 getLightColor() { return color; }

	void stopDaynight() { daynight = false; }
	void stopCamcam() { camcam = false; }
	//Bandera para cambiar el skybox
	bool getDaynight() { return daynight; }
	//Bandera para cambiar la cámara
	bool getCamcam() { return camcam; }
	bool getB_caminando() { return caminando; }
	//Bandera para cambiar la luz del carro
	//bool getBandera0() { return bandera0; }

	bool getShouldClose() {
		return  glfwWindowShouldClose(mainWindow);}
	bool* getsKeys() { return keys; }
	void swapBuffers() { return glfwSwapBuffers(mainWindow); }
	
	~Window();
private: 
	GLFWwindow *mainWindow;
	GLint width, height;
	bool keys[1024];
	GLint bufferWidth, bufferHeight;
	void createCallbacks();
	GLfloat lastX;
	GLfloat lastY;
	GLfloat xChange;
	GLfloat yChange;
	//Movimiento (+-X +-Y +-Z)
	GLfloat muevex = 0.0f;
	GLfloat muevey = 0.0f;
	GLfloat muevez = 0.0f;
	GLfloat angulo = 0.0f;
	//Dirección de la luz
	glm::vec3 direction = glm::vec3(1.0f, 0.0f, 0.0f);
	//Color de la luz
	glm::vec3 color = glm::vec3(0.0f, 0.0f, 1.0f);
	//Bandera para cambiar el skybox
	bool daynight = true;
	//Bandera para cambiar la cámara
	bool camcam = false;
	//Bandera para cambiar la cámara
	bool caminando = false;
	//Bandera para cambiar la luz del carro
	//bool bandera0 = true;
	int destructor = 0;
	int dirCarro = 0;
	bool mouseFirstMoved;
	static void ManejaTeclado(GLFWwindow* window, int key, int code, int action, int mode);
	static void ManejaMouse(GLFWwindow* window, double xPos, double yPos);

};

