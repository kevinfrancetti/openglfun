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
#include "shapes.h"
#include "matrixcalc.h"
#include "shader_manager.h"

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void mouse_callback(GLFWwindow *window, int button, int action, int mods);
void keyboard_callback(GLFWwindow *window, int key, int scancode, int action, int mods);
void processInput(GLFWwindow *window);

GLuint shaderProgramsIDs[10];

typedef struct {
	GLuint VAO;
	float vertices[3 * 3];
} Triangle;

typedef struct {
	GLuint VAO;
	float vertices[3 * 8]; //8 vertices and 3 float for each vertex
} Cube;

typedef struct {
	Triangle *t1;
	Triangle *t2;
	Triangle *t3;
	Triangle *t4;
} GameState;

void triangle_set_vertices(Triangle *t, float *data) {
	memcpy((*t).vertices, data, 9 * sizeof(float));
}

void cube_set_vertices(Cube *cube, float *data) {
	memcpy((*cube).vertices, data, 3 * 8 * sizeof(float));
}

void triangle_load_in_buffer(Triangle *t) {
	GLuint VBO;
	glGenVertexArrays(1, &(*t).VAO);
	glBindVertexArray((*t).VAO);
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(float), (*t).vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*) 0);
	glEnableVertexAttribArray(0);
	glBindVertexArray(0); //unbind
}

void triangle_use_vao(Triangle *t) {
	glBindVertexArray((*t).VAO);
}

void cube_load_in_buffer(Cube *cube) {
	GLuint VBO;
	GLuint VEO;
	glGenVertexArrays(1, &(*cube).VAO);
	glBindVertexArray((*cube).VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &VEO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, VEO);

	glBufferData(GL_ARRAY_BUFFER, 3 * 8 * sizeof(float), (*cube).vertices, GL_STATIC_DRAW);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cube_indices_ebo), cube_indices_ebo,
	GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*) 0); //HERE
	glEnableVertexAttribArray(0);
	glBindVertexArray(0); //unbind
}

