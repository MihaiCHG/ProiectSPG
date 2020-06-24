#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <stdio.h>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <glm/mat4x4.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/constants.hpp>
#include "spheremesh.h"
#include <vector>
#include <stack>

#define PI glm::pi<float>()

namespace myGame {

	class Object3d {
		
	protected:
		
		GLuint vbo;
		glm::vec3 color;

	public:
		float x, y, z;
		GLuint vao;
		Object3d(float _x, float _y, float _z) {
			x = _x;
			y = _y;
			z = _z;
		}

		~Object3d() {};

		virtual void DrawObject(GLuint shader_programme, glm::mat4 viewMatrix, glm::mat4 projectionMatrix)=0;

	};


	float pointsBox[] = {
	//coord				normala		cord textura
	// face x positive
	0.5, -0.5, -0.5,	1, 0, 0,	0, 0, 0,
	0.5, -0.5, 0.5,		1, 0, 0,	1, 0, 0,
	0.5, 0.5, 0.5,		1, 0, 0,	1, 1, 0,
	0.5, 0.5, -0.5,		1, 0, 0,	0, 1, 0,

	// face x negative
	-0.5, -0.5, -0.5,	-1, 0, 0,	0, 0, 0,
	-0.5, -0.5, 0.5,	-1, 0, 0,	1, 0, 0,
	-0.5, 0.5, 0.5,		-1, 0, 0,	1, 1, 0,
	-0.5, 0.5, -0.5,	-1, 0, 0,	0, 1, 0,

	// face y positive
	0.5, 0.5, -0.5,		0, 1, 0,	1, 0, 0,
	0.5, 0.5, 0.5,		0, 1, 0,	0, 0, 0,
	-0.5, 0.5, 0.5,		0, 1, 0,	0, 1, 0,
	-0.5, 0.5, -0.5,	0, 1, 0,	1, 1, 0,

	// face y negative
	0.5, -0.5, -0.5,	0, -1, 0,	1, 0, 0,
	0.5, -0.5, 0.5,		0, -1, 0,	0, 0, 0,
	-0.5, -0.5, 0.5,	0, -1, 0,	0, 1, 0,
	-0.5, -0.5, -0.5,	0, -1, 0,	1, 1, 0,

	// face z positive
	0.5, -0.5, 0.5,		0, 0, 1,	1, 0, 0,
	0.5, 0.5, 0.5,		0, 0, 1,	1, 1, 0,
	-0.5, 0.5, 0.5,		0, 0, 1,	0, 1, 0,
	-0.5, -0.5, 0.5,	0, 0, 1,	0, 0, 0,

	// face z negative
	0.5, -0.5, -0.5,	0, 0, -1,	1, 0, 0,
	0.5, 0.5, -0.5,		0, 0, -1,	1, 1, 0,
	-0.5, 0.5, -0.5,	0, 0, -1,	0, 1, 0,
	-0.5, -0.5, -0.5,	0, 0, -1,	0, 0, 0
	};
}