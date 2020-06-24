#pragma once
#include "DynamicObject.h"
#include <math.h>
#include "Rectangle.h"
#include "Bowling.h"

namespace myGame {

	class Ball : public DynamicObject {
		
		GLuint ebo;

	public:
		glm::mat4 modelMatrix, normalMatrix;
		float radius;
		float axisRotAngle; // unghiul de rotatie in jurul propriei axe
		float translatez;
		int sphereElementCount;
		Ball(float _x, float _y, float _z, float _radius) : DynamicObject(_x, _y, _z) {
			radius = _radius;
			moving = false;
			axisRotAngle = PI / 16.0;
			orentation = 3*PI/2;
			translatez = 0;
			mass = 20;
			SphereMesh sphere;
			color = glm::vec3(0.2, 1.0, 0.7);
			sphereElementCount = (GLsizei)sphere.triangles.size() * sizeof(glm::ivec3);
			glGenBuffers(1, &vbo);
			glBindBuffer(GL_ARRAY_BUFFER, vbo);
			glBufferData(GL_ARRAY_BUFFER, sphere.vertices.size() * sizeof(glm::vec3), sphere.vertices.data(), GL_STATIC_DRAW);

			glGenVertexArrays(1, &vao);
			glBindVertexArray(vao);
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);

			glGenBuffers(1, &ebo);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sphereElementCount, sphere.triangles.data(), GL_STATIC_DRAW);

		}

		~Ball(){}

		void Update() {
			if (moving == true) {
				v = v*0.994;
				axisRotAngle += v * 0.5;
				if (v<=0.001) {
					v = 0;
					v = 0;
					moving = false;
				}
				else {
					x += v * cos(orentation);
					z += v * sin(orentation);
				}
			}
			
		}

		void DrawObject(GLuint shader_programme, glm::mat4 viewMatrix, glm::mat4 projectionMatrix) {
			glBindVertexArray(vao);

			GLuint typeObject = glGetUniformLocation(shader_programme, "typeObject");
			glUniform1i(typeObject, 1);//obiect de tip minge
			GLuint matrixID = glGetUniformLocation(shader_programme, "modelViewProjectionMatrix");
			modelMatrix = glm::mat4(); // matricea de modelare este matricea identitate
			
			modelMatrix *= glm::translate(glm::vec3(x, radius+y, z));
			modelMatrix *= glm::scale(glm::vec3( radius));
			modelMatrix *= glm::rotate(axisRotAngle, glm::vec3(sin(orentation), 0, -cos(orentation)));

			glUniformMatrix4fv(matrixID, 1, GL_FALSE, glm::value_ptr(projectionMatrix * viewMatrix * modelMatrix));

			normalMatrix = glm::transpose(glm::inverse(modelMatrix));
			GLuint normalMatrixLoc = glGetUniformLocation(shader_programme, "normalMatrix");
			glUniformMatrix4fv(normalMatrixLoc, 1, GL_FALSE, glm::value_ptr(normalMatrix));

			GLuint colorID = glGetUniformLocation(shader_programme, "col");
			glUniform3fv(colorID, 1, glm::value_ptr(color));
			glDrawElements(GL_TRIANGLES, sphereElementCount, GL_UNSIGNED_INT, NULL);
		}

		void collisonWithStaticObject(Rectangle* r) {
			if (x-radius < r->x + r->width/2 &&
				x + radius > r->x - r->width / 2 &&
				z - radius< r->z + r->length/2 &&
				z + radius > r->z - r->length / 2)
			{
				float suprapunerex = x - r->x - radius - r->width / 2+0.3; 
				float suprapunerez = z - r->z - radius - r->length/ 2+0.3; 
				if (r->typeCollision== 1) {
					x += suprapunerex;
				}
				else if (r->typeCollision==2) {
					z -= suprapunerez;
				}
				if ((orentation > 0 && orentation < PI / 2) || (orentation > 3*PI/2 && orentation < 2*PI)) {
					
					if (r->typeCollision == 2) {
						if ((orentation > 0 && orentation < PI / 2))
						{
							orentation -= (orentation) * 2;
						}
						else if ((orentation > 3 * PI / 2 && orentation < 2 * PI)) {
							orentation += (2*PI-orentation) * 2;
						}
					}
					else {
						if ((orentation > 0 && orentation < PI / 2))
						{
							orentation += (PI / 2 - orentation) * 2;
						}
						else if ((orentation > 3 * PI / 2 && orentation < 2 * PI)) {
							orentation -= (orentation - 3 * PI / 2) * 2;
						}
					}
				}
				else {
					if (r->typeCollision == 2) {
						if (orentation > PI) {
							orentation -= (orentation - PI) * 2;
						}
						else {
							orentation += (PI - orentation) * 2;
						}
					}
					else {
						if (orentation > PI) {
							orentation += (3 * PI / 2 - orentation) * 2;
						}
						else {
							orentation -= (PI / 2 - orentation) * 2;
						}
					}
				}
				if(orentation>2*PI)
					orentation -= 2 * PI;
				if (orentation < 0)
					orentation += 2 * PI;
			}
		}

		void collisonWithBowling(Bowling * b) {
			if (x - radius < b->x + b->scale / 2 &&
				x + radius > b->x - b->scale / 2 &&
				z - radius< b->z + b->scale / 2 &&
				z + radius > b->z - b->scale / 2)
			{
				if ((orentation > 0 && orentation < PI / 2) || (orentation > 3 * PI / 2 && orentation < 2 * PI)) {

					if ((orentation > 0 && orentation < PI / 2))
					{
						orentation += (PI / 2 - orentation) * 2;
					}
					else if ((orentation > 3 * PI / 2 && orentation < 2 * PI)) {
						orentation -= (orentation - 3 * PI / 2) * 2;
					}
				}
				else {
					if (orentation > PI) {
						orentation += (3 * PI / 2 - orentation) * 2;
					}
					else {
						orentation -= (PI / 2 - orentation) * 2;
					}
				}
				
				b->orentation = atan((b->z - z)/(b->x - x));
				b->v = v * 0.8;
				b->moving = true;
				b->touched = true;
			}
		}
	};
}