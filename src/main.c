/*
 * main.c
 *
 *  Created on: Nov 17, 2025
 *      Author: cacca
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define GLFW_INCLUDE_NONE
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "matrixcalc.h"

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void mouse_callback(GLFWwindow *window, int button, int action, int mods);
void keyboard_callback(GLFWwindow *window, int key, int scancode, int action, int mods);
void processInput(GLFWwindow *window);

void createTringle1();
void createTringle2();
void createTringles();
void createTrianglesEbo();

void drawTriangles();
void drawLineStrip();

GLuint shaderProgramID = 0;
GLuint shaderProgramsIDs[10];

//@formatter:off

	float triangle1_big[] = {
			50.0f, 50.0f, -20.0f,
			150.0f, -25.0f, -20.0f,
			30.0f, 70.0f, 70.0f
	};

	float triangle1_vertices[] = {
			-0.5f, -0.5f, 0.0f,
			0.5f, -0.6f, 0.0f,
			0.0f, 0.5f,0.0f
	};

	float triangle2_vertices[] = {
			0.5f, -0.5f, 0.0f,
			0.5f, -0.1f, 0.0f,
			0.3f, 0.5f,0.0f
	};

	float triangles[] = {
			0.7f, 0.9f, 0.0f,
			0.1f, 0.4f, 0.0f,
			0.0f, 0.6f,0.0f,
			-0.5f, -0.5f, 0.0f,
			-0.5f, -0.1f, 0.0f,
			-0.3f, -0.5f,0.0f
	};

	float vertices2[] = {
			-1.0f, -1.0f, 0.0f,
			1.0f, 0.0f, 0.0f,

			0.0f, 1.0f, 0.0f,
			0.0f, 1.0f, 0.0f,

			1.0f, -1.0f, 0.0f,
			0.0f, 0.0f, 1.0f,

			-1.0f, -1.0f, 0.0f,
			0.0f, 1.0f, 0.0f,

			-1.0f, 1.0f, 0.0f,
			1.0f, 0.0f, 0.0f,

			0.5f, 1.0f, 0.0f,
			1.0f, 1.0f, 1.0f
	};

	float triangle_colors[] = {
			1.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f,
			0.0f, 0.0f, 1.0f
	};

	float triangles_ebo[] = {
	     0.5f,  0.5f, 0.0f,  // top right
	     0.5f, -0.5f, 0.0f,  // bottom right
	    -0.5f, -0.5f, 0.0f,  // bottom left
	    -0.5f,  0.5f, 0.0f   // top left
	};

	unsigned int indices_triangles_ebo[] = {  // note that we start from 0!
	    0, 1, 3,   // first triangle
	    1, 2, 3    // second triangle
	};
//@formatter:on

typedef struct {
	GLuint VAO;
	float vertices[9];
} Triangle;

void insert_triangle_vertices(Triangle *t, float *data) {
	memcpy((*t).vertices, data, 9 * sizeof(float));
}

void load_triangle_in_buffer(Triangle *t) {
	GLuint VBO;
	glGenVertexArrays(1, &(*t).VAO);
	glBindVertexArray((*t).VAO);
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(float), (*t).vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*) 0);
	glEnableVertexAttribArray(0);
}

char* textFileToStringChar(char const *file_name) {
	char *shaderSourceCode;
	long numOfBytes = 0;

	FILE *fp = fopen(file_name, "r");
	if (fp == NULL) {
		printf("File error\n");
	}
	fseek(fp, 0, SEEK_END);
	numOfBytes = ftell(fp);
	fseek(fp, 0, SEEK_SET);
	printf("Number of bytes: %ld\n", numOfBytes);
	shaderSourceCode = (char*) malloc(numOfBytes);
	if (shaderSourceCode == NULL) {
		printf("malloc error\n");
	}
	fread(shaderSourceCode, sizeof(char), numOfBytes + 1, fp);
	fclose(fp);
	*(shaderSourceCode + numOfBytes) = '\0';
	printf("The file: %s\n\n", shaderSourceCode);
	return shaderSourceCode;
}

//input: i nomi dei file, output id del programma shader compilato
//
GLuint install_shaders(char const *vertexshader, char const *fragmentshader) {
	char *vertexShaderSource = textFileToStringChar(vertexshader);
	char *fragmentShaderSource = textFileToStringChar(fragmentshader);

	// build and compile our shader program
	// ------------------------------------
	// vertex shader
	GLuint vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShaderID, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShaderID);
	// check for shader compile errors
	int success;
	char infoLog[512];
	glGetShaderiv(vertexShaderID, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(vertexShaderID, 512, NULL, infoLog);
		printf("ERROR::SHADER::VERTEX::COMPILATION_FAILED\n");
	}

	// fragment shader
	GLuint fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShaderID, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShaderID);

	// check for shader compile errors
	glGetShaderiv(fragmentShaderID, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(fragmentShaderID, 512, NULL, infoLog);
		printf("ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n");
	}

	// link shaders
	GLuint programID = glCreateProgram();
	glAttachShader(programID, vertexShaderID);
	glAttachShader(programID, fragmentShaderID);
	glLinkProgram(programID);

	// check for linking errors
	glGetProgramiv(programID, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(programID, 512, NULL, infoLog);
		printf("ERROR::SHADER::PROGRAM::LINKING_FAILED\n");
	}
	glDeleteShader(vertexShaderID);
	glDeleteShader(fragmentShaderID);
	//glUseProgram(programID);
	free(vertexShaderSource);
	free(fragmentShaderSource);
	return programID;
}

//Old function
void install_shaders_old() {

	char const *fname_shader = "./shader.gls";
	char const *fname_fragment = "./fragment.gls";
	char *vertexShaderSource = textFileToStringChar(fname_shader);
	char *fragmentShaderSource = textFileToStringChar(fname_fragment);

	// build and compile our shader program
	// ------------------------------------
	// vertex shader
	GLuint vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShaderID, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShaderID);
	// check for shader compile errors
	int success;
	char infoLog[512];
	glGetShaderiv(vertexShaderID, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(vertexShaderID, 512, NULL, infoLog);
		printf("ERROR::SHADER::VERTEX::COMPILATION_FAILED\n");
	}
	// fragment shader
	GLuint fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShaderID, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShaderID);
	// check for shader compile errors
	glGetShaderiv(fragmentShaderID, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(fragmentShaderID, 512, NULL, infoLog);
		printf("ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n");
	}
	// link shaders
	shaderProgramID = glCreateProgram();
	glAttachShader(shaderProgramID, vertexShaderID);
	glAttachShader(shaderProgramID, fragmentShaderID);
	glLinkProgram(shaderProgramID);
	// check for linking errors
	glGetProgramiv(shaderProgramID, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgramID, 512, NULL, infoLog);
		printf("ERROR::SHADER::PROGRAM::LINKING_FAILED\n");
	}
	glDeleteShader(vertexShaderID);
	glDeleteShader(fragmentShaderID);
	glUseProgram(shaderProgramID);
	free(vertexShaderSource);
	free(fragmentShaderSource);

}

GLuint VAO_TRIANGLE1_BIG;
void createTringle1_big() {
	GLuint VBO;
	glGenVertexArrays(1, &VAO_TRIANGLE1_BIG);
	glBindVertexArray(VAO_TRIANGLE1_BIG);
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(triangle1_big), triangle1_big, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*) 0);
	glEnableVertexAttribArray(0);
}

GLuint VAO_TRIANGLE1;
void createTringle1() {
	GLuint VBO;
	glGenVertexArrays(1, &VAO_TRIANGLE1);
	glBindVertexArray(VAO_TRIANGLE1);
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(triangle1_vertices), triangle1_vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*) 0);
	glEnableVertexAttribArray(0);
}

GLuint VAO_TRIANGLE2;
void createTringle2() {
	GLuint VBO;
	glGenVertexArrays(1, &VAO_TRIANGLE2);
	glBindVertexArray(VAO_TRIANGLE2);
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(triangle2_vertices), triangle2_vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*) 0);
	glEnableVertexAttribArray(0);
}

GLuint VAO_TRIANGLES1;
void createTringles() {
	GLuint VBO;
	glGenVertexArrays(1, &VAO_TRIANGLES1);
	glBindVertexArray(VAO_TRIANGLES1);
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(triangles), triangles, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*) 0);
	glEnableVertexAttribArray(0);
}

GLuint VAO_TRIANGLES2;
void createTrianglesEbo() {
	GLuint VBO;
	GLuint VEO;
	glGenVertexArrays(1, &VAO_TRIANGLES2);
	glBindVertexArray(VAO_TRIANGLES2);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glGenBuffers(1, &VEO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, VEO);

	glBufferData(GL_ARRAY_BUFFER, sizeof(triangles_ebo), triangles_ebo, GL_STATIC_DRAW);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices_triangles_ebo), indices_triangles_ebo,
	GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*) 0);
	glEnableVertexAttribArray(0);
}

void drawTriangles() {
	glBindVertexArray(VAO_TRIANGLE1);
	glDrawArrays(GL_TRIANGLES, 0, 3);

	glBindVertexArray(VAO_TRIANGLE2);
	glDrawArrays(GL_TRIANGLES, 0, 3);

	glBindVertexArray(VAO_TRIANGLES1);
	glDrawArrays(GL_TRIANGLES, 0, 6);

	glBindVertexArray(VAO_TRIANGLES2);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

void drawPoints() {
	glBindVertexArray(VAO_TRIANGLE1);
	glDrawArrays(GL_POINTS, 0, 3);

	glBindVertexArray(VAO_TRIANGLE2);
	glDrawArrays(GL_POINTS, 0, 3);

	glBindVertexArray(VAO_TRIANGLES1);
	glDrawArrays(GL_POINTS, 0, 6);
}

void drawLineStrip() {
	//glBindVertexArray(ID_TRIANGLE1);
	//glDrawArrays(GL_LINE_STRIP, 0, 3);

	//glBindVertexArray(ID_TRIANGLE2);
	//glDrawArrays(GL_LINE_STRIP, 0, 3);

	glBindVertexArray(VAO_TRIANGLES1);
	glDrawArrays(GL_LINE_STRIP, 0, 6);
}

void drawWithUniform() {
	float time = glfwGetTime();
	float greenValue = (sin(time) / 2.0f) + 0.5f;
	int vertexColorLocation = glGetUniformLocation(shaderProgramsIDs[2], "unifFragColor");
	glUseProgram(shaderProgramsIDs[2]);
	glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f);
}

int main() {

	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow *window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "LearnOpenGL", NULL, NULL);
	if (window == NULL) {
		printf("Failed to create GLFW window\n");
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetMouseButtonCallback(window, mouse_callback);
	glfwSetKeyCallback(window, keyboard_callback);

	if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
		printf("Failed to initialize GLAD\n");
		return -1;
	}

	Triangle t1;
	insert_triangle_vertices(&t1, triangle1_vertices);
	load_triangle_in_buffer(&t1);
	printf("hola\n");

	//install_shaders_old();
	shaderProgramsIDs[0] = install_shaders("shader.gls", "fragment.gls");
	shaderProgramsIDs[1] = install_shaders("shader2.gls", "fragment2.gls");
	shaderProgramsIDs[2] = install_shaders("shader3.gls", "fragment3.gls");

	shaderProgramsIDs[3] = install_shaders("shader4.gls", "fragment4.gls");
	int projectionUnifLoc = glGetUniformLocation(shaderProgramsIDs[3], "projection");
	glUseProgram(shaderProgramsIDs[3]);
	//Mat4x4 m4 = mat4x4_rotate(mat4x4_identity(), (Vec4 ) { 0, 0, 1, 1 }, degToRad(70));
	//glUniformMatrix4fv(transformMatrixLocation, 1, GL_FALSE, &m4);

	float l = -400;
	float r = 400;
	float b = -300;
	float t = 300;
	float n = 10;
	float f = 100;
	Mat4x4 m5 = mat4x4_projection(l, r, b, t, n, f);
	mat4x4_print(&m5);
	//Dio porcaccio, controlla GL_TRUE, vorrei fosse FALSE, COLUM major del cazzo
	glUniformMatrix4fv(projectionUnifLoc, 1, GL_TRUE, &m5);

	createTringle1_big();

//	glUseProgram(shaderProgramsIDs[1]);
	createTringle1();
	//createTringle2();
	//createTringles();
	//createTrianglesEbo();

	glBindVertexArray(t1.VAO);

	printf("\nSMEGMA\n");
	//mat4x4_print(triangle1_big);

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glEnable(GL_DEPTH_TEST);
	while (!glfwWindowShouldClose(window)) {
		// input
		// -----
		processInput(window);

		// render
		// ------
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		//drawTriangles();
//		drawWithUniform();
		//drawPoints();
		//drawLineStrip();

		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		// -------------------------------------------------------------------------------
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	printf("\n hello\n");
	matrix_hello();
	glfwTerminate();
	return 0;
}

void test_print_projection(float l, float r, float b, float t, float n, float f) {
	Mat4x4 m5 = mat4x4_projection(l, r, b, t, n, f);
	mat4x4_print(&m5);
}

void mouse_callback(GLFWwindow *window, int button, int action, int mods) {
	//printf("Mouse\n");
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
		int w_width, w_height;
		int fb_width, fb_height;
		double x, y;
		glfwGetCursorPos(window, &x, &y);
		glfwGetFramebufferSize(window, &fb_width, &fb_height);
		//glfwGetWindowSize(window, &w_width, &w_height); ciao

		printf("x:\t\t%f\ny:\t\t%f\n", x, y);
		//printf("w_width:\t%d\nw_height:\t%d\n", w_width, w_height);
		printf("fb_width:\t%d\nfb_height:\t%d\n", fb_width, fb_height);
		printf("X ratio:\t%f\n", x / fb_width);
		printf("Y ratio:\t%f\n", y / fb_height);
		printf("X NDC:\t\t%f\n", (x / fb_width) * 2 - 1);
		printf("Y NDC:\t\t%f\n\n", -((y / fb_height) * 2 - 1));

	}
}

void keyboard_callback(GLFWwindow *window, int key, int scancode, int action, int mods) {
	char text[10];

	if (action == GLFW_PRESS) {
		if (key == GLFW_KEY_0) {
			printf("program 0:\n");
			glUseProgram(shaderProgramsIDs[0]);
		}
		if (key == GLFW_KEY_1) {
			printf("program 1:\n");
			glUseProgram(shaderProgramsIDs[1]);
		}
		if (key == GLFW_KEY_2) {
			printf("program 2:\n");
			glUseProgram(shaderProgramsIDs[2]);
		}
		if (key == GLFW_KEY_3) {
			printf("program 3:\n");
			glUseProgram(shaderProgramsIDs[3]);
		}
	}

	if (key == GLFW_KEY_E && action == GLFW_PRESS) {
		printf("Letter E pressed. WAITING FOR INPUT\n");
		scanf("%s", text);
		printf("Input: %s\n", text);
		if (0 == strcmp(text, "enable")) {
			printf("Enabled:\n");
			glEnableVertexAttribArray(0);
			return;
		}
		if (0 == strcmp(text, "disable")) {
			printf("Disabled:\n");
			glDisableVertexAttribArray(0);
			return;
		}
		if (0 == strcmp(text, "tri1")) {
			printf("triangle 1:\n");
			glBindVertexArray(VAO_TRIANGLE1);
			return;
		}
		if (0 == strcmp(text, "tri2")) {
			printf("triangle 2:\n");
			glBindVertexArray(VAO_TRIANGLE2);
			return;
		}
		if (0 == strcmp(text, "tri3")) {
			printf("triangle 3:\n");
			glBindVertexArray(VAO_TRIANGLE1_BIG);
			return;
		}
	}

}

void processInput(GLFWwindow *window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, 1);
}

