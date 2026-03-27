/*
 * shapes.h
 * this header has no .c
 *
 *  Created on: Mar 27, 2026
 *      Author: bomber
 */

#ifndef INCLUDE_SHAPES_H_
#define INCLUDE_SHAPES_H_

//@formatter:off

static float cube_verices[8 * 3] = {
	//V1
	-0.5, -0.5, 0.5,
	//V2
	0.5, -0.5, 0.5,
	//V3
	0.5, 0.5, 0.5,
	//V4
	-0.5, 0.5, 0.5,


	//V5
	-0.5, -0.5, -0.5,
	//V6
	0.5, -0.5, -0.5,
	//V7
	0.5, 0.5, -0.5,
	//V8
	-0.5, 0.5, -0.5,
};

unsigned int cube_indices_ebo[] = {  // note that we start from 0!
		// front
		0, 1, 2,
		2, 3, 0,
		// right
		1, 5, 6,
		6, 2, 1,
		// back
		7, 6, 5,
		5, 4, 7,
		// left
		4, 0, 3,
		3, 7, 4,
		// bottom
		4, 5, 1,
		1, 0, 4,
		// top
		3, 2, 6,
		6, 7, 3
};


	//platonic equilateral triangle
	float triangle_vertices[] = {
			-0.5f, -0.5f, 0.0f,
			0.5f, -0.5f, 0.0f,
			0.0f,  0.5f, 0.0f
	};

	float triangle1_big[] = {
			200.0f, 200.0f, -2.0f,
			150.0f, -205.0f, -2.0f,
			300.0f, 70.0f, -2.0f
	};

	float triangle1_vertices[] = {
			-0.9f, -0.5f, 0.0f,
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



#endif /* INCLUDE_SHAPES_H_ */
