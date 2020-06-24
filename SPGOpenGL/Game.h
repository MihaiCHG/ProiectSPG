#pragma once
#include "Object3d.h"
#include "Ball.h"
#include "Rectangle.h"
#include "Bowling.h"
#include "Line.h"

#define  STB_IMAGE_IMPLEMENTATION 1

#include "stb_image.h"


namespace myGame {

	class Game {
	private:

		myGame::Ball* b;
		myGame::Rectangle* r;
		myGame::Bowling** bowling;
		myGame::Rectangle** margins;
		myGame::Line* line;
		float power;
		bool gameStarted;

	public:
		Game() {
			setGame();
		}

		~Game() {

		}
		void setGame() {
			power = 0.0;
			gameStarted = false;
			line = new myGame::Line(0, 0);
			b = new myGame::Ball(0, 0, 80, 2.0);
			r = new myGame::Rectangle(0, 0, 0, 20, 0.5, 170, 0);
			margins = new myGame::Rectangle * [3];
			margins[0] = new myGame::Rectangle(-11, 0, 0, 1.5, 4.0, 170, 1);
			margins[1] = new myGame::Rectangle(11, 0, 0, 1.5, 4.0, 170, 1);
			margins[2] = new myGame::Rectangle(0, 0, -87, 23, 4.0, 4.0, 2);
			bowling = new myGame::Bowling * [10];
			bowling[0] = new myGame::Bowling(-4, 0, -83, 0.2, 1);
			bowling[1] = new myGame::Bowling(-1.5, 0, -83, 0.2, 2);
			bowling[2] = new myGame::Bowling(1.5, 0, -83, 0.2, 3);
			bowling[3] = new myGame::Bowling(4, 0, -83, 0.2, 3);
			bowling[4] = new myGame::Bowling(-2.5, 0, -81, 0.2, 3);
			bowling[5] = new myGame::Bowling(0, 0, -81, 0.2, 4);
			bowling[6] = new myGame::Bowling(2.5, 0, -81, 0.2, 5);
			bowling[7] = new myGame::Bowling(-1.5, 0, -79, 0.2, 6);
			bowling[8] = new myGame::Bowling(1.5, 0, -79, 0.2, 6);
			bowling[9] = new myGame::Bowling(0, 0, -77, 0.2, 6);
		}

		void Update() {
			for (int i = 0; i < 3; i++) {
				b->collisonWithStaticObject(margins[i]);
			}
			for (int i = 0; i < 10; i++) {
				b->collisonWithBowling(bowling[i]);
				for (int j = 0; j < 3; j++) {
					bowling[i]->collisonWithStaticObject(margins[j]);
				}
				for (int j = 0; j < 10; j++) {
					if (bowling[i]->id != bowling[j]->id) {
						bowling[i]->collisonWithBowling(bowling[j]);
					}
				}
			}
			b->Update();
			bool movingObjects = false;
			for (int i = 0; i < 10; i++) {
				bowling[i]->Update();
				
			}
			for (int i = 0; i < 10; i++) 
			{
				if (bowling[i]->moving)
				{
					movingObjects = true;
					break;
				}
			}
			if (!movingObjects) {
				if (gameStarted && !b->moving) {
					int bowlingsTouched=0;
					for (int i = 0; i < 10; i++) {
						if (bowling[i]->touched)
							bowlingsTouched++;
					}
					std::cout << "Ai lovit " << bowlingsTouched << "/10 popice." << std::endl;
					setGame();
				}
			}
		}

		void Render(GLuint shader_programme, glm::mat4 viewMatrix, glm::mat4 projectionMatrix) {
			
			b->DrawObject(shader_programme, viewMatrix, projectionMatrix);

			r->DrawObject(shader_programme, viewMatrix, projectionMatrix);
			for (int i = 0; i < 3; i++) {
				margins[i]->DrawObject(shader_programme, viewMatrix, projectionMatrix);
			}
			for (int i = 0; i < 10; i++) {
				bowling[i]->DrawObject(shader_programme, viewMatrix, projectionMatrix);
			}
			line->DrawLine(shader_programme, viewMatrix, projectionMatrix);
		}

		void keyboard(unsigned char key, int x, int y)
		{
			switch (key)
			{
			case 's':
				if (b->orentation < 3 * PI / 2 + PI / 4) {
					b->orentation += 0.05;
				}
				if (line->orentation > PI / 4) {
					line->orentation -= 0.05;
				}

				break;
			case 'a':
				if (b->orentation > 3 * PI / 2 - PI / 4) {
					b->orentation -= 0.05;
				}
				if (line->orentation < PI / 2 + PI / 4) {
					line->orentation += 0.05;
				}
				break;

			case 'z':
				if (b->x > -8) {
					b->x -= 0.3;
					line->x1 -= 0.3;
				}

				break;
			case 'x':
				if (b->x < 8) {
					b->x += 0.3;
					line->x1 += 0.3;
				}
				break;
			};
			glutPostRedisplay(); // cauzeaza redesenarea ferestrei
		}


		void increasePower(void)
		{
			if (power < 1.3) {
				power += 0.1;
			}

			glutPostRedisplay();
		}
		void mouse(int button, int state, int x, int y)
		{
			switch (button)
			{
			case GLUT_LEFT_BUTTON:
				if (state == GLUT_DOWN)
				{
					power = 0.0;
				}
				else
				{
					if (!gameStarted) {
						b->v = power;
						b->moving = true;
						glutPostRedisplay();
						gameStarted = true;
					}
				}
				break;
			default:
				break;
			}
		}
		void loadTexture(const char* path, const char* nameTexInFragment, GLuint shader_programme, GLuint gl_Texture,  int idTex) {
			GLuint texture;
			glGenTextures(1, &texture);
			glActiveTexture(gl_Texture);
			glBindTexture(GL_TEXTURE_2D, texture);
			int width, height, nrChannels;
			stbi_set_flip_vertically_on_load(true);
			unsigned char* data = stbi_load(path, &width, &height, &nrChannels, 0);
			if (data)
			{
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
				glGenerateMipmap(GL_TEXTURE_2D);
			}
			else
			{
				std::cout << "Failed to load texture" << std::endl;
			}
			stbi_image_free(data);
			glUniform1i(glGetUniformLocation(shader_programme, nameTexInFragment), idTex);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		}
	};

}