/*
 * shader_manager.c
 *
 *  Created on: Mar 14, 2026
 *      Author: bomber
 */
#include "glad/glad.h"
#include "shader_manager.h"
#include <stdio.h>

void shader_hi(){
	printf("Hi from shader manager\n");
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
GLuint shaders_install(char const *vertexshader, char const *fragmentshader) {
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

GLuint shaderProgramID = 0;
//Old function
void shaders_install_old() {

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
