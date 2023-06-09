#pragma warning(disable : 4996)
#define STB_IMAGE_IMPLEMENTATION

#include <stdio.h>
#include <string.h>
#include <cmath>
#include <vector>
#include <math.h>
#include <ctime>
#include <vector>

#include <glew.h>
#include <glfw3.h>

#include <glm.hpp>
#include <gtc\matrix_transform.hpp>
#include <gtc\type_ptr.hpp>

#include "Window.h"
#include "Mesh.h"
#include "Shader_light.h"
#include "CameraIso.h"
#include "Texture.h"
#include"Model.h"
#include "Skybox.h"

#include "CommonValues.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"
#include "Material.h"
const float toRadians = 3.14159265f / 180.0f;

//Animación de las luces del escenario
float movCoche;
float movOffset;
bool avanza;
/* ***************************************************
*				KEYFRAMES PARTE 0
* - Banderas de control.
* ****************************************************
*/
float reproduciranimacion, habilitaranimacion;

Window mainWindow;
std::vector<Mesh*> meshList;
std::vector<Shader> shaderList;

CameraIso cameraX;
CameraIso cameraP; //0
CameraIso cameraI; //1
CameraIso cameraL; //2
unsigned int b_camera = 0;
glm::vec3 t01 = glm::vec3(0.0f, 0.0f, -10.0f); //Posición inicial del personaje.

Texture pisoTexture;

Skybox skybox;

//materiales
Material Material_brillante;
Material Material_opaco;


//Sphere cabeza = Sphere(0.5, 20, 20);
GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;
static double limitFPS = 1.0 / 60.0;

/* **********
*	LUCES
*  **********
*/
// 1. luz direccional
DirectionalLight mainLight;
// 2. luces puntuales
PointLight pointLights[MAX_POINT_LIGHTS];
// 3. luces spotlight
SpotLight spotLights[MAX_SPOT_LIGHTS];

// Vertex Shader
static const char* vShader = "shaders/shader_light.vert";
// Fragment Shader
static const char* fShader = "shaders/shader_light.frag";

void calcAverageNormals(unsigned int* indices, unsigned int indiceCount, GLfloat* vertices, unsigned int verticeCount,
	unsigned int vLength, unsigned int normalOffset)
{
	for (size_t i = 0; i < indiceCount; i += 3)
	{
		unsigned int in0 = indices[i] * vLength;
		unsigned int in1 = indices[i + 1] * vLength;
		unsigned int in2 = indices[i + 2] * vLength;
		glm::vec3 v1(vertices[in1] - vertices[in0], vertices[in1 + 1] - vertices[in0 + 1], vertices[in1 + 2] - vertices[in0 + 2]);
		glm::vec3 v2(vertices[in2] - vertices[in0], vertices[in2 + 1] - vertices[in0 + 1], vertices[in2 + 2] - vertices[in0 + 2]);
		glm::vec3 normal = glm::cross(v1, v2);
		normal = -glm::normalize(normal);
		//normal = glm::normalize(normal);

		in0 += normalOffset; in1 += normalOffset; in2 += normalOffset;
		vertices[in0] += normal.x; vertices[in0 + 1] += normal.y; vertices[in0 + 2] += normal.z;
		vertices[in1] += normal.x; vertices[in1 + 1] += normal.y; vertices[in1 + 2] += normal.z;
		vertices[in2] += normal.x; vertices[in2 + 1] += normal.y; vertices[in2 + 2] += normal.z;
	}

	for (size_t i = 0; i < verticeCount / vLength; i++)
	{
		unsigned int nOffset = i * vLength + normalOffset;
		glm::vec3 vec(vertices[nOffset], vertices[nOffset + 1], vertices[nOffset + 2]);
		vec = glm::normalize(vec);
		vertices[nOffset] = vec.x; vertices[nOffset + 1] = vec.y; vertices[nOffset + 2] = vec.z;
	}
}

