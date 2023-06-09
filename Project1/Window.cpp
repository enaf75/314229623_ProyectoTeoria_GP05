#include "Window.h"

Window::Window()
{
	width = 800;
	height = 600;
	for (size_t i = 0; i < 1024; i++)
	{
		keys[i] = 0;
	}
}
Window::Window(GLint windowWidth, GLint windowHeight)
{
	width = windowWidth;
	height = windowHeight;
	muevex = 0.0f;
	for (size_t i = 0; i < 1024; i++)
	{
		keys[i] = 0;
	}
}
int Window::Initialise()
{
	//Inicialización de GLFW
	if (!glfwInit())
	{
		printf("Falló inicializar GLFW");
		glfwTerminate();
		return 1;
	}
	//Asignando variables de GLFW y propiedades de ventana
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	//para solo usar el core profile de OpenGL y no tener retrocompatibilidad
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	//CREAR VENTANA
	mainWindow = glfwCreateWindow(width, height, "Primer ventana", NULL, NULL);

	if (!mainWindow)
	{
		printf("Fallo en crearse la ventana con GLFW");
		glfwTerminate();
		return 1;
	}
	//Obtener tamaño de Buffer
	glfwGetFramebufferSize(mainWindow, &bufferWidth, &bufferHeight);

	//asignar el contexto
	glfwMakeContextCurrent(mainWindow);

	//MANEJAR TECLADO y MOUSE
	createCallbacks();


	//permitir nuevas extensiones
	glewExperimental = GL_TRUE;

	if (glewInit() != GLEW_OK)
	{
		printf("Falló inicialización de GLEW");
		glfwDestroyWindow(mainWindow);
		glfwTerminate();
		return 1;
	}

	glEnable(GL_DEPTH_TEST); //HABILITAR BUFFER DE PROFUNDIDAD
							 // Asignar valores de la ventana y coordenadas
							 
							 //Asignar Viewport
	glViewport(0, 0, bufferWidth, bufferHeight);
	//Callback para detectar que se está usando la ventana
	glfwSetWindowUserPointer(mainWindow, this);
}

void Window::createCallbacks()
{
	glfwSetKeyCallback(mainWindow, ManejaTeclado);
	glfwSetCursorPosCallback(mainWindow, ManejaMouse);
}
GLfloat Window::getXChange()
{
	GLfloat theChange = xChange;
	xChange = 0.0f;
	return theChange;
}

GLfloat Window::getYChange()
{
	GLfloat theChange = yChange;
	yChange = 0.0f;
	return theChange;
}

