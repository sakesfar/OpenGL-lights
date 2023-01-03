#include <iostream>
#include <complex>
#include <vector>

#include <glad/glad.h>
#include <glfw3.h>

#include <glm/glm/glm.hpp>
#include <glm/glm/gtc/matrix_transform.hpp>
#include <glm/glm/gtc/type_ptr.hpp>

#include <random> // for std::mt19937
#include <ctime> // for std::time

#include "SHADER_H.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"



glm::vec3 cameraPos = glm::vec3(0.0f, 0.0, 0.0);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

//float lastX = 400, lastY = 300;
float yaw{ -90.0f }, pitch;
float lastX = 400, lastY = 300;
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
bool firstMove = true;

float deltaTime = 0.0f; // Time between current frame and last frame
float lastFrame = 0.0f; // Time of last frame



void processInput(GLFWwindow* window);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);

int main()
{
	glfwInit(); // Initializes glfW library
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(800, 600, "GLF Window", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window\n";
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window); // Making this Window's context current for the calling thread


	// Glad manages function pointers to OpenGL functions --> Initializw GLAD
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	//Telling OpenGL the size of the rendering window
	glViewport(0, 0, 800, 600);
	//Once the user resized window, viewPort should readjust itself---->
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);


	float vertices[]{
		// positions         // normals           // texture coords
	   -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
		0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  0.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
	   -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  1.0f,
	   -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,

	   -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
		0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
	   -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f,
	   -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,

	   -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
	   -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
	   -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
	   -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
	   -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
	   -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

		0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
		0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
		0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
		0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
		0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
		0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

	   -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,
		0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  1.0f,
		0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
		0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
	   -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,
	   -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,

	   -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  1.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
	   -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,
	   -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f

	};

	unsigned int VAOobject, VBO;
	glGenVertexArrays(1, &VAOobject);
	glBindVertexArray(VAOobject);
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	//VAO for LightObject
	unsigned int lightVAO;
	glGenVertexArrays(1, &lightVAO);
	glBindVertexArray(lightVAO);
	// we only need to bind to the VBO, the container’s VBO’s data
	// already contains the data.
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// set the vertex attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	//Generating texture
	unsigned int diffuse, specular;
	glGenTextures(1, &diffuse);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, diffuse);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//loading actual texture
	int widthW, heightW, nrChannelsW;
	unsigned char* container = stbi_load("C:/OpenGL/shaders/2textures/contain.jpg", &widthW, &heightW,
		&nrChannelsW, 0);
	if (container)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, widthW, heightW, 0, GL_RGB, GL_UNSIGNED_BYTE, container);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load container" << std::endl;
	}
	stbi_set_flip_vertically_on_load(true);

	// T E X T U R E GENERATION END FOR DIFFUSE. NEXT FOR SPEC
	glGenTextures(1, &specular);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, specular);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//loading actual texture
	int widthS, heightS, nrChannelsS;
	unsigned char* contSpec = stbi_load("C:/OpenGL/shaders/2textures/spec.jpg", &widthS, &heightS,
		&nrChannelsS, 0);
	if (contSpec)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, widthS, heightS, 0, GL_RGB, GL_UNSIGNED_BYTE, contSpec);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load container" << std::endl;
	}
	stbi_set_flip_vertically_on_load(true);

	///////////////////////////////E N D ////////////////////////

	glm::vec3 cubePositions[]
	{
		glm::vec3(0.0f, 0.0f, 0.0f),
glm::vec3(2.0f, 5.0f, -15.0f),
glm::vec3(-1.5f, -2.2f, -2.5f),
glm::vec3(-3.8f, -2.0f, -12.3f),
glm::vec3(2.4f, -0.4f, -3.5f),

	};


	Shader shaderObject("C:/OpenGL/shaders/basic lighting/verLmap.txt", "C:/OpenGL/shaders/basic lighting/fragAllTogether.txt");
	Shader shaderLight("C:/OpenGL/shaders/basic lighting/vertexLight.txt", "C:/OpenGL/shaders/basic lighting/fragLight.txt");
	//Creating complete transformation
	glm::mat4 modelObject = glm::mat4(1.0f);
	modelObject = glm::translate(modelObject, glm::vec3(0.0f, 0.0f, 1.5f));
	glm::mat4 view = glm::mat4(1.0f);
	view = glm::translate(view, glm::vec3(0.0f, 10.0f, 5.0f));
	glm::mat4 projection;
	projection = glm::perspective(glm::radians(45.0f), (800.0f / 600.0f), 0.1f, 100.0f);
	unsigned int modelLoc = glGetUniformLocation(shaderObject.ID, "model");
	unsigned int viewLoc = glGetUniformLocation(shaderObject.ID, "view");
	unsigned int projectionLoc = glGetUniformLocation(shaderObject.ID, "projection");


	unsigned int LightLoc = glGetUniformLocation(shaderLight.ID, "model");
	unsigned int viewLight = glGetUniformLocation(shaderLight.ID, "view");
	unsigned int projectLight = glGetUniformLocation(shaderLight.ID, "projection");

	glm::vec3 lightPos(0.5f, 0.5f, 1.5f);
	glm::mat4 modelLight = glm::mat4(1.0f);
	modelLight = glm::translate(modelLight, lightPos);
	modelLight = glm::scale(modelLight, glm::vec3(0.2f));


	glEnable(GL_DEPTH_TEST);

	shaderObject.use();
	shaderObject.setInt("material.diffuse", 0);
	shaderObject.setInt("material.specular", 1);




	while (!glfwWindowShouldClose(window))
	{

				

		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;



		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		glfwSetCursorPosCallback(window, mouse_callback);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


		processInput(window);
		glClearColor(0.3, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		shaderObject.use();
		int objectColorLocation = glGetUniformLocation(shaderObject.ID, "objectColor");
		int lightColorLocation = glGetUniformLocation(shaderObject.ID, "lightColor");
		int sourcePosition = glGetUniformLocation(shaderObject.ID, "sourcePosition");
		int viewPos = glGetUniformLocation(shaderObject.ID, "viewPos");

		glUniform3f(objectColorLocation, 1.0f, 0.5f, 0.31f);
		glUniform3f(lightColorLocation, 1.0f, 1.0f, 1.0f);
		glUniform3f(sourcePosition, lightPos.x, lightPos.y, lightPos.z);
		glUniform3f(viewPos, cameraPos.x, cameraPos.y, cameraPos.z);

		//Settings for material struct START

		
		int shininessMatLoc = glGetUniformLocation(shaderObject.ID, "material.shininess");
		glUniform1f(shininessMatLoc, 35.0f);
		

		//Settings for material struct END

		//settings for light struct
		/*
		int ambientLightLoc = glGetUniformLocation(shaderObject.ID, "light.ambient");
		int diffuseLightLoc = glGetUniformLocation(shaderObject.ID, "light.diffuse");
		int specularLightLoc = glGetUniformLocation(shaderObject.ID, "light.specular");
		int directionFlash = glGetUniformLocation(shaderObject.ID, "light.direction");
		int positionFlash = glGetUniformLocation(shaderObject.ID, "light.position");
		int innerAngle = glGetUniformLocation(shaderObject.ID, "light.innerAngle");
		int outerAngle = glGetUniformLocation(shaderObject.ID, "light.outerAngle");


		glUniform3f(ambientLightLoc, 0.35f, 0.35f, 0.35f);
		glUniform3f(diffuseLightLoc, 0.8f, 0.8f, 0.8f);
		glUniform3f(specularLightLoc, 1.0f, 1.0f, 1.0f);
		glUniform3f(directionFlash, cameraFront.x, cameraFront.y, cameraFront.z);
		glUniform3f(positionFlash, cameraPos.x, cameraPos.y, cameraPos.z);
		glUniform1f(innerAngle, glm::cos(glm::radians(12.5f)));
		glUniform1f(outerAngle, glm::cos(glm::radians(17.5f)));
		*/

		// S E T T I N G S FOR LIGHT TYPES STRUCTS
		// DIR LIGHT SETTINGS
		int DirLightDirection = glGetUniformLocation(shaderObject.ID, "lightD.direction");
		int DirLightAmbient = glGetUniformLocation(shaderObject.ID, "lightD.ambient");
		int DirLightDiffuse = glGetUniformLocation(shaderObject.ID, "lightD.diffuse");
		int DirLightSpecular = glGetUniformLocation(shaderObject.ID, "lightD.specular");

		glUniform3f(DirLightDirection, lightPos.x, lightPos.y, lightPos.z);
		glUniform3f(DirLightAmbient, 0.1, 0.1, 0.1);
		glUniform3f(DirLightDiffuse, 0.5, 0.5, 0.5);
		glUniform3f(DirLightSpecular, 1.0, 1.0, 1.0);


		// POINT LIGHT SETTINGS

		int PointLightDirection = glGetUniformLocation(shaderObject.ID, "lightP.position");
		int PointLightAmbient = glGetUniformLocation(shaderObject.ID, "lightP.ambient");
		int PointLightDiffuse = glGetUniformLocation(shaderObject.ID, "lightP.diffuse");
		int PointLightSpecular = glGetUniformLocation(shaderObject.ID, "lightP.specular");

		glUniform3f(PointLightDirection, lightPos.x, lightPos.y, lightPos.z);
		glUniform3f(PointLightAmbient, 0.1, 0.1, 0.1);
		glUniform3f(PointLightDiffuse, 0.5, 0.5, 0.5);
		glUniform3f(PointLightSpecular, 1.0, 1.0, 1.0);


		// SPOT LIGHT SETTINGS
		int SpotLightPosition = glGetUniformLocation(shaderObject.ID, "lightS.position");
		int SpotLightDirection = glGetUniformLocation(shaderObject.ID, "lightS.direction");
		int SpotLightInnerAngle = glGetUniformLocation(shaderObject.ID, "lightS.innerAngle");
		int SpotLightOuterAngle = glGetUniformLocation(shaderObject.ID, "lightS.outerAngle");

		int SpotLightAmbient = glGetUniformLocation(shaderObject.ID, "lightS.ambient");
		int SpotLightDiffuse = glGetUniformLocation(shaderObject.ID, "lightS.diffuse");
		int SpotLightSpecular = glGetUniformLocation(shaderObject.ID, "lightS.specular");

		glUniform3f(SpotLightPosition, cameraPos.x, cameraPos.y, cameraPos.z);
		glUniform3f(SpotLightDirection, cameraFront.x, cameraFront.y, cameraFront.z);
		glUniform1f(SpotLightInnerAngle, glm::cos(glm::radians(7.5f)));
		glUniform1f(SpotLightOuterAngle, glm::cos(glm::radians(9.5f)));
		glUniform3f(SpotLightAmbient, 0.1, 0.1, 0.1);
		glUniform3f(SpotLightDiffuse, 0.5, 0.5, 0.5);
		glUniform3f(SpotLightSpecular, 1.0, 1.0, 1.0);








		view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);


		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelObject));
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

		// bind diffuse map and spec map
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, diffuse);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, specular);


		glBindVertexArray(VAOobject);
		for (unsigned int i = 0; i < 10; i++)
		{
			shaderObject.use();
			float time = sin(glfwGetTime()) * 100.0f;
			float angle = 15.0f * i;
			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, cubePositions[i]);
			if (i % 3 == 0)
				model = glm::rotate(model, glm::radians(time), glm::vec3(1.0f, 0.3f, 0.5f));
			else
				model = glm::rotate(model, glm::radians(60.0f), glm::vec3(1.0f, 0.3f, 0.5f));



		


			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}


		// DRAW LIGHT

		shaderLight.use();
		glUniformMatrix4fv(LightLoc, 1, GL_FALSE, glm::value_ptr(modelLight));
		glUniformMatrix4fv(viewLight, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projectLight, 1, GL_FALSE, glm::value_ptr(projection));
		glBindVertexArray(lightVAO);

		glDrawArrays(GL_TRIANGLES, 0, 36);


		glfwSwapBuffers(window);
		glfwPollEvents();



	}

	glfwTerminate();

	return 0;



}


