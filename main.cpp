#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "my_shader.h" 
#include <iostream>
#include <vector>
#include <cmath>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

// ���������
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;
//const float radius = 1.0f;

// ������
glm::vec3 cameraPos(0.0f, 0.0f, 3.0f);
glm::vec3 cameraFront(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp(0.0f, 1.0f, 0.0f);
//����
glm::vec3 lightPos(1.0f, 0.8f, 0.8f);

float RotateX = 0.0f;
float RotateY = 0.0f;
float RotateZ = 0.0f;

float figurePrecision = 4.0f;
bool recalculateFigure = false;
std::pair<std::vector<float>, std::vector<unsigned>> customFigure(size_t precision);
std::pair<std::vector<float>, std::vector<unsigned>> cubeFigure();


int main()
{
	// glfw: ������������� � ����������������
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	

	// glfw: �������� ����
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "test", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	

	// glad: �������� ���� ���������� �� OpenGL-�������
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}
	
	

	Shader fig_shader("path to shader.ves", "path to shader.frs");
	Shader light_shader("path to light_shader.ves", "path to light_shader.frs");

	// ���������� ������ (� �������) � ��������� ��������� ���������
	/*float vertices[] = {
	-0.5f, -0.5f, -0.5f,  
	 0.5f, -0.5f, -0.5f,  
	 0.5f,  0.5f, -0.5f,  
	 0.5f,  0.5f, -0.5f,  
	-0.5f,  0.5f, -0.5f,  
	-0.5f, -0.5f, -0.5f,  

	-0.5f, -0.5f,  0.5f,  
	 0.5f, -0.5f,  0.5f,  
	 0.5f,  0.5f,  0.5f,  
	 0.5f,  0.5f,  0.5f,  
	-0.5f,  0.5f,  0.5f,  
	-0.5f, -0.5f,  0.5f,  

	-0.5f,  0.5f,  0.5f,  
	-0.5f,  0.5f, -0.5f,  
	-0.5f, -0.5f, -0.5f,  
	-0.5f, -0.5f, -0.5f,  
	-0.5f, -0.5f,  0.5f,  
	-0.5f,  0.5f,  0.5f,  

	 0.5f,  0.5f,  0.5f,  
	 0.5f,  0.5f, -0.5f,  
	 0.5f, -0.5f, -0.5f,  
	 0.5f, -0.5f, -0.5f,  
	 0.5f, -0.5f,  0.5f,  
	 0.5f,  0.5f,  0.5f,  

	-0.5f, -0.5f, -0.5f,  
	 0.5f, -0.5f, -0.5f,  
	 0.5f, -0.5f,  0.5f,  
	 0.5f, -0.5f,  0.5f,  
	-0.5f, -0.5f,  0.5f,  
	-0.5f, -0.5f, -0.5f,  

	-0.5f,  0.5f, -0.5f,  
	 0.5f,  0.5f, -0.5f,  
	 0.5f,  0.5f,  0.5f,  
	 0.5f,  0.5f,  0.5f,  
	-0.5f,  0.5f,  0.5f,  
	-0.5f,  0.5f, -0.5f,
	};*/

	auto tempTriangles = customFigure(figurePrecision);
	std::vector<float> vertices = tempTriangles.first;
	std::vector<unsigned> indices = tempTriangles.second;

	auto tempCubeTriangles = cubeFigure();
	std::vector<float> cubeVertices = tempCubeTriangles.first;
	std::vector<unsigned> cubeIndices = tempCubeTriangles.second;

	unsigned int VBO1, VAO1, EBO1;
	glGenVertexArrays(1, &VAO1);
	glGenBuffers(1, &VBO1);
	glGenBuffers(1, &EBO1);

	// ������� ��������� ������ ���������� �������, ����� ��������� � ������������� ��������� �����(�), � ����� ������������� ��������� �������(�)
	glBindVertexArray(VAO1);

	glBindBuffer(GL_ARRAY_BUFFER, VBO1);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_DYNAMIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO1);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned), indices.data(), GL_DYNAMIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	unsigned int lightVAO, lightVBO, lightEBO;
	glGenVertexArrays(1, &lightVAO);
	glBindVertexArray(lightVAO);
	glGenBuffers(1, &lightVBO);
	glGenBuffers(1, &lightEBO);
	
	glBindBuffer(GL_ARRAY_BUFFER, lightVBO);
	glBufferData(GL_ARRAY_BUFFER, cubeVertices.size() * sizeof(float), cubeVertices.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, lightEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, cubeIndices.size() * sizeof(unsigned), cubeIndices.data(), GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
	glEnableVertexAttribArray(0);


	// �� ������ �������� �������� V�� ����� �����, ����� ������ ������ V�� �������� �� �������� ���� VAO (�� �������� �������� ����� ���������).
	// ����������� ������ VAO ������� ����� glBindVertexArray(), ������� �� ������ �� ������� �������� VAO (��� VBO), ����� ��� �� ��������� ��������
	//glBindVertexArray(0);


	// �������� ��������, ��� ������ �������� ���������, ����� glVertexAttribPointer() ��������������� VBO ��� ����������� ��������� �������� ������ ��� ���������� ��������, ��� ��� ����� ����� �� ����� �������� ��������� �������
	glBindBuffer(GL_ARRAY_BUFFER, 0);


	// ������� GL_FILL/������ GL_LINE
	 glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);


	// ���� ����������
	while (!glfwWindowShouldClose(window))
	{
	
		
		// ��������� �����
		processInput(window);

		// ���������
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glEnable(GL_DEPTH_TEST);

		glBindVertexArray(VAO1);

		fig_shader.use();
		fig_shader.setVec4("objColor", 0.0f, 0.0f, 1.0f, 1.0f);
		fig_shader.setVec4("lightColor", 1.0f, 1.0f, 1.0f, 1.0f);
		
		if (recalculateFigure && (int)figurePrecision % 1 == 0) {
			tempTriangles = customFigure(figurePrecision);
			vertices = tempTriangles.first;
			indices = tempTriangles.second;
			
			glBindBuffer(GL_ARRAY_BUFFER, VBO1);
			glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_DYNAMIC_DRAW);

			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO1);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned), indices.data(), GL_DYNAMIC_DRAW);

			recalculateFigure = false;
		}
		
		glm::mat4 model = glm::mat4(1.0f); // ������� �������������� ��������� �������
		//model = glm::rotate(model, glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		
		// ������� �������������� ������/����
		glm::mat4 view = glm::mat4(1.0f);
		view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp); //������� ������, ������� �������, ������-�����
		view = glm::rotate(view, glm::radians(RotateX), glm::vec3(1.0, 0.0, 0.0));
		view = glm::rotate(view, glm::radians(RotateY), glm::vec3(0.0, 1.0, 0.0));
		view = glm::rotate(view, glm::radians(RotateZ), glm::vec3(0.0, 0.0, 1.0)); 

		// // �������� ������� ������� ��������(��������� ������������ ������� ����� ��������, ��� ������������� ������ ��� ��� ������� �����)
		glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
		fig_shader.setMat4("projection", projection);
		
		// �������� �������������� uniform-������...
		unsigned int modelLoc = glGetUniformLocation(fig_shader.ID, "model");
		unsigned int viewLoc = glGetUniformLocation(fig_shader.ID, "view");
		// ...������� �� � �������
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		
		

		glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);

		// glBindVertexArray(0); // �� ����� ������ ��� ��� ����������
		glBindVertexArray(lightVAO);

		light_shader.use();
		//view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
		light_shader.setMat4("projection", projection);
		light_shader.setMat4("view", view);
		model = glm::mat4(1.0f);
		model = glm::translate(model, lightPos);
		model = glm::scale(model, glm::vec3(0.2f)); // ���, �������� �������
		light_shader.setMat4("model", model);
		
		glDrawElements(GL_TRIANGLES, cubeIndices.size(), GL_UNSIGNED_INT, 0);

		// glfw: ����� ���������� front- � back- �������. ������������ ������� �����\������ (���� �� ������/�������� ������, ��������� ������ ���� � �.�.)
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// �����������: ����������� ��� �������, ��� ������ ��� ��������� ���� ��������������
	glDeleteVertexArrays(1, &VAO1);
	glDeleteBuffers(1, &VBO1);
	glDeleteBuffers(1, &EBO1);
	

	glDeleteVertexArrays(1, &lightVAO);
	glDeleteBuffers(1, &lightVBO);
	glDeleteBuffers(1, &lightEBO);
	

	// glfw: ����������, ������������ ���� ����� ��������������� GLFW-��������
	glfwTerminate();
	return 0;
}

