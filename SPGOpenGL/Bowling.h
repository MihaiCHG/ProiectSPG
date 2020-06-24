#pragma once
#include "DynamicObject.h"
#include "ObjectLoader.h"



namespace myGame {

	class BowlingObjectLoader {
		std::vector< glm::vec3 > points;
		BowlingObjectLoader() {
			std::vector<Vertex> obj = loadOBJ("obj/Mid Poly.obj");
			for (int i = 0; i < obj.size(); i++) {
				points.push_back(obj[i].position);	
			}
			for (int i = 0; i < obj.size(); i++) {
				points.push_back(obj[i].normal);
			}
			for (int i = 0; i < obj.size(); i++) {
				points.push_back(obj[i].texcoord);
			}
		}

	public:
		static BowlingObjectLoader* instance;
		static BowlingObjectLoader* getInstance() {
			if (!instance)
				instance = new BowlingObjectLoader();
			return instance;
		}
		std::vector< glm::vec3 > getPoints() {
			return points;
		}
	};
	BowlingObjectLoader* BowlingObjectLoader::instance = nullptr;


	class Bowling : public DynamicObject {


	public:
		glm::mat4 modelMatrix, normalMatrix;
		std::vector< glm::vec3 > points;
		float scale;
		int id;
		bool touched;
		Bowling(float _x, float _y, float _z, float _scale, int _id):DynamicObject(_x, _y, _z) {
			color = glm::vec3(0.2, 1.0, 0.7);
			scale = _scale;
			mass = 10;
			id = _id;
			touched = false;
			BowlingObjectLoader *bObj = bObj->getInstance();
			points = bObj->getPoints();
			glGenBuffers(1, &vbo);
			glBindBuffer(GL_ARRAY_BUFFER, vbo);
			glBufferData(GL_ARRAY_BUFFER, points.size() * sizeof(glm::vec3), &points[0], GL_STATIC_DRAW);

			glGenVertexArrays(1, &vao);
			glBindVertexArray(vao);

			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);

			glEnableVertexAttribArray(1);
			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)(points.size()/3 * sizeof(glm::vec3)));
			glEnableVertexAttribArray(2);
			glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, (void*)((points.size()*2/3) * sizeof(glm::vec3)));
		}

		~Bowling(){}

		void Update(){
			if (moving == true) {
				v = v * 0.994;
				if (v <= 0.001) {
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

		void DrawObject(GLuint shader_programme, glm::mat4 viewMatrix, glm::mat4 projectionMatrix) 
		{
			glBindVertexArray(vao);

			GLuint typeObject = glGetUniformLocation(shader_programme, "typeObject");
			glUniform1i(typeObject, 3);//obiect de tip bowling

			modelMatrix = glm::mat4(); // matricea de modelare este matricea identitate
			
			modelMatrix *= glm::translate(glm::vec3(x, y, z));
			modelMatrix *= glm::scale(glm::vec3(scale, scale, scale));
			modelMatrix *= glm::rotate(-PI / 2, glm::vec3(0, 1, 0));
			GLuint modelMatrixLoc = glGetUniformLocation(shader_programme, "modelViewProjectionMatrix");
			glUniformMatrix4fv(modelMatrixLoc, 1, GL_FALSE, glm::value_ptr(projectionMatrix * viewMatrix * modelMatrix));


			glm::mat4 normalMatrix = glm::transpose(glm::inverse(modelMatrix));
			GLuint normalMatrixLoc = glGetUniformLocation(shader_programme, "normalMatrix");
			glUniformMatrix4fv(normalMatrixLoc, 1, GL_FALSE, glm::value_ptr(normalMatrix));

			glDrawArrays(GL_TRIANGLE_FAN, 0, points.size()/3);
		}

		void collisonWithStaticObject(Rectangle* r) {
			if (x - scale*8 < r->x + r->width / 2 &&
				x + scale * 8 > r->x - r->width / 2 &&
				z - scale * 8 < r->z + r->length / 2 &&
				z + scale * 8 > r->z - r->length / 2)
			{
				if ((orentation > 0 && orentation < PI / 2) || (orentation > 3 * PI / 2 && orentation < 2 * PI)) {

					if (r->typeCollision == 2) {
						if ((orentation > 0 && orentation < PI / 2))
						{
							orentation -= (orentation) * 2;
						}
						else if ((orentation > 3 * PI / 2 && orentation < 2 * PI)) {
							orentation += (2 * PI - orentation) * 2;
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
				if (orentation > 2 * PI)
					orentation -= 2 * PI;
				if (orentation < 0)
					orentation += 2 * PI;
			}
		}

		void collisonWithBowling(Bowling* b) {
			if (x - scale*4 < b->x + b->scale * 4 &&
				x + scale*4 > b->x - b->scale * 4 &&
				z - scale*4< b->z + b->scale * 4 &&
				z + scale*4 > b->z - b->scale * 4)
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
				b->orentation = orentation + PI;
				if (b->orentation > 2 * PI)
					b->orentation -= 2 * PI;
				b->v = v*0.8;
				b->moving = true;
				b->touched = true;
			}
		}

	};


	

}