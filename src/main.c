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

//@formatter:off
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

void installShaders() {

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

GLuint ID_TRIANGLE1;
void createTringle1() {
	GLuint VBO;
	glGenVertexArrays(1, &ID_TRIANGLE1);
	glBindVertexArray(ID_TRIANGLE1);
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(triangle1_vertices), triangle1_vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*) 0);
	glEnableVertexAttribArray(0);
}

GLuint ID_TRIANGLE2;
void createTringle2() {
	GLuint VBO;
	glGenVertexArrays(1, &ID_TRIANGLE2);
	glBindVertexArray(ID_TRIANGLE2);
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(triangle2_vertices), triangle2_vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*) 0);
	glEnableVertexAttribArray(0);
}

GLuint ID_TRIANGLES;
void createTringles() {
	GLuint VBO;
	glGenVertexArrays(1, &ID_TRIANGLES);
	glBindVertexArray(ID_TRIANGLES);
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(triangles), triangles, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*) 0);
	glEnableVertexAttribArray(0);
}

GLuint ID_TRIANGLES_EBO;
void createTrianglesEbo(){
	GLuint VBO;
	GLuint VEO;
	glGenVertexArrays(1, &ID_TRIANGLES_EBO);
	glBindVertexArray(ID_TRIANGLES_EBO);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glGenBuffers(1, &VEO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, VEO);

	glBufferData(GL_ARRAY_BUFFER, sizeof(triangles_ebo), triangles_ebo, GL_STATIC_DRAW);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices_triangles_ebo), indices_triangles_ebo, GL_STATIC_DRAW);


	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*) 0);
	glEnableVertexAttribArray(0);
}

void drawTriangles() {
	glBindVertexArray(ID_TRIANGLE1);
	glDrawArrays(GL_TRIANGLES, 0, 3);

	glBindVertexArray(ID_TRIANGLE2);
	glDrawArrays(GL_TRIANGLES, 0, 3);

	glBindVertexArray(ID_TRIANGLES);
	glDrawArrays(GL_TRIANGLES, 0, 6);

	glBindVertexArray(ID_TRIANGLES_EBO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

void drawPoints() {
	glBindVertexArray(ID_TRIANGLE1);
	glDrawArrays(GL_POINTS, 0, 3);

	glBindVertexArray(ID_TRIANGLE2);
	glDrawArrays(GL_POINTS, 0, 3);

	glBindVertexArray(ID_TRIANGLES);
	glDrawArrays(GL_POINTS, 0, 6);
}

void drawLineStrip() {
	//glBindVertexArray(ID_TRIANGLE1);
	//glDrawArrays(GL_LINE_STRIP, 0, 3);

	//glBindVertexArray(ID_TRIANGLE2);
	//glDrawArrays(GL_LINE_STRIP, 0, 3);

	glBindVertexArray(ID_TRIANGLES);
	glDrawArrays(GL_LINE_STRIP, 0, 6);
}

int main() {

	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow *window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
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

	installShaders();
	createTringle1();
	createTringle2();
	createTringles();
	createTrianglesEbo();


	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	while (!glfwWindowShouldClose(window)) {
		// input
		// -----
		processInput(window);

		// render
		// ------
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		drawTriangles();
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

void mouse_callback(GLFWwindow *window, int button, int action, int mods) {
	//printf("Mouse\n");
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
		//int w_width, w_height;
		int fb_width, fb_height;
		double x, y;
		glfwGetCursorPos(window, &x, &y);
		glfwGetFramebufferSize(window, &fb_width, &fb_height);
		//glfwGetWindowSize(window, &w_width, &w_height);

		printf("x:\t\t%f\ny:\t\t%f\n", x, y);
		//printf("w_width:\t%d\nw_height:\t%d\n", w_width, w_height);
		//printf("fb_width:\t%d\nfb_height:\t%d\n", fb_width, fb_height);
		printf("X ratio:\t%f\n", x / fb_width);
		printf("Y ratio:\t%f\n", y / fb_height);
		printf("X NDC:\t\t%f\n", (x / fb_width) * 2 - 1);
		printf("Y NDC:\t\t%f\n\n", -((y / fb_height) * 2 - 1));

	}
}

void keyboard_callback(GLFWwindow *window, int key, int scancode, int action, int mods) {
	char text[10];

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
			glBindVertexArray(ID_TRIANGLE1);
			return;
		}
		if (0 == strcmp(text, "tri2")) {
			printf("triangle 2:\n");
			glBindVertexArray(ID_TRIANGLE2);
			return;
		}
	}

}

void processInput(GLFWwindow *window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, 1);
}

