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
	0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f,   // 右下  // 右上角
	0.5f, -0.5f, 0.5f, 0.0f, 1.0f, 0.0f,   // 左下 // 右下角
	-0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f,    // 顶部// 左下角
	-0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f,    // 顶部   // 左上角
	0.5f, 0.5f, -0.5f, 0.0f, 0.0f, 1.0f,    // 右下  // 右上角
	0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 0.0f,     // 左下 // 右下角
	-0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f,    // 顶部// 左下角
	-0.5f, 0.5f, -0.5f,  0.5f, 0.5f, 0.5f   // 顶部   // 左上角
};

unsigned int indices[] = { // 注意索引从0开始! 
	0, 1, 3, // 第一个三角形
	1, 2, 3,  // 第二个三角形
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
	// 初始化 设置当前context
	glfwMakeContextCurrent(window);

	//设置 改变大小时的回调方法
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
}

void draw() {
	// 设置重置的颜色
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	// 重置的方法
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

	// 2. 把我们的顶点数组复制到一个顶点缓冲中，供OpenGL使用
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
	第一个参数指定我们要配置的顶点属性。
		还记得我们在顶点着色器中使用layout(location = 0)定义了position顶点属性的位置值(Location)吗？
		它可以把顶点属性的位置值设置为0。因为我们希望把数据传递到这一个顶点属性中，所以这里我们传入0。
	第二个参数指定顶点属性的大小。顶点属性是一个vec3，它由3个值组成，所以大小是3。
	第三个参数指定数据的类型，这里是GL_FLOAT(GLSL中vec*都是由浮点数值组成的)。
	第四个参数定义我们是否希望数据被标准化(Normalize)。如果我们设置为GL_TRUE，所有数据都会被映射到0（对于有符号型signed数据是-1）到1之间。我们把它设置为GL_FALSE。
	第五个参数叫做步长(Stride)，它告诉我们在连续的顶点属性组之间的间隔。由于下个组位置数据在3个float之后，我们把步长设置为3 * sizeof(float)。要注意的是由于我们知道这个数组是紧密排列的（在两个顶点属性之间没有空隙）
		我们也可以设置为0来让OpenGL决定具体步长是多少（只有当数值是紧密排列时才可用）。
		一旦我们有更多的顶点属性，我们就必须更小心地定义每个顶点属性之间的间隔，我们在后面会看到更多的例子
		（译注: 这个参数的意思简单说就是从这个属性第二次出现的地方到整个数组0位置之间有多少字节）。
	最后一个参数的类型是void*，所以需要我们进行这个奇怪的强制类型转换。它表示位置数据在缓冲中起始位置的偏移量(Offset)。
		由于位置数据在数组的开头，所以这里是0。我们会在后面详细解释这个参数。
	*/
	// 位置属性
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// 颜色属性
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

	// 设置gl的视窗大小
	glViewport(0, 0, 800, 600);

	unsigned int VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	//bindTriangleWithColorVertices();
	bindRectVertices();

	// 编译shader并注入到显存中
	Shader shaderProgram("shader.vs", "shader.fs");

	// 设置顶点排列属性
	setAttribute();

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glEnable(GL_DEPTH_TEST);

	while (!glfwWindowShouldClose(window))
	{
		processInput(window);
		draw();

		glUseProgram(shaderProgram.ID);

		transToOrthoGraphic(shaderProgram.ID);

		// 使用VAO数据
		glBindVertexArray(VAO);
		//drawTriangle();

		drawRect();
		clear(window);
	}

	glfwTerminate();

	return 0;
}