void cube_use_vao(Cube *cube) {
	glBindVertexArray((*cube).VAO);
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

float g_l = -4;
float g_r = 4;
float g_b = -3;
float g_t = 3;
float g_n = 1;
float g_f = 100;
float g_rotation = 0;
void update_uniform_projection_matrix() {
	//float time = glfwGetTime();
	//float value = (sin(time) / 2.0f) + 0.5f;

	printf("l= %f \n", g_l);
	printf("r= %f \n", g_r);
	printf("b= %f \n", g_b);
	printf("t= %f \n", g_t);
	printf("n= %f \n", g_n);
	printf("f= %f \n", g_f);
	printf("angle rotation= %f \n", g_rotation);

	Vec4 rotation_axe;
	rotation_axe.x = 1;
	rotation_axe.y = 1;
	rotation_axe.z = 0;
	rotation_axe.w = 0;
	//Mat4x4 mat_ortho = mat4x4_orthographic_projection(l, r, b, t, n, f);
	Mat4x4 mat_persp = mat4x4_perspective_projection(g_l, g_r, g_b, g_t, g_n, g_f);
	Mat4x4 mat_identity = mat4x4_identity();
	Mat4x4 mat_scale = mat4x4_scale(mat_identity, (Vec4 ) { 10, 10, 10, 1 });
	Mat4x4 mat_rotation = mat4x4_rotate(mat_scale, rotation_axe, degToRad(g_rotation));
	Mat4x4 mat_translate = mat4x4_translate_composed(mat_rotation, (Vec4 ) { 0, 0, -50, 0 });

	int projectionUnifLoc = glGetUniformLocation(shaderProgramsIDs[3], "projection");
	int transformUnifLoc = glGetUniformLocation(shaderProgramsIDs[3], "transform");
	glUseProgram(shaderProgramsIDs[3]);
	//glUniformMatrix4fv(projectionUnifLoc, 1, GL_TRUE, &mat_ortho);
	glUniformMatrix4fv(projectionUnifLoc, 1, GL_TRUE, &mat_persp);
	glUniformMatrix4fv(transformUnifLoc, 1, GL_TRUE, &mat_translate);
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
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
		printf("Failed to initialize GLAD\n");
		return -1;
	}

	Triangle t1, t2, t3, t4;
	Cube c1;
	GameState game_state;
	game_state.t1 = &t1;
	game_state.t2 = &t2;
	game_state.t3 = &t3;
	game_state.t4 = &t4;

	glfwSetWindowUserPointer(window, &game_state);

	triangle_set_vertices(&t1, triangle1_vertices);
	triangle_set_vertices(&t2, triangle2_vertices);
	triangle_set_vertices(&t3, triangle1_big);
	triangle_set_vertices(&t4, triangle_vertices);
	triangle_load_in_buffer(&t1);
	triangle_load_in_buffer(&t2);
	triangle_load_in_buffer(&t3);
	triangle_load_in_buffer(&t4);

	cube_set_vertices(&c1, cube_verices);
	cube_load_in_buffer(&c1);

	float l = -40;
	float r = 40;
	float b = -30;
	float t = 30;
	float n = 1;
	float f = 100;
	Mat4x4 mat_persp = mat4x4_perspective_projection(l, r, b, t, n, f);
	Mat4x4 mat_ortho = mat4x4_orthographic_projection(l, r, b, t, n, f);
	Mat4x4 mat_identity = mat4x4_identity();
	Mat4x4 mat_scale = mat4x4_scale(mat_identity, (Vec4 ) { 1, 1, 1, 1 });
	Mat4x4 mat_translate = mat4x4_translate_composed(mat_scale, (Vec4 ) { 0, 0, 0, 0 });

	mat4x4_print(&mat_persp);
	mat4x4_print(&mat_ortho);
	mat4x4_print(&mat_identity);
	mat4x4_print(&mat_scale);
	mat4x4_print(&mat_translate);

	printf("hola\n");

//triangle_use_vao(&t4);
	cube_use_vao(&c1);
	shaderProgramsIDs[0] = shaders_install("shader.gls", "fragment.gls");
	shaderProgramsIDs[1] = shaders_install("shader2.gls", "fragment2.gls");
	shaderProgramsIDs[2] = shaders_install("shader3.gls", "fragment3.gls");
	shaderProgramsIDs[3] = shaders_install("shader4.gls", "fragment4.gls");	//has uniform

	int projectionUnifLoc = glGetUniformLocation(shaderProgramsIDs[3], "projection");
	int transformUnifLoc = glGetUniformLocation(shaderProgramsIDs[3], "transform");
	glUseProgram(shaderProgramsIDs[3]);
	glUniformMatrix4fv(projectionUnifLoc, 1, GL_TRUE, &mat_persp);
	glUniformMatrix4fv(transformUnifLoc, 1, GL_TRUE, &mat_identity);

	shader_hi();
	printf("\nSMEGMA\n");
	glUseProgram(shaderProgramsIDs[1]);

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glEnable(GL_DEPTH_TEST);
	while (!glfwWindowShouldClose(window)) {
		// input
		// -----
		processInput(window);
		//float time = glfwGetTime();

		//Mat4x4 m = mat4x4_rotate(mat_identity, v, degToRad(20));

		// render
		// ------
		//glUniformMatrix4fv(transformUnifLoc, 1, GL_TRUE, &m);

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		//triangle_use_vao(&t1);
		//glDrawArrays(GL_TRIANGLES, 0, 3);
		glDrawElements(GL_TRIANGLES, 6*6, GL_UNSIGNED_INT, 0);
		//triangle_use_vao(&t2);
		//glDrawArrays(GL_TRIANGLES, 0, 3);

//		drawWithUniform();

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
	Mat4x4 m5 = mat4x4_perspective_projection(l, r, b, t, n, f);
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
	GameState *game_state = (GameState*) glfwGetWindowUserPointer(window);
	char text[10];

	if (action == GLFW_PRESS || action == GLFW_REPEAT) {
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

		if (key == GLFW_KEY_N && mods == GLFW_MOD_SHIFT) {
			printf("pressed: Shift + n\n");
			g_n -= 1;
		}
		if (key == GLFW_KEY_N && mods != GLFW_MOD_SHIFT) {
			printf("pressed: n\n");
			g_n += 1;
		}

		if (key == GLFW_KEY_F && mods == GLFW_MOD_SHIFT) {
			printf("pressed: Shift + f\n");
			g_f -= 1;
		}
		if (key == GLFW_KEY_F && mods != GLFW_MOD_SHIFT) {
			printf("pressed: f\n");
			g_f += 1;
		}

		if (key == GLFW_KEY_L && mods == GLFW_MOD_SHIFT) {
			printf("pressed: Shift + l\n");
			g_l -= 1;
		}
		if (key == GLFW_KEY_L && mods != GLFW_MOD_SHIFT) {
			printf("pressed: l\n");
			g_l += 1;
		}

		if (key == GLFW_KEY_R && mods == GLFW_MOD_SHIFT) {
			printf("pressed: Shift + r\n");
			g_r -= 1;
		}
		if (key == GLFW_KEY_R && mods != GLFW_MOD_SHIFT) {
			printf("pressed: r\n");
			g_r += 1;
		}

		if (key == GLFW_KEY_B && mods == GLFW_MOD_SHIFT) {
			printf("pressed: Shift + b\n");
			g_b -= 1;
		}
		if (key == GLFW_KEY_B && mods != GLFW_MOD_SHIFT) {
			printf("pressed: b\n");
			g_b += 1;
		}

		if (key == GLFW_KEY_T && mods == GLFW_MOD_SHIFT) {
			printf("pressed: Shift + t\n");
			g_t -= 1;
		}
		if (key == GLFW_KEY_T && mods != GLFW_MOD_SHIFT) {
			printf("pressed: t\n");
			g_t += 1;
		}

		if (key == GLFW_KEY_J && mods == GLFW_MOD_SHIFT) {
			printf("pressed: Shift + j\n");
			g_rotation -= 1;
		}
		if (key == GLFW_KEY_J && mods != GLFW_MOD_SHIFT) {
			printf("pressed: j\n");
			g_rotation += 1;
		}
		update_uniform_projection_matrix();

	}

//	printf("action pressed: %d\n", action);
//	printf("key pressed: %d\n", key);
//	printf("key pressedc: %c\n", key);
//	printf("mod pressed: %d\n", mods);
//	printf("mod pressedc: %c\n", mods);
//	printf("====\n");

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
			triangle_use_vao((*game_state).t1);
			//glBindVertexArray(VAO_TRIANGLE1);
			return;
		}
		if (0 == strcmp(text, "tri2")) {
			printf("triangle 2:\n");
			//glBindVertexArray(VAO_TRIANGLE2);
			triangle_use_vao((*game_state).t2);
			return;
		}
		if (0 == strcmp(text, "tri3")) {
			printf("triangle 3:\n");
			triangle_use_vao((*game_state).t3);
			return;
		}
	}
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
	printf("Oh yea\n");
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, 1);
}

