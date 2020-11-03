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
	// 位置              // 颜色
	 0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,   // 右下
	-0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,   // 左下
	 0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f    // 顶部
};

float two_single_vertices[] = {
	// 第一个三角形
	0.5f, 0.5f, 0.0f,   // 右上角
	0.5f, -0.5f, 0.0f,  // 右下角
	-0.5f, 0.5f, 0.0f,  // 左上角
	// 第二个三角形
	0.5f, -0.5f, 0.0f,  // 右下角
	-0.5f, -0.5f, 0.0f, // 左下角
	-0.5f, 0.5f, 0.0f   // 左上角
};

float multi_vertices[] = {
	0.5f, 0.5f, 0.0f,   // 右上角
	0.5f, -0.5f, 0.0f,  // 右下角
	-0.5f, -0.5f, 0.0f, // 左下角
	-0.5f, 0.5f, 0.0f   // 左上角
};

unsigned int indices[] = { // 注意索引从0开始! 
	0, 1, 3, // 第一个三角形
	1, 2, 3  // 第二个三角形
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
	// 初始化 设置当前context
	glfwMakeContextCurrent(window);

	//设置 改变大小时的回调方法
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
}

void draw() {
	// 设置重置的颜色
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	// 重置的方法
	glClear(GL_COLOR_BUFFER_BIT);
}

void clear(GLFWwindow* window) {
	glfwSwapBuffers(window);
	glfwPollEvents();
}

/*
判断着色器是否编译成功
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
判断着色器程序是否成功
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

	// 开始传输数据到显存中
	// 申请显存地址
	glGenBuffers(1, &VBO);

	// 设置刚刚申请的显存的类型
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	// 将数据传输到显存中
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
}

void bindTriangleWithColorVertices() {
	unsigned int VBO;

	// 开始传输数据到显存中
	// 申请显存地址
	glGenBuffers(1, &VBO);

	// 设置刚刚申请的显存的类型
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	// 将数据传输到显存中
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_with_color), vertices_with_color, GL_STATIC_DRAW);
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

void bindTwoTriangleVertices() {
	unsigned int VBO;
	// 开始传输数据到显存中
	// 申请显存地址
	glGenBuffers(1, &VBO);
	// 设置刚刚申请的显存的类型
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// 将数据传输到显存中
	glBufferData(GL_ARRAY_BUFFER, sizeof(two_single_vertices), two_single_vertices, GL_STATIC_DRAW);
}

int compileShader() {
	// 申请内存指针
	unsigned int vertexShader;
	// 创建shader对象
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	// 将shader的代码丢到shader里等待编译
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	// 编译顶点着色器
	glCompileShader(vertexShader);
	// 判断着色器是否成功，失败的话会在console里进行log
	checkShaderCompileIsSuccess(vertexShader);

	// 流程同上，这部分是片元着色器
	unsigned int fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);
	checkShaderCompileIsSuccess(fragmentShader);
	// 片元着色器 end

	// 生成着色器程序
	unsigned int shaderProgram;
	shaderProgram = glCreateProgram();
	// 将两个编译好的shader丢到shader的程序中
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	// opengl链接 shader程序
	glLinkProgram(shaderProgram);

	checkProgramCompileIsSuccess(shaderProgram);

	// 注入到gpu以后释放掉内存
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	return shaderProgram;
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

	// 设置gl的视窗大小
	glViewport(0, 0, 800, 600);

	unsigned int VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	//bindTriangleVertices();
	bindTriangleWithColorVertices();
	//bindRectVertices();
	//bindTwoTriangleVertices();
	// 编译shader并注入到显存中
	int shaderProgram = compileShader();

	// 设置顶点排列属性
	setAttribute();

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	while (!glfwWindowShouldClose(window))
	{
		processInput(window);
		draw();

		glUseProgram(shaderProgram);

		// 使用VAO数据
		glBindVertexArray(VAO);
		drawTriangle();
		
		//drawRect();
		clear(window);
	}

	glfwTerminate();

	return 0;
}