// ��������� ���� ������� �����: ������ GLFW � �������/���������� ������ ���� � ������ ����� � ��������������� ��������� ������ �������
void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	
	float rotateSpeed = 3.0f;
	if (glfwGetKey(window, GLFW_KEY_EQUAL) == GLFW_PRESS) {
		cameraPos += rotateSpeed/300 * cameraFront;
	}
	if (glfwGetKey(window, GLFW_KEY_MINUS) == GLFW_PRESS) {
		cameraPos -= rotateSpeed/300 * cameraFront;
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		RotateX -= rotateSpeed/10;
	}
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		RotateX += rotateSpeed/10;
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		RotateY -= rotateSpeed/10;
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		RotateY += rotateSpeed/10;
	}
	if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS) {
		figurePrecision -= 0.05;
		recalculateFigure = true;
	}
	if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS) {
		figurePrecision += 0.05;
		recalculateFigure = true;
	}
	if (figurePrecision < 3) {
		figurePrecision = 3;
	}
}

// glfw: ������ ���, ����� ���������� ������� ���� (������������� ��� ������������ ��������), ���������� ������ callback-�������
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// ����������, ��� ���� ��������� ������������� ����� �������� ����.
	// �������� ��������, ������ ����� ����������� ������, ��� �������, �� Retina-��������
	glViewport(0, 0, width, height);
}

