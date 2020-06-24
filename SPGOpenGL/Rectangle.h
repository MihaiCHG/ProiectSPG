#pragma once


#include "Object3d.h"

namespace myGame {

	class Rectangle : public Object3d {

		
		float * points;
	public:
		float length, width, height;
		glm::mat4 modelMatrix, normalMatrix;
		int typeCollision;// 1- on x, 2 on z
		Rectangle(float _x, float _y, float _z, float _width, float _height, float _length, int _typeCollision) :Object3d(_x, _y, _z) {
			length = _length;
			typeCollision = _typeCollision;
			width = _width;
			height = _height;
			color = glm::vec3(0.2, 0.6, 1.0);
			points = new float[216];
			for (int i = 0; i < 216; i++) {
				points[i] = pointsBox[i];
			}

			glGenBuffers(1, &vbo);
			glBindBuffer(GL_ARRAY_BUFFER, vbo);
			glBufferData(GL_ARRAY_BUFFER, 216 * sizeof(float), points, GL_STATIC_DRAW);

			glGenVertexArrays(1, &vao);
			glBindVertexArray(vao);
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), 0);

			//atrivutul pentru normale
			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(3 * sizeof(float)));
			glEnableVertexAttribArray(1);

			glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(6 * sizeof(float)));
			glEnableVertexAttribArray(2);
		}
		 
		void DrawObject(GLuint shader_programme, glm::mat4 viewMatrix, glm::mat4 projectionMatrix) {
			glBindVertexArray(vao);

			GLuint typeObject = glGetUniformLocation(shader_programme, "typeObject");
			glUniform1i(typeObject, 2);//obiect de tip rectangle

			GLuint matrixID = glGetUniformLocation(shader_programme, "modelViewProjectionMatrix");
			modelMatrix = glm::mat4(); // matricea de modelare este matricea identitate

			modelMatrix *= glm::translate(glm::vec3(x, y, z));

			modelMatrix *= glm::scale(glm::vec3(width, height, length));

			glUniformMatrix4fv(matrixID, 1, GL_FALSE, glm::value_ptr(projectionMatrix * viewMatrix * modelMatrix));

			normalMatrix = glm::transpose(glm::inverse(modelMatrix));
			GLuint normalMatrixLoc = glGetUniformLocation(shader_programme, "normalMatrix");
			glUniformMatrix4fv(normalMatrixLoc, 1, GL_FALSE, glm::value_ptr(normalMatrix));

			GLuint colorID = glGetUniformLocation(shader_programme, "col");
			glUniform3fv(colorID, 1, glm::value_ptr(color));
			glDrawArrays(GL_QUADS, 0, 34);
		}
	};
}