void processInput(GLFWwindow* window)
{
	float cameraSpeed = 0.85f * deltaTime;

	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		//if (cameraPos.y = 0.3f)
		cameraPos += cameraSpeed * cameraFront;
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		//if (cameraPos.y = 0.3f)
		cameraPos -= cameraSpeed * cameraFront;
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		cameraPos += glm::cross(cameraFront, cameraUp) * cameraSpeed;
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		cameraPos -= glm::cross(cameraFront, cameraUp) * cameraSpeed;


}

//Once the user resized window, viewPort should readjust itself---->
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);

}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{

	if (firstMove)
	{
		lastX = xpos;
		lastY = ypos;
		firstMove = false;
	}

	float xoffset = xpos - lastX;
	float yoffest = lastY - ypos;

	lastX = xpos;
	lastY = ypos;
	const float sensitivity = 0.05f;
	xoffset *= sensitivity;
	yoffest *= sensitivity;

	yaw += xoffset;
	pitch += yoffest;

	if (pitch > 89.0f)
		pitch = 89.0f;
	if (pitch < -89.0f)
		pitch = -89.0f;

	glm::vec3 direction;
	direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	direction.y = sin(glm::radians(pitch));
	direction.z = sin(glm::radians(yaw));
	cameraFront = glm::normalize(direction);


}
