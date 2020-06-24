#pragma once
#include "Object3d.h"

namespace myGame {

	class Line {


	public:
		float x1, z1;
		GLuint vbo, vao;
		glm::mat4 modelMatrix;
		float orentation;
		Line(float _x1, float _z1) {
			x1 = _x1;
			z1 = _z1;
			orentation = PI / 2;
			float points[] = {
				0.0f, 0.0f, 0.0f,
				16.0f, 0.0f, 0.0f
			};

			glGenBuffers(1, &vbo);
			glBindBuffer(GL_ARRAY_BUFFER, vbo);
			glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(float), points, GL_STATIC_DRAW);

			glGenVertexArrays(1, &vao);
			glBindVertexArray(vao);
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
		}

		void DrawLine(GLuint shader_programme, glm::mat4 viewMatrix, glm::mat4 projectionMatrix) {
			glBindVertexArray(vao);

			GLuint typeObject = glGetUniformLocation(shader_programme, "typeObject");
			glUniform1i(typeObject, 4);//obiect de tip linie

			modelMatrix = glm::mat4(); // matricea de modelare este matricea identitate

			modelMatrix *= glm::translate(glm::vec3(x1, 2, 80));
			modelMatrix *= glm::rotate(orentation, glm::vec3(0, 1, 0));
			GLuint modelMatrixLoc = glGetUniformLocation(shader_programme, "modelViewProjectionMatrix");
			glUniformMatrix4fv(modelMatrixLoc, 1, GL_FALSE, glm::value_ptr(projectionMatrix * viewMatrix * modelMatrix));


			glm::mat4 normalMatrix = glm::transpose(glm::inverse(modelMatrix));
			GLuint normalMatrixLoc = glGetUniformLocation(shader_programme, "normalMatrix");
			glUniformMatrix4fv(normalMatrixLoc, 1, GL_FALSE, glm::value_ptr(normalMatrix));

			glDrawArrays(GL_LINES, 0, 2);
		}
	};

}