std::pair<std::vector<float>, std::vector<unsigned>> customFigure(size_t precision) {
	
	float co1, si1;
	float co2, si2;
	co1 = si1 = 0;
	co2 = si2 = 0;
	std::vector<float> triangles;
	for (int i = 0; i < precision; i++) //������� �����
	{
		co1 = cos(2 * 3.14 * i / precision);
		co2 = cos(2 * 3.14 * (i + 1) / precision);
		si1 = sin(2 * 3.14 * i / precision);
		si2 = sin(2 * 3.14 * (i+1) / precision);
		triangles.insert(triangles.end(), {co1/1.5f, 0.5f, si1/1.5f});
		triangles.insert(triangles.end(), {co2 / 1.5f, 0.5f, si2 / 1.5f });
		triangles.insert(triangles.end(), {0, 0.5f, 0});
		triangles.insert(triangles.end(), { co2, -0.5f, si2 });
		triangles.insert(triangles.end(), { co1, -0.5f, si1});
		triangles.insert(triangles.end(), { 0, -0.5f, 0 });
		triangles.insert(triangles.end(), { co2 / 1.5f, 0.5f, si2 / 1.5f });
		triangles.insert(triangles.end(), { co1 / 1.5f, 0.5f, si1 / 1.5f });
		triangles.insert(triangles.end(), { co2, -0.5f, si2 });
		triangles.insert(triangles.end(), { co1, -0.5f, si1 });
		triangles.insert(triangles.end(), { co2, -0.5f, si2 });
		triangles.insert(triangles.end(), { co1 / 1.5f, 0.5f, si1 / 1.5f });
	}

	std::vector<unsigned> indices(triangles.size() / 3);
	for (size_t i = 0; i < indices.size(); i++) {
		indices[i] = i;
	}
	return { triangles, indices };
}

std::pair<std::vector<float>, std::vector<unsigned>> cubeFigure() {

	std::vector<float> vertices = {
			1.0, 1.0, -1.0, //0
			-1.0, 1.0, -1.0,
			-1.0, -1.0, -1.0,
			1.0, -1.0, -1.0,

			1.0, 1.0, 1.0, //4
			-1.0, 1.0, 1.0,
			-1.0, -1.0, 1.0,
			1.0, -1.0, 1.0,
	};
	std::vector<unsigned> indices = {
			0, 1, 4,
			1, 5, 4,
			1, 2, 5,
			2, 6, 5,
			2, 3, 6,
			3, 7, 6,
			0, 7, 3,
			0, 4, 7,

			4, 5, 6,
			6, 7, 4,

			0, 2, 1,
			0, 3, 2,
	};


	return { vertices, indices };
}