void CreateObjects()
{
	unsigned int indices[] = {
		0, 3, 1,
		1, 3, 2,
		2, 3, 0,
		0, 1, 2
	};

	GLfloat vertices[] = {
		//	x      y      z			u	  v			nx	  ny    nz
			-1.0f, -1.0f, -0.6f,	0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
			0.0f, -1.0f, 1.0f,		0.5f, 0.0f,		0.0f, 0.0f, 0.0f,
			1.0f, -1.0f, -0.6f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f,		0.5f, 1.0f,		0.0f, 0.0f, 0.0f
	};



	unsigned int floorIndices[] = {
		0, 2, 1,
		1, 2, 3
	};

	GLfloat floorVertices[] = {
		-10.0f, 0.0f, -10.0f,	0.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		10.0f, 0.0f, -10.0f,	10.0f, 0.0f,	0.0f, -1.0f, 0.0f,
		-10.0f, 0.0f, 10.0f,	0.0f, 10.0f,	0.0f, -1.0f, 0.0f,
		10.0f, 0.0f, 10.0f,		10.0f, 10.0f,	0.0f, -1.0f, 0.0f
	};

	unsigned int vegetacionIndices[] = {
	   0, 1, 2,
	   0, 2, 3,
	   4,5,6,
	   4,6,7
	};

	GLfloat vegetacionVertices[] = {
		-0.5f, -0.5f, 0.0f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.5f, -0.5f, 0.0f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.5f, 0.5f, 0.0f,		1.0f, 1.0f,		0.0f, 0.0f, 0.0f,
		-0.5f, 0.5f, 0.0f,		0.0f, 1.0f,		0.0f, 0.0f, 0.0f,

		0.0f, -0.5f, -0.5f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.0f, -0.5f, 0.5f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.0f, 0.5f, 0.5f,		1.0f, 1.0f,		0.0f, 0.0f, 0.0f,
		0.0f, 0.5f, -0.5f,		0.0f, 1.0f,		0.0f, 0.0f, 0.0f,


	};


	
	Mesh *obj1 = new Mesh();
	obj1->CreateMesh(vertices, indices, 32, 12);
	meshList.push_back(obj1);

	Mesh *obj2 = new Mesh();
	obj2->CreateMesh(vertices, indices, 32, 12);
	meshList.push_back(obj2);

	Mesh *obj3 = new Mesh();
	obj3->CreateMesh(floorVertices, floorIndices, 32, 6);
	meshList.push_back(obj3);


	Mesh* obj4 = new Mesh();
	obj4->CreateMesh(vegetacionVertices, vegetacionIndices, 64, 12);
	meshList.push_back(obj4);

	calcAverageNormals(indices, 12, vertices, 32, 8, 5);

	calcAverageNormals(vegetacionIndices, 12, vegetacionVertices, 64, 8, 5);


}

void CreateShaders()
{
	Shader *shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);
}

/* ***************************************************
*				KEYFRAMES PARTE 1
* ****************************************************
*/
//PARA INPUT CON KEYFRAMES 
void inputKeyframes(bool* keys);

bool animacion = false;

//NEW// Keyframes
float posXavion = 2.0f;
float posYavion = 5.0f;
float posZavion = -3.0f;
float movAvion_x = 0.0f;
float movAvion_y = 0.0f;
float giroAvion = 0;

float giroK_BI = 0.0f;
float giroK_BD = 0.0f;
float giroK_PI = 0.0f;
float giroK_PD = 0.0f;

int index = 7;

#define MAX_FRAMES 30 //Número de frames máximo.
int i_max_steps = 90; //Indica el número de interpolaciones, mientras más grande el número más fulida la animación y más recursos consume.
int i_curr_steps = index; //Indica a partir de qué indice del frame voy a guardar datos, depende del número de frames que tengamos establecidos, aquí ya tenemos 0-4 frames establecidos anteriormente y que no queremos sobreescribir.
typedef struct _frame
{
	//Variables para GUARDAR Key Frames
	float movAvion_x;		//Variable para PosicionX //Variable primaria.
	float movAvion_y;		//Variable para PosicionY //Variable primaria.
	float movAvion_xInc;		//Variable para IncrementoX //Variable secundaria que guarda los datos de las interpolaciones.
	float movAvion_yInc;		//Variable para IncrementoY //Variable secundaria que guarda los datos de las interpolaciones.
	float giroAvion;
	float giroAvionInc;

	float giroK_BI;
	float giroK_BI_Inc;
	float giroK_BD;
	float giroK_PI;
	float giroK_PD;
}FRAME;

FRAME KeyFrame[MAX_FRAMES];
int FrameIndex = index;			//introducir datos
bool play = false;
int playIndex = 0;

