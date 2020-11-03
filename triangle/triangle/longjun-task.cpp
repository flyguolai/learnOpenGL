#include <iostream>

#include <string>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shader.h"

using namespace std;

float multi_vertices[] = {
	0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f,   // ����  // ���Ͻ�
	0.5f, -0.5f, 0.5f, 0.0f, 1.0f, 0.0f,   // ���� // ���½�
	-0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f,    // ����// ���½�
	-0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f,    // ����   // ���Ͻ�
	0.5f, 0.5f, -0.5f, 0.0f, 0.0f, 1.0f,    // ����  // ���Ͻ�
	0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 0.0f,     // ���� // ���½�
	-0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f,    // ����// ���½�
	-0.5f, 0.5f, -0.5f,  0.5f, 0.5f, 0.5f   // ����   // ���Ͻ�
};

unsigned int indices[] = { // ע��������0��ʼ! 
	0, 1, 3, // ��һ��������
	1, 2, 3,  // �ڶ���������
	0, 4, 3,
	3, 7, 4,
	4, 5, 7,
	5, 6, 7,
	1, 5, 6,
	1, 2, 6,
	0, 4, 5,
	0, 1, 5,
	3, 7, 6,
	3, 2, 6
};

const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	try
	{
		glViewport(0, 0, width, height);

	}
	catch (const std::exception&)
	{
		std::cout << "failed" << std::endl;
	}
}

void processInput(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}

	if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}

	if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
}

void init() {
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
}

void initWindow(GLFWwindow* window) {
	// ��ʼ�� ���õ�ǰcontext
	glfwMakeContextCurrent(window);

	//���� �ı��Сʱ�Ļص�����
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
}

void draw() {
	// �������õ���ɫ
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	// ���õķ���
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void clear(GLFWwindow* window) {
	glfwSwapBuffers(window);
	glfwPollEvents();
}


void bindRectVertices() {
	unsigned int VBO;
	glGenBuffers(1, &VBO);
	unsigned int EBO;
	glGenBuffers(1, &EBO);

	// 2. �����ǵĶ������鸴�Ƶ�һ�����㻺���У���OpenGLʹ��
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(multi_vertices), multi_vertices, GL_STATIC_DRAW);


	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
}

void transToOrthoGraphic(int shaderProgram) {
	glm::mat4 model = glm::mat4(1.0f);
	glm::mat4 view = glm::mat4(1.0f);
	glm::mat4 projection = glm::mat4(1.0f);
	model = glm::rotate(model, (float)glfwGetTime()*glm::radians(50.0f), glm::vec3(0.5f, 1.0f, 0.0f));
	view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
	projection = glm::perspective(glm::radians(45.0f), (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT, 0.1f, 100.0f);

	unsigned int modelLoc = glGetUniformLocation(shaderProgram, "model");
	unsigned int viewLoc = glGetUniformLocation(shaderProgram, "view");

	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &view[0][0]);

	string name = "projection";

	glUniformMatrix4fv(glGetUniformLocation(shaderProgram, name.c_str()), 1, GL_FALSE, &projection[0][0]);
}

void setAttribute() {
	/*
	��һ������ָ������Ҫ���õĶ������ԡ�
		���ǵ������ڶ�����ɫ����ʹ��layout(location = 0)������position�������Ե�λ��ֵ(Location)��
		�����԰Ѷ������Ե�λ��ֵ����Ϊ0����Ϊ����ϣ�������ݴ��ݵ���һ�����������У������������Ǵ���0��
	�ڶ�������ָ���������ԵĴ�С������������һ��vec3������3��ֵ��ɣ����Դ�С��3��
	����������ָ�����ݵ����ͣ�������GL_FLOAT(GLSL��vec*�����ɸ�����ֵ��ɵ�)��
	���ĸ��������������Ƿ�ϣ�����ݱ���׼��(Normalize)�������������ΪGL_TRUE���������ݶ��ᱻӳ�䵽0�������з�����signed������-1����1֮�䡣���ǰ�������ΪGL_FALSE��
	�����������������(Stride)�������������������Ķ���������֮��ļ���������¸���λ��������3��float֮�����ǰѲ�������Ϊ3 * sizeof(float)��Ҫע�������������֪����������ǽ������еģ���������������֮��û�п�϶��
		����Ҳ��������Ϊ0����OpenGL�������岽���Ƕ��٣�ֻ�е���ֵ�ǽ�������ʱ�ſ��ã���
		һ�������и���Ķ������ԣ����Ǿͱ����С�ĵض���ÿ����������֮��ļ���������ں���ῴ�����������
		����ע: �����������˼��˵���Ǵ�������Եڶ��γ��ֵĵط�����������0λ��֮���ж����ֽڣ���
	���һ��������������void*��������Ҫ���ǽ��������ֵ�ǿ������ת��������ʾλ�������ڻ�������ʼλ�õ�ƫ����(Offset)��
		����λ������������Ŀ�ͷ������������0�����ǻ��ں�����ϸ�������������
	*/
	// λ������
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// ��ɫ����
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
}

void paint() {

}

void drawTriangle() {
	glDrawArrays(GL_TRIANGLES, 0, 3);
	//glDrawArrays(GL_TRIANGLES, 3, 6);

}

void drawRect() {
	glDrawElements(GL_TRIANGLES, 72, GL_UNSIGNED_INT, 0);
}


int main() {
	init();

	GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "LearnOpenGL", NULL, NULL);

	if (window == NULL) {
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}

	initWindow(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "failed to initialize GLAD" << std::endl;
		return -1;
	}

	// ����gl���Ӵ���С
	glViewport(0, 0, 800, 600);

	unsigned int VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	//bindTriangleWithColorVertices();
	bindRectVertices();

	// ����shader��ע�뵽�Դ���
	Shader shaderProgram("shader.vs", "shader.fs");

	// ���ö�����������
	setAttribute();

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glEnable(GL_DEPTH_TEST);

	while (!glfwWindowShouldClose(window))
	{
		processInput(window);
		draw();

		glUseProgram(shaderProgram.ID);

		transToOrthoGraphic(shaderProgram.ID);

		// ʹ��VAO����
		glBindVertexArray(VAO);
		//drawTriangle();

		drawRect();
		clear(window);
	}

	glfwTerminate();

	return 0;
}