void Window::ManejaTeclado(GLFWwindow* window, int key, int code, int action, int mode)
{
	Window* theWindow = static_cast<Window*>(glfwGetWindowUserPointer(window));

	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
	}

	/* ***************************************************
	*				TECLAS DEL AVATAR (kirito)
	* ****************************************************
	*/
	
	/*
	* Casos de movimiento:
	* +X+Z
	* +X-Z
	* -X+Z
	* -X-Z
	* +X
	* -X
	* +Z
	* -Z
	*/
	if (key == GLFW_KEY_I) //Movimiento avanzar
	{
		//printf("angulo actual %f\n", theWindow->angulo);
		float ang = theWindow->angulo;
		
		//Movimientos rectos:
		if(ang == 0.0f) // 0 grados
		{
			theWindow->muevez += 2.0f;
			//theWindow->muevez += 0.1;
		}
		if (ang == 90.0f) // 0 grados
		{
			theWindow->muevex += 2.0f;
			//theWindow->muevex += 0.1;
		}
		if (ang == 180.0f) // 0 grados
		{
			theWindow->muevez -= 2.0f;
			//theWindow->muevez -= 0.1;
		}
		if (ang == 270.0f) // 0 grados
		{
			theWindow->muevex -= 2.0f;
			//theWindow->muevex -= 0.1;
		}

		//Movimientos en diagonal:
		if (ang > 0.0f && ang < 90.0f)
		{
			theWindow->muevez += 2.0f;
			theWindow->muevex += 2.0f;
			//theWindow->muevez += 0.1;
			//theWindow->muevex += 0.1;
		}
		if (ang > 90.0f && ang < 180.0f)
		{
			theWindow->muevez -= 2.0f;
			theWindow->muevex += 2.0f;
			//theWindow->muevez -= 0.1;
			//theWindow->muevex += 0.1;
		}
		if (ang > 180.0f && ang < 270.0f)
		{
			theWindow->muevez -= 2.0f;
			theWindow->muevex -= 2.0f;
			//theWindow->muevez -= 0.1;
			//theWindow->muevex -= 0.1;
		}
		if (ang > 270.0f)
		{
			theWindow->muevez += 2.0f;
			theWindow->muevex -= 2.0f;
			//theWindow->muevez += 0.1;
			//theWindow->muevex -= 0.1;
		}

		//theWindow->muevex += 0.1 * tan(theWindow->angulo);
		//theWindow->direction = glm::vec3(1.0f, 0.0f, 0.0f);
		//theWindow->color = glm::vec3(
		//	0.0f / 255,
		//	0.0f / 255,
		//	255.0f / 255); //azul teku
		//theWindow->bandera0 = true;
	}
	if (key == GLFW_KEY_K) //Movimiento retroceder
	{
		//theWindow->muevez -= 0.1;

		//printf("angulo actual %f\n", theWindow->angulo);
		float ang = theWindow->angulo;

		//Movimientos rectos:
		if (ang == 0.0f) // 0 grados
		{
			theWindow->muevez -= 2.0f;
			//theWindow->muevez -= 0.1;
		}
		if (ang == 90.0f) // 0 grados
		{
			theWindow->muevex -= 2.0f;
			//theWindow->muevex -= 0.1;
		}
		if (ang == 180.0f) // 0 grados
		{
			theWindow->muevez += 2.0f;
			//theWindow->muevez += 0.1;
		}
		if (ang == 270.0f) // 0 grados
		{
			theWindow->muevex += 2.0f;
			//theWindow->muevex += 0.1;
		}

		//Movimientos en diagonal:
		if (ang > 0.0f && ang < 90.0f)
		{
			theWindow->muevez -= 2.0f;
			theWindow->muevex -= 2.0f;
			//theWindow->muevez -= 0.1;
			//theWindow->muevex -= 0.1;
		}
		if (ang > 90.0f && ang < 180.0f)
		{
			theWindow->muevez += 2.0f;
			theWindow->muevex -= 2.0f;
			//theWindow->muevez += 0.1;
			//theWindow->muevex -= 0.1;
		}
		if (ang > 180.0f && ang < 270.0f)
		{
			theWindow->muevez += 2.0f;
			theWindow->muevex += 2.0f;
			//theWindow->muevez += 0.1;
			//theWindow->muevex += 0.1;
		}
		if (ang > 270.0f)
		{
			theWindow->muevez -= 2.0f;
			theWindow->muevex += 2.0f;
			//theWindow->muevez -= 0.1;
			//theWindow->muevex += 0.1;
		}
		//theWindow->muevex -= 0.1 * tan(theWindow->angulo);
		//theWindow->direction = glm::vec3(-1.0f, 0.0f, 0.0f);
		//theWindow->color = glm::vec3(
		//	255.0f / 255,
		//	0.0f / 255,
		//	0.0f / 255); //rojo reversa
		//theWindow->bandera0 = false;
	}
	//Ángulo de rotación
	if (key == GLFW_KEY_J && action == GLFW_PRESS) //Girar a la izquierda
	{
		if (theWindow->angulo + 45.0f == 360.0f)
		{
			theWindow->angulo = 0.0f;
		}
		else
		{
			theWindow->angulo += 45.0f;
		}
	}
	if (key == GLFW_KEY_L && action == GLFW_PRESS) //Girar a la derecha
	{
		if (theWindow->angulo == 0.0f)
		{
			theWindow->angulo = 360.0f - 45.0f;
		}
		else
		{
			theWindow->angulo -= 45.0f;
		}
	}

	/* ***************************************************
	*				TECLAS DE ACCIÓN
	* ****************************************************
	*/
	// Cambio de cámara
	if (key == GLFW_KEY_Q && action == GLFW_PRESS)
	{
		if (theWindow->camcam == true)
		{
			theWindow->camcam = false;
		}
		else
		{
			theWindow->camcam = true;
		}
	}
	//Cambio de día y noche
	if (key == GLFW_KEY_Z && action == GLFW_PRESS)
	{
		if (theWindow->daynight == true)
		{
			theWindow->daynight = false;
		}
		else
		{
			theWindow->daynight = true;
		}
	}



	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
		{
			theWindow->keys[key] = true;
			//printf("se presiono la tecla %d'\n", key);
		}
		else if (action == GLFW_RELEASE)
		{
			theWindow->keys[key] = false;
			//printf("se solto la tecla %d'\n", key);
		}
	}
}

void Window::ManejaMouse(GLFWwindow* window, double xPos, double yPos)
{
	Window* theWindow = static_cast<Window*>(glfwGetWindowUserPointer(window));

	if (theWindow->mouseFirstMoved)
	{
		theWindow->lastX = xPos;
		theWindow->lastY = yPos;
		theWindow->mouseFirstMoved = false;
	}

	theWindow->xChange = xPos - theWindow->lastX;
	theWindow->yChange = theWindow->lastY - yPos;

	theWindow->lastX = xPos;
	theWindow->lastY = yPos;
}


Window::~Window()
{
	glfwDestroyWindow(mainWindow);
	glfwTerminate();

}