void resetElements(void)
{
	movAvion_x = KeyFrame[0].movAvion_x;
	movAvion_y = KeyFrame[0].movAvion_y;
	giroAvion = KeyFrame[0].giroAvion;
	giroK_BI = KeyFrame[0].giroK_BI;
}

void interpolation(void)
{
	//frame siguiente // frame actual // número de pasos
	KeyFrame[playIndex].movAvion_xInc = (KeyFrame[playIndex + 1].movAvion_x - KeyFrame[playIndex].movAvion_x) / i_max_steps;
	KeyFrame[playIndex].movAvion_yInc = (KeyFrame[playIndex + 1].movAvion_y - KeyFrame[playIndex].movAvion_y) / i_max_steps;
	KeyFrame[playIndex].giroAvionInc = (KeyFrame[playIndex + 1].giroAvion - KeyFrame[playIndex].giroAvion) / i_max_steps;
	KeyFrame[playIndex].giroK_BI_Inc = (KeyFrame[playIndex + 1].giroK_BI - KeyFrame[playIndex].giroK_BI) / i_max_steps;
}

/*
* Cuando play está en 1 se reproduce la animación.
*/
void animate(void)
{
	//Movimiento del objeto
	if (play)
	{
		//Esto evita que se pase del número de frames que tenemos.
		if (i_curr_steps >= i_max_steps) //end of animation between frames?
		{
			playIndex++;
			//printf("playindex : %d\n", playIndex);
			if (playIndex > FrameIndex - 2)	//end of total animation?
			{
				//printf("Frame index= %d\n", FrameIndex);
				//printf("termina animación\n");
				playIndex = 0;
				play = false;
			}
			else //Next frame interpolations
			{
				//printf("entro aquí\n");
				i_curr_steps = 0; //Reset counter
				//Interpolation
				interpolation();
			}
		}
		else
		{
			movAvion_x += KeyFrame[playIndex].movAvion_xInc;
			movAvion_y += KeyFrame[playIndex].movAvion_yInc;
			giroAvion += KeyFrame[playIndex].giroAvionInc;
			giroK_BI += KeyFrame[playIndex].giroK_BI_Inc;
			i_curr_steps++;
		}

	}
}
/* FIN KEYFRAMES*/


