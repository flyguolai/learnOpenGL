#include <iostream>

#include <string>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

using namespace std;

// triangle
float vertices[] = {
	-0.5f, -0.5f, 0.0f,
	0.5f, -0.5f, 0.0f,
	0.0f, 0.5f, 0.0f
};

float vertices_with_color[] = {
	// λ��              // ��ɫ
	 0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,   // ����
	-0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,   // ����
	 0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f    // ����
};

float two_single_vertices[] = {
	// ��һ��������
	0.5f, 0.5f, 0.0f,   // ���Ͻ�
	0.5f, -0.5f, 0.0f,  // ���½�
	-0.5f, 0.5f, 0.0f,  // ���Ͻ�
	// �ڶ���������
	0.5f, -0.5f, 0.0f,  // ���½�
	-0.5f, -0.5f, 0.0f, // ���½�
	-0.5f, 0.5f, 0.0f   // ���Ͻ�
};

float multi_vertices[] = {
	0.5f, 0.5f, 0.0f,   // ���Ͻ�
	0.5f, -0.5f, 0.0f,  // ���½�
	-0.5f, -0.5f, 0.0f, // ���½�
	-0.5f, 0.5f, 0.0f   // ���Ͻ�
};

unsigned int indices[] = { // ע��������0��ʼ! 
	0, 1, 3, // ��һ��������
	1, 2, 3  // �ڶ���������
};

const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;

// shader
const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"layout (location = 1) in vec3 aColor;\n"
"out vec3 outColor;"
"void main(){ \n"
	"gl_Position = vec4(aPos, 1.0);\n"
	"outColor = aColor;\n"
"}\n";
 
// fragment shader
const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"in vec3 outColor;\n"
"void main(){ \n"
	"FragColor = vec4(outColor, 1.0);\n"
"}\n";

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
	glClear(GL_COLOR_BUFFER_BIT);
}

void clear(GLFWwindow* window) {
	glfwSwapBuffers(window);
	glfwPollEvents();
}

/*
�ж���ɫ���Ƿ����ɹ�
*/
void checkShaderCompileIsSuccess(int vertexShader) {
	int  success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
}

/*
�ж���ɫ�������Ƿ�ɹ�
*/
void checkProgramCompileIsSuccess(int shaderProgram) {
	int  success;
	char infoLog[512];
	glGetProgramiv(shaderProgram, GL_COMPILE_STATUS, &success);

	if (!success)
	{
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
}

void bindTriangleVertices() {
	unsigned int VBO;

	// ��ʼ�������ݵ��Դ���
	// �����Դ��ַ
	glGenBuffers(1, &VBO);

	// ���øո�������Դ������
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	// �����ݴ��䵽�Դ���
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
}

void bindTriangleWithColorVertices() {
	unsigned int VBO;

	// ��ʼ�������ݵ��Դ���
	// �����Դ��ַ
	glGenBuffers(1, &VBO);

	// ���øո�������Դ������
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	// �����ݴ��䵽�Դ���
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_with_color), vertices_with_color, GL_STATIC_DRAW);
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

void bindTwoTriangleVertices() {
	unsigned int VBO;
	// ��ʼ�������ݵ��Դ���
	// �����Դ��ַ
	glGenBuffers(1, &VBO);
	// ���øո�������Դ������
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// �����ݴ��䵽�Դ���
	glBufferData(GL_ARRAY_BUFFER, sizeof(two_single_vertices), two_single_vertices, GL_STATIC_DRAW);
}

int compileShader() {
	// �����ڴ�ָ��
	unsigned int vertexShader;
	// ����shader����
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	// ��shader�Ĵ��붪��shader��ȴ�����
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	// ���붥����ɫ��
	glCompileShader(vertexShader);
	// �ж���ɫ���Ƿ�ɹ���ʧ�ܵĻ�����console�����log
	checkShaderCompileIsSuccess(vertexShader);

	// ����ͬ�ϣ��ⲿ����ƬԪ��ɫ��
	unsigned int fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);
	checkShaderCompileIsSuccess(fragmentShader);
	// ƬԪ��ɫ�� end

	// ������ɫ������
	unsigned int shaderProgram;
	shaderProgram = glCreateProgram();
	// ����������õ�shader����shader�ĳ�����
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	// opengl���� shader����
	glLinkProgram(shaderProgram);

	checkProgramCompileIsSuccess(shaderProgram);

	// ע�뵽gpu�Ժ��ͷŵ��ڴ�
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	return shaderProgram;
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

void drawTriangle() {
	//glDrawArrays(GL_TRIANGLES, 0, 3);
	glDrawArrays(GL_TRIANGLES, 3, 6);

}

void drawRect() {
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
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

	//bindTriangleVertices();
	bindTriangleWithColorVertices();
	//bindRectVertices();
	//bindTwoTriangleVertices();
	// ����shader��ע�뵽�Դ���
	int shaderProgram = compileShader();

	// ���ö�����������
	setAttribute();

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	while (!glfwWindowShouldClose(window))
	{
		processInput(window);
		draw();

		glUseProgram(shaderProgram);

		// ʹ��VAO����
		glBindVertexArray(VAO);
		drawTriangle();
		
		//drawRect();
		clear(window);
	}

	glfwTerminate();

	return 0;
}