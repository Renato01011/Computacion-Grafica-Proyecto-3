#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shader.h"
#include "Camera.h"
#include "Mesh.h"

#include <iostream>

// Settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// Function To Load Points
std::pair<std::vector<Vertex>, std::vector<unsigned int>> readFilePoints(std::string filename) {
    std::ifstream pointFile;
    pointFile.open(filename);
    if (!pointFile.is_open())
        std::cout << "Could not Open File" << std::endl;

    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;

    std::string temp;
    int numPoints;
	int texCoordInd = 0;
    pointFile >> temp >> numPoints;

	for (int i = 0; i < numPoints; i++) {
        Vertex vtemp;
        GLfloat x, y, z, nx, ny, nz;
        pointFile >> x >> y >> z >> nx >> ny >> nz;
    
        vtemp.Position = glm::vec3(x, y, z);
        vtemp.Normal = glm::vec3(nx, ny, nz);
        vtemp.Color = glm::vec3(1.0f, 1.0f, 1.0f);
		//vtemp.TexCoords = glm::vec2(0.0f, 0.0f);
		if (texCoordInd == 0) {
			vtemp.TexCoords = glm::vec2(0.0f, 0.0f);
			texCoordInd++;
		}
		else if (texCoordInd == 1) {
			vtemp.TexCoords = glm::vec2(0.5f, 1.0f);
			texCoordInd++;
		}
		else if (texCoordInd == 2) {
			vtemp.TexCoords = glm::vec2(1.0f, 0.0f);
			texCoordInd = 0;
		}
        
		vertices.push_back(vtemp);
    }

    int numIndices;
    pointFile >> temp >> numIndices;
    numIndices *= 3;

	for (int i = 0; i < numIndices; i++) {
        unsigned int ind;
        pointFile >> ind;
		indices.push_back(ind);
    }
    
    pointFile.close();

    return std::make_pair(vertices, indices);
}

int main()
{
	// Init Window
	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Proyecto 3", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	gladLoadGL();
	glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);

    // Get Points
    std::pair<std::vector<Vertex>, std::vector<unsigned int>> readPoints = readFilePoints("Points.dat");

	// Generate Shaders
	Shader shaderProgram("VertexShader.vs", "FragmentShader.fs");
	VAO VAO1;
	VAO1.Bind();
	VBO VBO1(readPoints.first);
	EBO EBO1(readPoints.second);
	VAO1.LinkAttrib(VBO1, 0, 3, GL_FLOAT, sizeof(Vertex), (void*)0);
	VAO1.LinkAttrib(VBO1, 1, 3, GL_FLOAT, sizeof(Vertex), (void*)offsetof(Vertex, Color));
	VAO1.LinkAttrib(VBO1, 2, 2, GL_FLOAT, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));
	VAO1.LinkAttrib(VBO1, 3, 3, GL_FLOAT, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
	VAO1.UnBind();
	VBO1.UnBind();
	EBO1.UnBind();

	// Load and Create Texture
	Texture loadedTex("./Resources/Stone.jpg", GL_TEXTURE_2D, GL_RGB, GL_UNSIGNED_BYTE);

	// Light Pos And Color
	glm::vec4 lightColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	glm::vec3 lightPos = glm::vec3(0.0f, 0.0f, 0.0f);

	// Tunnel Model Matrix
	glm::vec3 pos = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, pos);

	// Set Starting Uniforms
	shaderProgram.Activate();
	glUniformMatrix4fv(glGetUniformLocation(shaderProgram.ID, "model"), 1, GL_FALSE, glm::value_ptr(model));
	glUniform4f(glGetUniformLocation(shaderProgram.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
	glUniform3f(glGetUniformLocation(shaderProgram.ID, "lightPos"), lightPos.x, lightPos.y, lightPos.z);

	glEnable(GL_DEPTH_TEST);

	Camera camera(SCR_WIDTH, SCR_HEIGHT, glm::vec3(0.0f, 0.0f, 20.0f));

	// Main Loop
	while (!glfwWindowShouldClose(window))
	{
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		camera.Inputs(window);
		camera.updateMatrix(45.0f, 0.1f, 100.0f);
		
		shaderProgram.Activate();
		glUniform3f(glGetUniformLocation(shaderProgram.ID, "camPos"), camera.Position.x, camera.Position.y, camera.Position.z);
		camera.Matrix(shaderProgram, "camMatrix");
		
		VAO1.Bind();
		loadedTex.Bind();
		glDrawElements(GL_TRIANGLES, readPoints.second.size(), GL_UNSIGNED_INT, 0);
		loadedTex.UnBind();

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// Delete
	VAO1.Delete();
	VBO1.Delete();
	EBO1.Delete();
	loadedTex.Delete();
	glDeleteProgram(shaderProgram.ID);
	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}