int main()
{
	mainWindow = Window(1366, 768); // 1280, 1024 or 1024, 768
	mainWindow.Initialise();

	CreateObjects();	
	CreateShaders();

	/* ***************************************************
	*			CÁMARA 3RA PERSONA
	* ****************************************************
	*/
	glm::vec3 dirCam = glm::vec3(0.0f, 0.0f, 1.0f); 
	cameraP = CameraIso(
		glm::vec3(0.0f, 5.0f, 10.0f), //posición inicial
		glm::vec3(0.0f, 1.0f, 0.0f), //vector hacia arriba del mundo
		0.0f, //startyaw ángulo de la visión en horizontal (- izquierda, + derecha)
		0.0f, //startPitch ángulo de la visión en vertical (- abajo, + arriba)
		0.5f, //velocidad de movimiento con las teclas
		2.0f, //velocidad de giro con el mouse
		0); //tipo de cámara

	/* ***************************************************
	*			CÁMARA ISOMÉTRICA
	* ****************************************************
	*/
	glm::vec3 posCam1 = glm::vec3(0.0f, 70.0f, 0.0f); //Posición de la cámara aérea.
	cameraI = CameraIso(
		posCam1, //posición inicial
		glm::vec3(0.0f, 1.0f, 0.0f), //vector hacia arriba del mundo
		-60.0f, //startyaw ángulo de la visión en horizontal//(- izq, + der)
		-20.0f, //startPitch ángulo de la visión en vertical (- abajo, + arriba)
		0.5f, //velocidad de movimiento con las teclas
		2.0f, //velocidad de giro con el mouse
		1); //tipo de cámara
	
	cameraX = cameraP;
	
	/* ***************************************************
	*			CÁMARA LIBRE
	* ****************************************************
	*/
	glm::vec3 posCam2 = glm::vec3(0.0f, 70.0f, 0.0f); //Posición de la cámara aérea.
	glm::vec3 dirCam2 = glm::vec3(0.0f, 0.0f, -1.0f);
	cameraL = CameraIso(
		glm::vec3(0.0f, 50.0f, 10.0f), //posición inicial
		glm::vec3(0.0f, 5.0f, 0.0f), //vector hacia arriba del mundo
		-60.0f, //startyaw ángulo de la visión en horizontal//(- izq, + der)
		0.0f, //startPitch ángulo de la visión en vertical (- abajo, + arriba)
		2.0f, //velocidad de movimiento con las teclas
		1.0f, //velocidad de giro con el mouse
		2); //tipo de cámara

	/* ***************************************************
	*		IMPORTACIÓN DE TEXTURAS Y MODELOS
	* ****************************************************
	*/
	pisoTexture = Texture("Textures/piso_pasto.tga");
	pisoTexture.LoadTextureA();
	
	//Modelo del avatar (kirito)
	
	Model KiritoCE_M = Model();
	KiritoCE_M.LoadModel("Models/kirito_centro.obj");
	Model KiritoBD_M = Model();
	KiritoBD_M.LoadModel("Models/kirito_brazo_der.obj");
	Model KiritoBI_M = Model();
	KiritoBI_M.LoadModel("Models/kirito_brazo_izq.obj");
	Model KiritoPD_M = Model();
	KiritoPD_M.LoadModel("Models/kirito_pierna_der.obj");
	Model KiritoPI_M = Model();
	KiritoPI_M.LoadModel("Models/kirito_pierna_izq.obj");

	//Modelos Casa
	Model Casa = Model();
	Casa.LoadModel("Models/casafinal.obj");
	
	/* ***************************************************
	*				TEXTURAS DE SKYBOX
	* ****************************************************
	*/
	bool b_daynight = false;
	bool b_cambio = false;
	bool b_tiempo = true;
	std::vector<std::string> skyboxFacesD;
	skyboxFacesD.push_back("Textures/Skybox/dia_px.tga"); //right
	skyboxFacesD.push_back("Textures/Skybox/dia_nx.tga"); //left
	skyboxFacesD.push_back("Textures/Skybox/dia_ny.tga"); //down
	skyboxFacesD.push_back("Textures/Skybox/dia_py.tga"); //up
	skyboxFacesD.push_back("Textures/Skybox/dia_pz.tga"); //front
	skyboxFacesD.push_back("Textures/Skybox/dia_nz.tga"); //back
	std::vector<std::string> skyboxFacesN;
	skyboxFacesN.push_back("Textures/Skybox/noche_px.tga"); //right
	skyboxFacesN.push_back("Textures/Skybox/noche_nx.tga"); //left
	skyboxFacesN.push_back("Textures/Skybox/noche_ny.tga"); //down
	skyboxFacesN.push_back("Textures/Skybox/noche_py.tga"); //up
	skyboxFacesN.push_back("Textures/Skybox/noche_pz.tga"); //front
	skyboxFacesN.push_back("Textures/Skybox/noche_nz.tga"); //back

	skybox = Skybox(skyboxFacesD);

	Material_brillante = Material(4.0f, 256);
	Material_opaco = Material(0.3f, 4);

	/* ***************************************************
	*				CREACIÓN DE LUCES
	* ****************************************************
	*/
	//1. LUZ DIRECCIONAL (sólo 1 y siempre debe de existir)
	mainLight = DirectionalLight(
		1.0f, 1.0f, 1.0f, //color de la luz 
		0.8f, 0.3f, //intensidad ambiental (1.0f = día soleado, 0.0f  = noche), intensidad difusa (parece ser el nivel de brillo del sol que reflejan los objetos)
		0.0f, 0.0f, -1.0f); //vector de dirección del sol
	
	//2. LUZ PUNTUAL
	//contador de luces puntuales
	unsigned int pointLightCount = 0;
	//pointLights[0] = PointLight(
	//	1.0f, 0.0f, 0.0f, //color
	//	1.5f, 1.5f, //ambiental y difusa
	//	20.0f, 1.5f, 0.5f,
	//	0.3f, 0.2f, 0.1f);
	//pointLightCount++;

	//3. LUZ SPOTLIGHT
	unsigned int spotLightCount = 0;

	spotLights[0] = SpotLight(
		254.0f / 255, 1.0f / 255, 143.0f / 255,//luz color azul
		0.2f, 1.0f, //ambiental y difusa
		12.0f, 10.0f, -127.0f, //posición
		0.0f, 0.0f, 0.0f, //vector de dirección
		0.3f, 0.0f, 0.0f, //ecuación (constante,lineal,exponencial)
		8.0f); //ángulo de apertura
	spotLightCount++;

	spotLights[1] = SpotLight(
		0.0f, 1.0f, 1.0f,//luz color azul
		0.2f, 1.0f, //ambiental y difusa
		37.0f, 10.0f, -127.0f, //posición
		0.0f, 0.0f, 0.0f, //vector de dirección
		0.3f, 0.0f, 0.0f, //ecuación (constante,lineal,exponencial)
		8.0f); //ángulo de apertura
	spotLightCount++;

	spotLights[2] = SpotLight(
		0.0f, 1.0f, 1.0f,//luz color azul
		1.0f, 1.0f, //ambiental y difusa
		-13.0f, 10.0f, -127.0f, //posición
		0.0f, 0.0f, 0.0f, //vector de dirección
		0.3f, 0.0f, 0.0f, //ecuación (constante,lineal,exponencial)
		8.0f); //ángulo de apertura
	spotLightCount++;

	spotLights[3] = SpotLight(
		254.0f / 255, 1.0f / 255, 143.0f / 255,//luz color azul
		1.0f, 1.0f, //ambiental y difusa
		-38.0f, 10.0f, -127.0f, //posición
		0.0f, 0.0f, 0.0f, //vector de dirección
		0.3f, 0.0f, 0.0f, //ecuación (constante,lineal,exponencial)
		8.0f); //ángulo de apertura
	spotLightCount++;

	/* ***************************************************
	*						???
	* ****************************************************
	*/
	GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0,
		uniformEyePosition = 0, uniformSpecularIntensity = 0, uniformShininess = 0;
	GLuint uniformColor = 0;
	glm::mat4 projection =
		glm::perspective(45.0f, (GLfloat)mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 1000.0f);
	
	//Animación de las luces del escenario
	movCoche = 0.2f;
	movOffset = 0.005f;
	avanza = true;

	/* ***************************************************
	*				KEYFRAMES PARTE 2
	* ****************************************************
	*/
	glm::vec3 posblackhawk = glm::vec3(2.0f, 0.0f, 0.0f);
	KeyFrame[0].movAvion_x = 0.0f;
	KeyFrame[0].movAvion_y = 0.0f;
	KeyFrame[0].giroAvion = 0;
	KeyFrame[0].giroK_BI = 0.0f;

	KeyFrame[1].movAvion_x = 1.0f;
	KeyFrame[1].movAvion_y = 2.0f;
	KeyFrame[1].giroAvion = 0;
	KeyFrame[1].giroK_BI = 180.0f;


	KeyFrame[2].movAvion_x = 2.0f;
	KeyFrame[2].movAvion_y = 0.0f;
	KeyFrame[2].giroAvion = 0;


	KeyFrame[3].movAvion_x = 3.0f;
	KeyFrame[3].movAvion_y = -2.0f;
	KeyFrame[3].giroAvion = 0;

	KeyFrame[4].movAvion_x = 4.0f;
	KeyFrame[4].movAvion_y = 0.0f;
	KeyFrame[4].giroAvion = 0.0f;
	//KeyFrame[4].giroAvion = 180.0f;

	//Agregar Kefyrame[5] para que el avión regrese al inicio

	KeyFrame[5].movAvion_x = 4.0f;
	KeyFrame[5].movAvion_y = 0.0f;
	KeyFrame[5].giroAvion = 180.0f;

	/* ***************************************************
	*	Loop mientras no se cierra la ventana
	* ****************************************************
	*/
	while (!mainWindow.getShouldClose())
	{
		GLfloat now = glfwGetTime();
		deltaTime = now - lastTime;
		deltaTime += (now - lastTime) / limitFPS;
		lastTime = now;

		// ***** Inicio de la animación del escenario *****
		if (avanza)
		{
			if (movCoche >= 0.0f)
			{
				movCoche -= movOffset * deltaTime;

				//printf("avanza%f \n ",movCoche);
			}
			else
			{
				avanza = false;
			}
		}
		else
		{
			if (movCoche < 0.2f)
			{
				movCoche += movOffset * deltaTime;

				//printf("avanza%f \n ",movCoche);
			}
			else
			{
				avanza = true;
			}
		}
		glm::vec3 temporal = glm::vec3(0.0f, movCoche, -1.0f);

		spotLights[0].SetDir(temporal);
		spotLights[1].SetDir(temporal);
		spotLights[2].SetDir(temporal);
		spotLights[3].SetDir(temporal);
		// ***** Fin la animación del escenario *****
		
		//Recibir eventos del usuario
		glfwPollEvents();
		cameraX.keyControl(mainWindow.getsKeys(), deltaTime);
		cameraX.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());

		// Clear the window
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		skybox.DrawSkybox(cameraX.calculateViewMatrix(), projection);
		shaderList[0].UseShader();
		uniformModel = shaderList[0].GetModelLocation();
		uniformProjection = shaderList[0].GetProjectionLocation();
		uniformView = shaderList[0].GetViewLocation();
		uniformEyePosition = shaderList[0].GetEyePositionLocation();
		uniformColor = shaderList[0].getColorLocation();
		
		//información en el shader de intensidad especular y brillo
		uniformSpecularIntensity = shaderList[0].GetSpecularIntensityLocation();
		uniformShininess = shaderList[0].GetShininessLocation();

		glUniformMatrix4fv(
			uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(
			uniformView, 1, GL_FALSE, glm::value_ptr(cameraX.calculateViewMatrix()));
		glUniform3f(
			uniformEyePosition, 
			cameraX.getCameraPosition().x, 
			cameraX.getCameraPosition().y, 
			cameraX.getCameraPosition().z);
		
		/* ***************************************************
		*	CONTROL DEL TIEMPO DEL HELICÓPTERO (SHADER)
		* ****************************************************
		*/

		time_t currentTime;
		struct tm* localTime;
		time(&currentTime); // Get the current time
		localTime = localtime(&currentTime);
		int Sec = localTime->tm_sec;
		//std::cout << "This program was exectued at: " << Sec << std::endl;
		if (Sec == 0 || Sec == 30)
		{
			b_cambio = true;
		}
		if (Sec == 1 || Sec == 31)
		{
			b_tiempo = true;
		}

		/* ***************************************************
		*			CICLO DE DÍA Y LA NOCHE
		* ****************************************************
		*/
		//if (mainWindow.getDaynight() == true) {
		if (b_tiempo == true)
		{
			if (b_cambio == true) {
				if (b_daynight == true)
				{
					skybox = Skybox(skyboxFacesN);
					b_daynight = false;
					mainLight.SetIntensity(glm::vec2(0.4f, 0.1f));
				}
				else
				{
					skybox = Skybox(skyboxFacesD);
					b_daynight = true;
					mainLight.SetIntensity(glm::vec2(0.8f, 0.3f));
				}
				//mainWindow.stopDaynight();
				b_cambio = false;
			}
			b_tiempo = false;
		}

		/* ***************************************************
		*			CAMBIO DE CÁMARA
		* ****************************************************
		*/
		if (mainWindow.getCamcam() == true) {
			if (b_camera == 0)
			{
				b_camera = 1;
				cameraX = cameraI;
				cameraX.SetPos(posCam1);
				cameraX.SetDir(dirCam);
			}
			else if (b_camera == 1)
			{
				posCam1 = cameraX.getCameraPosition();
				b_camera = 2;
				cameraX = cameraL;
				cameraX.SetPos(posCam2);
				cameraX.SetDir(dirCam2);
			}
			else if (b_camera == 2)
			{
				posCam2 = cameraX.getCameraPosition();
				dirCam2 = cameraX.getCameraDirection();
				b_camera = 0;
				cameraX = cameraP;
			}
			mainWindow.stopCamcam();
		}

		
		/* ***************************************************
		*				SHADER DE LUCES
		* ****************************************************
		*/
		//0. informa al shader de fuentes de iluminación
		shaderList[0].SetDirectionalLight(&mainLight);
		shaderList[0].SetPointLights(pointLights, pointLightCount);
		shaderList[0].SetSpotLights(spotLights, spotLightCount);
		
		/* ***************************************************
		*					???
		* ****************************************************
		*/
		glm::mat4 model(1.0);
		glm::mat4 modelaux(1.0);
		glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f);

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(30.0f, 1.0f, 30.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));

		pisoTexture.UseTexture();		
		meshList[2]->RenderMesh();

		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess); 
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);

		/* ***************************************************
		*					MODELOS DE PRUEBA
		* ****************************************************
		*/		


		
		glEnable(GL_BLEND);//CANAL ALPHA (blending: transparencia o traslucidez)
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		
		glDisable(GL_BLEND);

		

		////VIDRIOS APARTIR DE AQUI
		////VIDRIOS APARTIR DE AQUI
		////VIDRIOS APARTIR DE AQUI

		glEnable(GL_BLEND);//CANAL ALPHA (blending: transparencia o traslucidez)
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		
		glDisable(GL_BLEND);//FIN DEL CANAL ALPHA (blending: transparencia o traslucidez)

		/* ***************************************************
		*				Inicio del avatar (kirito)
		* 
		* -20 grados en z paso hacia adelante.
		* +20 grados en z paso hacia atrás.
		* ****************************************************
		*/
		//glm::vec3 t01 = glm::vec3(0.0f, 0.0f, -10.0f);
		
		t01.z = mainWindow.getmuevez();
		t01.x = mainWindow.getmuevex();
		float avatar_ang = mainWindow.getangulo();
		float e01 = 25.0f;

		model = glm::mat4(1.0);
		model = glm::translate(model, t01);
		modelaux = model;
		model = glm::rotate(model, avatar_ang * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));		
		model = glm::scale(model, glm::vec3(e01, e01, e01));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		KiritoCE_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, t01);
		model = glm::rotate(model, avatar_ang * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(e01, e01, e01));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		KiritoBI_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, t01);
		model = glm::rotate(model, avatar_ang * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(e01, e01, e01));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		KiritoBD_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, t01);		
		model = glm::rotate(model, avatar_ang * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(e01, e01, e01));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		KiritoPI_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, t01);		
		model = glm::rotate(model, avatar_ang * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(e01, e01, e01));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		KiritoPD_M.RenderModel();
		
		//Casa//
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 0.5f, 0.0f));
		model = glm::scale(model, glm::vec3(40.0f, 30.0f, 30.0f));
		// = glm::scale(model, glm::vec3(e01, e01, e01));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Casa.RenderModel();


		/*
		* Cámara en 3ra persona.
		* - La matriz model guardará las coordenadas que debe tomar la posición
		*   de la cámara, solo hay que extraerlas de la matriz.
		*/
		model = glm::mat4(1.0);
		model = glm::translate(model, t01);
		model = glm::rotate(model, avatar_ang * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		//Distancia a la que estará la cámara del personaje:
		model = glm::translate(model, glm::vec3(0.0f, 40.0f, -30.0f)); //+X izquierda, -X derecha, +Z adelande, - Z detrás de, ... kirito
		
		//Si está en modo de cámara de 3ra persona:
		if (b_camera == 0)
		{
			glm::vec3 posCam = glm::vec3(model[3][0], model[3][1]+5.0f, model[3][2]);
			cameraX.SetPos(posCam);
			glm::vec3 pdirCam = glm::vec3(modelaux[3][0], modelaux[3][1]+40.0f, modelaux[3][2]);
			cameraX.SetTarget(pdirCam);
		}
		
		/*
		* m[0] : columna 0
		* m[0][1] : columna 0, renglón 1
		*    0 1 2 3
		*
		* 0  1 0 0 0   (ABCD) =
		* 1  0 1 0 0 . (EFGH) =
		* 2  0 0 1 0   (IJKL) =
		* 3  0 0 0 1   (MNÑO) =
		* 
		* Los resultados se guardan en la columna 3, renglones 0 a 2.
		* https://learnopengl.com/Getting-started/Transformations
		*/
		

		glUseProgram(0);

		mainWindow.swapBuffers();
	}

	return 0;
}

/* ***************************************************
*				KEYFRAMES PARTE 3
* - Segundo teclado, para las banderas de control.
* ****************************************************
*/
/*
* Segundo teclado.
* - Para las banderas de control.
*/
void inputKeyframes(bool* keys)
{
	if (keys[GLFW_KEY_SPACE])
	{
		if (reproduciranimacion < 1)
		{
			if (play == false && (FrameIndex > 1))
			{
				resetElements();
				//First Interpolation				
				interpolation();
				play = true;
				playIndex = 0;
				i_curr_steps = 0;
				reproduciranimacion++;				
				habilitaranimacion = 0;
			}
			else
			{
				play = false;
			}
		}
	}
}