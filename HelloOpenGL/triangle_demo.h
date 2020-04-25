#ifndef _TRIANGLE_DEMO_H
#define _TRIANGLE_DEMO_H

#include "demo_base.h"
#include "bitmap.h"
#include <vector>
#include <iostream>

#include <fmod.hpp>
#include <fmod_errors.h>
#include <stdio.h>
#include <string>
#include <sstream>
#include<time.h>
#include <chrono>

using namespace std;

#define TEXTURE_COUNT 100

#define RECT_VERTICE_W 10
#define RECT_VERTICE_H 10

//every quad has 6
const int RECT_VERTEX_COUNT = (RECT_VERTICE_W - 1) * (RECT_VERTICE_H - 1) * 6;

const int RECT_VERTEX_ARRAY_SIZE = RECT_VERTEX_COUNT * 3;

const int RECT_UV_ARRAY_SIZE = RECT_VERTEX_COUNT * 2;

const int number_of_square = RECT_VERTICE_W * RECT_VERTICE_H;

const int number_of_vertices = number_of_square * 18;

const int number_of_texture = number_of_square * 12;

const int nunmber_of_color = number_of_square * 18;

#define SPECTRUM_SIZE 256

void ERRCHECK(FMOD_RESULT result) {

	if (result != FMOD_OK) {

		printf("FMOD error! (%d) %s\n",
			result, FMOD_ErrorString(result));

	}

}

class TriangleDemo : public DemoBase
{

private:

	float oldTime;
	float deltaTime;

	int songNum;

	float floatRate = 0;
	float floatRate1 = 0;

	float rotateRate = 0;
	float rotateRate1 = 0;

	float translateX = 0.0f, translateY = 0.0f, translateZ = 0.0f;
	float scaleX = 0.0f, scaleY = 0.0f, scaleZ = 0.0f;
	float rotateX = 0.0f, rotateY = 0.0f, rotateZ = 0.0f;
	float walkSpeed;
	float feetRotation = 0;
	int wanToGoX;

	float bodyRotation = 0;

	float jumpFeetAngle = 0;

	float jumpHeight = 0;
	float jumpSpeed = 0;

	const float number_of_x = 5;
	const float number_of_y = 2;

	//number of square * 18
	GLfloat vertices[number_of_vertices];
	GLfloat textCoords[number_of_texture];
	GLfloat colors[nunmber_of_color];

	int turn = 0;
	int moveX = 0;
	int moveY = 0;

	GLuint mTextureID[TEXTURE_COUNT];

	FMOD::System* m_fmodSystem;
	FMOD::Sound* m_music;
	FMOD::Channel* m_musicChannel;

	float m_spectrumLeft[SPECTRUM_SIZE];
	float m_spectrumRight[SPECTRUM_SIZE];

	GLfloat visual_vertices[1000];

	void loadTexture(const char* path, GLuint textureID) {

		CBitmap bitmap(path);

		//Create Linear Fltered Texture
		glBindTexture(GL_TEXTURE_2D, textureID);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		//old school(minecraft) filtering
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

		/*
		//bilinear filtering
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		*/

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, bitmap.GetWidth(), bitmap.GetHeight(), 0, GL_RGBA, GL_UNSIGNED_BYTE, bitmap.GetBits());

	}

	float m_rotation;
	float m_speed;
	bool onWayUp;

	float mUShift = 0;

public:

	void initFmod() {

		srand(time(NULL));

		songNum = rand() % 4;

		songNum = 3;

		FMOD_RESULT result;
		unsigned int version;

		result = FMOD::System_Create(&m_fmodSystem);
		ERRCHECK(result);

		result = m_fmodSystem->getVersion(&version);
		ERRCHECK(result);

		if (version < FMOD_VERSION) printf("FMOD Error! You are using an old version of FMOD.", version, FMOD_VERSION);

		result = m_fmodSystem->init(32, FMOD_INIT_NORMAL, 0);
		ERRCHECK(result);

		ostringstream songNameStream;
		songNameStream << "../media/music" << songNum << ".mp3";

		string songNameString = songNameStream.str();
		const char* songName = songNameString.data();

		cout << songName;
		result = m_fmodSystem->createStream(songName, FMOD_SOFTWARE, 0, &m_music);
		//result = m_fmodSystem->createStream("../media/music3.mp3", FMOD_SOFTWARE, 0, &m_music);
		ERRCHECK(result);

		result = m_fmodSystem->playSound(FMOD_CHANNEL_FREE, m_music, false, &m_musicChannel);
		ERRCHECK(result);

	}

	void init()
	{
		initFmod();

		m_rotation = 0.0f;
		m_speed = 0.0f;
		onWayUp = true;
		walkSpeed = 0;
		wanToGoX = 1;

		glGenTextures(TEXTURE_COUNT, mTextureID);

		loadTexture("../media/ElectricPurple.bmp", mTextureID[0]);
		loadTexture("../media/ElectricGreen.bmp", mTextureID[1]);
		loadTexture("../media/ElectricBlue.bmp", mTextureID[2]);
		loadTexture("../media/ElectricRed.bmp", mTextureID[3]);
		
		loadTexture("../media/background0.bmp", mTextureID[10]);

		loadTexture("../media/background1.bmp", mTextureID[11]);

		loadTexture("../media/background2.bmp", mTextureID[12]);

		loadTexture("../media/background3.bmp", mTextureID[13]);

		loadTexture("../media/deco0.bmp", mTextureID[20]);
		loadTexture("../media/deco1.bmp", mTextureID[21]);
		loadTexture("../media/deco2.bmp", mTextureID[22]);
		loadTexture("../media/deco3.bmp", mTextureID[23]);

	}

	void deinit()
	{
	}

	void updateFmod() {

		m_fmodSystem->update();

		m_musicChannel->getSpectrum(m_spectrumLeft, SPECTRUM_SIZE, 0, FMOD_DSP_FFT_WINDOW_RECT);
		m_musicChannel->getSpectrum(m_spectrumRight, SPECTRUM_SIZE, 1, FMOD_DSP_FFT_WINDOW_RECT);

	}

	void drawAxis(const Matrix& viewMatrix)
	{
		// ============draw axis.
		glLoadMatrixf((GLfloat*)viewMatrix.mVal);
		glBegin(GL_LINES);
		glColor3f(1.0f, 0.3f, 0.3f);
		glVertex3f(0.0f, 0.0f, 0.0f);
		glVertex3f(1.0f, 0.0f, 0.0f);

		glColor3f(0.3f, 1.0f, 0.3f);
		glVertex3f(0.0f, 0.0f, 0.0f);
		glVertex3f(0.0f, 1.0f, 0.0f);

		glColor3f(0.3f, 0.3f, 1.0f);
		glVertex3f(0.0f, 0.0f, 0.0f);
		glVertex3f(0.0f, 0.0f, 1.0f);
		glEnd();
		// ============================
	}

	void drawTriangle(void)
	{
		glBegin(GL_TRIANGLES);								// Drawing Using Triangles

		glColor3f(1.0f, 0.0f, 1.0f);

		glVertex3f(0.0f, 1.0f, 0.0f);
		glVertex3f(1.0f, 0.0f, 0.0f);
		glVertex3f(0.0f, 0.0f, 0.0f);

		glEnd();
	}

	void drawSquare(void) {

		glBegin(GL_TRIANGLES);								// Drawing Using Triangles

		//glColor3f(1.0f, 0.0f, 0.0f);

		glVertex3f(-0.5f, 0.5f, 0.0f);
		glVertex3f(-0.5f, -0.5f, 0.0f);
		glVertex3f(0.5f, -0.5f, 0.0f);

		//glColor3f(0.0f, 1.0f, 0.0f);

		glVertex3f(0.5f, -0.5f, 0.0f);
		glVertex3f(-0.5f, 0.5f, 0.0f);
		glVertex3f(0.5f, 0.5f, 0.0f);

		glEnd();
	}

	void drawCube(void) {

		glBegin(GL_TRIANGLES);								// Drawing Using Triangles

		//back
		glColor3f(1.0f, 0.0f, 0.0f);
		glVertex3f(-0.5f, 0.5f, 0.0f - 0.5f);
		glVertex3f(-0.5f, -0.5f, 0.0f - 0.5f);
		glVertex3f(0.5f, -0.5f, 0.0f - 0.5f);

		glVertex3f(0.5f, -0.5f, 0.0f - 0.5f);
		glVertex3f(-0.5f, 0.5f, 0.0f - 0.5f);
		glVertex3f(0.5f, 0.5f, 0.0f - 0.5f);

		//front
		glColor3f(0.0f, 1.0f, 0.0f);
		glVertex3f(-0.5f, 0.5f, 1.0f - 0.5f);
		glVertex3f(-0.5f, -0.5f, 1.0f - 0.5f);
		glVertex3f(0.5f, -0.5f, 1.0f - 0.5f);

		glVertex3f(0.5f, -0.5f, 1.0f - 0.5f);
		glVertex3f(-0.5f, 0.5f, 1.0f - 0.5f);
		glVertex3f(0.5f, 0.5f, 1.0f - 0.5f);

		//left
		glColor3f(0.0f, 0.0f, 1.0f);
		glVertex3f(-0.5f, 0.5f, 1.0f - 0.5f);
		glVertex3f(-0.5f, -0.5f, 1.0f - 0.5f);
		glVertex3f(-0.5f, -0.5f, 0.0f - 0.5f);

		glVertex3f(-0.5f, -0.5f, 0.0f - 0.5f);
		glVertex3f(-0.5f, 0.5f, 1.0f - 0.5f);
		glVertex3f(-0.5f, 0.5f, 0.0f - 0.5f);

		//right
		glColor3f(1.0f, 1.0f, 0.0f);
		glVertex3f(0.5f, 0.5f, 1.0f - 0.5f);
		glVertex3f(0.5f, -0.5f, 1.0f - 0.5f);
		glVertex3f(0.5f, -0.5f, 0.0f - 0.5f);

		glVertex3f(0.5f, -0.5f, 0.0f - 0.5f);
		glVertex3f(0.5f, 0.5f, 1.0f - 0.5f);
		glVertex3f(0.5f, 0.5f, 0.0f - 0.5f);


		//bot
		glColor3f(0.0f, 1.0f, 1.0f);
		glVertex3f(-0.5f, -0.5f, 1.0f - 0.5f);
		glVertex3f(0.5f, -0.5f, 1.0f - 0.5f);
		glVertex3f(0.5f, -0.5f, 0.0f - 0.5f);

		glVertex3f(-0.5f, -0.5f, 0.5f);
		glVertex3f(-0.5f, -0.5f, -0.5f);
		glVertex3f(0.5f, -0.5f, 0.0f - 0.5f);

		//top
		glColor3f(1.0f, 1.0f, 1.0f);
		glVertex3f(-0.5f, 0.5f, 1.0f - 0.5f);
		glVertex3f(0.5f, 0.5f, 1.0f - 0.5f);
		glVertex3f(0.5f, 0.5f, 0.0f - 0.5f);

		glVertex3f(-0.5f, 0.5f, 0.5f);
		glVertex3f(-0.5f, 0.5f, -0.5f);
		glVertex3f(0.5f, 0.5f, 0.0f - 0.5f);

		glEnd();

	}

	void drawPyramid(void) {

		glBegin(GL_TRIANGLES);								// Drawing Using Triangles

		//base
		glVertex3f(-0.5f, 0.0f - 0.5f, 1.0f - 0.5f);
		glVertex3f(0.5f, 0.0f - 0.5f, 1.0f - 0.5f);
		glVertex3f(0.5f, 0.0f - 0.5f, 0.0f - 0.5f);

		glVertex3f(-0.5f, 0.0f - 0.5f, 0.5f);
		glVertex3f(-0.5f, 0.0f - 0.5f, -0.5f);
		glVertex3f(0.5f, 0.0f - 0.5f, 0.0f - 0.5f);

		//Left
		glVertex3f(0.0f, 1.0f - 0.5f, 0.0f);
		glVertex3f(-0.5f, 0.0f - 0.5f, 0.0f);
		glVertex3f(-0.5f, 0.0f - 0.5f, -0.5f);

		glVertex3f(0.0f, 1.0f - 0.5f, 0.0f);
		glVertex3f(-0.5f, 0.0f - 0.5f, 0.0f);
		glVertex3f(-0.5f, 0.0f - 0.5f, 0.5f);

		//Right
		glVertex3f(0.0f, 1.0f - 0.5f, 0.0f);
		glVertex3f(0.5f, 0.0f - 0.5f, 0.0f);
		glVertex3f(0.5f, 0.0f - 0.5f, -0.5f);

		glVertex3f(0.0f, 1.0f - 0.5f, 0.0f);
		glVertex3f(0.5f, 0.0f - 0.5f, 0.0f);
		glVertex3f(0.5f, 0.0f - 0.5f, 0.5f);

		//Back
		glVertex3f(-0.5f, 0.0f - 0.5f, -0.5f);
		glVertex3f(0.0f, 0.0f - 0.5f, -0.5f);
		glVertex3f(0.0f, 1.0f - 0.5f, 0.0f);

		glVertex3f(0.5f, 0.0f - 0.5f, -0.5f);
		glVertex3f(0.0f, 0.0f - 0.5f, -0.5f);
		glVertex3f(0.0f, 1.0f - 0.5f, 0.0f);

		//Front
		glVertex3f(-0.5f, 0.0f - 0.5f, 0.5f);
		glVertex3f(0.0f, 0.0f - 0.5f, 0.5f);
		glVertex3f(0.0f, 1.0f - 0.5f, 0.0f);

		glVertex3f(0.5f, 0.0f - 0.5f, 0.5f);
		glVertex3f(0.0f, 0.0f - 0.5f, 0.5f);
		glVertex3f(0.0f, 1.0f - 0.5f, 0.0f);

		glEnd();

	}

	void drawCircle(GLfloat x, GLfloat y, GLfloat z, float r, int numberOfTriangles) {

		GLfloat doublePi = 2.0f * M_PI;

		for (int i = 0; i <= numberOfTriangles; i++) {

			glBegin(GL_TRIANGLES);

			glVertex3f(x, y, z); // center of circle

			glVertex3f(x + (r * cos(doublePi * i / numberOfTriangles)), y, z + (r * sin(doublePi * i / numberOfTriangles)));

			glVertex3f(x + (r * cos(doublePi * (i + 1) / numberOfTriangles)), y, z + (r * sin(doublePi * (i + 1) / numberOfTriangles)));

			glEnd();

		}

	}

	void drawCircleFan(GLfloat originx, GLfloat originy, GLfloat radius, int triangleAmount) {

		int i;

		float x = 0.0;
		float y = 0.0;

		//GLfloat radius = 0.8f; //radius
		GLfloat twicePi = 2.0f * M_PI;

		glBegin(GL_TRIANGLE_FAN);

		glVertex3f(originx, originy, 0); // center of circle

		for (i = 0; i <= triangleAmount; i++) {
			glVertex3f(
				y + (radius * sin(i * twicePi / triangleAmount)),
				originy,
				x + (radius * cos(i * twicePi / triangleAmount))
			);
		}

		glEnd();
	}

	void drawCylinder(GLfloat originx, GLfloat originy, GLfloat r, float height, int numberOfTriangles) {

		int i;

		float x = 0.0;
		float y = 0.0;

		GLfloat doublePi = 2.0f * M_PI;

		//top
		for (int i = 0; i <= numberOfTriangles; i++) {

			glBegin(GL_TRIANGLES);

			glVertex3f(originx, originy + height, 0); // center of circle

			glVertex3f(x + (r * cos(doublePi * i / numberOfTriangles)), originy + height, y + (r * sin(doublePi * i / numberOfTriangles)));

			glVertex3f(x + (r * cos(doublePi * (i + 1) / numberOfTriangles)), originy + height, y + (r * sin(doublePi * (i + 1) / numberOfTriangles)));

			glEnd();

		}

		//bot
		for (int i = 0; i <= numberOfTriangles; i++) {

			glBegin(GL_TRIANGLES);

			glVertex3f(originx, originy - height, 0); // center of circle

			glVertex3f(x + (r * cos(doublePi * i / numberOfTriangles)), originy - height, y + (r * sin(doublePi * i / numberOfTriangles)));

			glVertex3f(x + (r * cos(doublePi * (i + 1) / numberOfTriangles)), originy - height, y + (r * sin(doublePi * (i + 1) / numberOfTriangles)));

			glEnd();

		}

		for (i = 0; i <= numberOfTriangles; i++)
		{
			glBegin(GL_TRIANGLES);

			int temp = 1;

			if (i == numberOfTriangles) {
				temp = 0;
			}
			else {
				temp = i + 1;
			}

			glVertex3f(x + (r * sin(temp * doublePi / numberOfTriangles)),
				originy - height,
				y + (r * cos(temp * doublePi / numberOfTriangles))
			);

			glVertex3f(x + (r * sin(i * doublePi / numberOfTriangles)),
				originy - height,
				y + (r * cos(i * doublePi / numberOfTriangles))
			);

			glVertex3f(
				x + (r * sin(i * doublePi / numberOfTriangles)),
				originy + height,
				y + (r * cos(i * doublePi / numberOfTriangles))
			);

			//-------otherhalf

			glVertex3f(x + (r * sin(temp * doublePi / numberOfTriangles)),
				originy + height,
				y + (r * cos(temp * doublePi / numberOfTriangles))
			);

			glVertex3f(x + (r * sin(i * doublePi / numberOfTriangles)),
				originy + height,
				y + (r * cos(i * doublePi / numberOfTriangles))
			);

			glVertex3f(
				x + (r * sin(temp * doublePi / numberOfTriangles)),
				originy - height,
				y + (r * cos(temp * doublePi / numberOfTriangles))
			);


			glEnd();

		}

	}

	void drawCone(float x, float y, float z, float r, float numberOfTriangles, float height) {

		GLfloat doublePi = 2.0f * M_PI;

		float newr = 0.0f;
		float nextR = 0.0f;
		int decrease = numberOfTriangles;

		float newY = 0.00f;
		float nextY = 0.00f;


		for (int u = 0; u <= numberOfTriangles; u++) {

			glBegin(GL_TRIANGLES);

			int temp = 1;

			if (u == numberOfTriangles) {
				temp = 0;
			}
			else {
				temp = 1;
			}

			for (int i = 0; i <= numberOfTriangles; i++) {

				newr = r * ((decrease - u) / numberOfTriangles);

				nextR = r * ((decrease - (u + temp)) / numberOfTriangles);

				newY = y + (height * (u / numberOfTriangles));

				nextY = y + (height * (u + temp) / numberOfTriangles);

				glVertex3f(x + (newr * cos(doublePi * i / numberOfTriangles)), newY, z + (newr * sin(doublePi * i / numberOfTriangles)));

				glVertex3f(x + (newr * cos(doublePi * (i + temp) / numberOfTriangles)), newY, z + (newr * sin(doublePi * (i + temp) / numberOfTriangles)));

				glVertex3f(x + (nextR * cos(doublePi * (i + temp) / numberOfTriangles)), nextY, z + (nextR * sin(doublePi * (i + temp) / numberOfTriangles)));

				glVertex3f(x + (nextR * cos(doublePi * i / numberOfTriangles)), nextY, z + (nextR * sin(doublePi * i / numberOfTriangles)));

				glVertex3f(x + (nextR * cos(doublePi * (i + temp) / numberOfTriangles)), nextY, z + (nextR * sin(doublePi * (i + temp) / numberOfTriangles)));

				glVertex3f(x + (newr * cos(doublePi * i / numberOfTriangles)), newY, z + (newr * sin(doublePi * i / numberOfTriangles)));

			}




			glEnd();

		}
	}

	void drawSphere(float x, float y, float z, float radius, float numberOfTriangles) {

		float newX = 0.00000000000000f;
		float newY = 0.00000000000000f;
		float newZ = 0.00000000000000f;

		float nextX = 0.00f;
		float nextY = 0.00f;
		float nextZ = 0.00f;

		float normalX = 0.00f;
		float normalY = 0.00f;
		float normalZ = 0.00f;

		float nextNormalX = 0.00f;
		float nextNormalZ = 0.00f;

		float levelNormalX = 0.000f;
		float levelNormalZ = 0.000f;
		float levelNormalY = 0.000f;

		float levelNextNormalX = 0.000f;
		float levelNextNormalZ = 0.000f;
		float levelNextNormalY = 0.000f;

		for (int u = 0; u <= numberOfTriangles; u++) {

			if (u != 0) {

				newX = (radius * cos((M_PI / 2) - (M_PI * (u / numberOfTriangles))));
				newY = (radius * sin((M_PI / 2) - (M_PI * (u / numberOfTriangles))));
				newZ = (radius * cos((M_PI / 2) - (M_PI * (u / numberOfTriangles))));

				nextX = (radius * cos((M_PI / 2) - (M_PI * ((u + 1) / numberOfTriangles))));
				nextY = (radius * sin((M_PI / 2) - (M_PI * ((u + 1) / numberOfTriangles))));
				nextZ = (radius * cos((M_PI / 2) - (M_PI * ((u + 1) / numberOfTriangles))));

				normalY = (y + newY) * 1.0f / radius;
				levelNormalY = (y + nextY) * 1.0f / radius;
			}

			for (int i = 0; i <= numberOfTriangles; i++) {

				normalX = (x + (newX * cos(M_2PI * i / numberOfTriangles))) * 1.0f / radius;
				normalZ = (z + (newZ * sin(M_2PI * i / numberOfTriangles))) * 1.0f / radius;

				nextNormalX = (x + (newX * cos(M_2PI * (i + 1) / numberOfTriangles))) * 1.0f / radius;
				nextNormalZ = (z + (newZ * sin(M_2PI * (i + 1) / numberOfTriangles))) * 1.0f / radius;

				levelNormalX = (x + (nextX * cos(M_2PI * (i) / numberOfTriangles))) * 1.0f / radius;
				levelNormalZ = (z + (nextZ * sin(M_2PI * (i) / numberOfTriangles))) * 1.0f / radius;

				levelNextNormalX = (x + (nextX * cos(M_2PI * (i + 1) / numberOfTriangles))) * 1.0f / radius;
				levelNextNormalZ = (z + (nextZ * sin(M_2PI * (i + 1) / numberOfTriangles))) * 1.0f / radius;

				glBegin(GL_TRIANGLES);

				glVertex3f(x, normalY, z); // center of circle

				glVertex3f(normalX, normalY, normalZ);

				glVertex3f(nextNormalX, normalY, nextNormalZ);

				//

				glVertex3f(normalX, normalY, normalZ);

				glVertex3f(nextNormalX, normalY, nextNormalZ);

				glVertex3f(levelNormalX, levelNormalY, levelNormalZ);


				//

				glVertex3f(levelNextNormalX, levelNormalY, levelNextNormalZ);

				glVertex3f(levelNormalX, levelNormalY, levelNormalZ);

				glVertex3f(nextNormalX, normalY, nextNormalZ);

				glEnd();
			}
		}

	}

	void drawTextureCuboid(float sizeX, float sizeY) {

		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, mTextureID[0]);

		glBegin(GL_TRIANGLES);								// Drawing Using Triangles

		glColor3f(1, 1, 1);

		//front
		//Left Triangle
		//------------------------------------------------
		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(-sizeX, -sizeY, sizeX); //bot left

		glTexCoord2f(0.0f, 1.0f);
		glVertex3f(-sizeX, sizeY, sizeX); //top left

		glTexCoord2f(1.0f, 1.0f);
		glVertex3f(sizeX, sizeY, sizeX); //top right


		//Right Triangle
		glTexCoord2f(1.0f, 0.0f);
		glVertex3f(sizeX, -sizeY, sizeX); // bot right

		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(-sizeX, -sizeY, sizeX); //bot left

		glTexCoord2f(1.0f, 1.0f);
		glVertex3f(sizeX, sizeY, sizeX); // top right
		//--------------------------------------------------

		//right
		//Left Triangle
		//------------------------------------------------
		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(sizeX, -sizeY, sizeX); //bot left

		glTexCoord2f(0.0f, 1.0f);
		glVertex3f(sizeX, sizeY, sizeX); //top left

		glTexCoord2f(1.0f, 1.0f);
		glVertex3f(sizeX, sizeY, -sizeX); //top right

		//Right Triangle
		glTexCoord2f(1.0f, 0.0f);
		glVertex3f(sizeX, -sizeY, -sizeX); // bot right

		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(sizeX, -sizeY, sizeX); //bot left

		glTexCoord2f(1.0f, 1.0f);
		glVertex3f(sizeX, sizeY, -sizeX); // top right
		//-------------------------------------------------

		//left
		//Left Triangle
		//------------------------------------------------
		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(-sizeX, -sizeY, -sizeX); //bot left

		glTexCoord2f(0.0f, 1.0f);
		glVertex3f(-sizeX, sizeY, -sizeX); //top left

		glTexCoord2f(1.0f, 1.0f);
		glVertex3f(-sizeX, sizeY, sizeX); //top right

		//Right Triangle
		glTexCoord2f(1.0f, 0.0f);
		glVertex3f(-sizeX, -sizeY, sizeX); // bot right

		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(-sizeX, -sizeY, -sizeX); //bot left

		glTexCoord2f(1.0f, 1.0f);
		glVertex3f(-sizeX, sizeY, sizeX); // top right
		//-------------------------------------------------

		//back
		//Left Triangle
		//------------------------------------------------
		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(sizeX, -sizeY, -sizeX); //bot left

		glTexCoord2f(0.0f, 1.0f);
		glVertex3f(sizeX, sizeY, -sizeX); //top left

		glTexCoord2f(1.0f, 1.0f);
		glVertex3f(-sizeX, sizeY, -sizeX); //top right

		//Right Triangle
		glTexCoord2f(1.0f, 0.0f);
		glVertex3f(-sizeX, -sizeY, -sizeX); // bot right

		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(sizeX, -sizeY, -sizeX); //bot left

		glTexCoord2f(1.0f, 1.0f);
		glVertex3f(-sizeX, sizeY, -sizeX); // top right

		//-------------------------------------------------

		//top
		//Left Triangle
		//------------------------------------------------
		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(-sizeX, sizeY, sizeX); //bot left

		glTexCoord2f(0.0f, 1.0f);
		glVertex3f(-sizeX, sizeY, -sizeX); //top left

		glTexCoord2f(1.0f, 1.0f);
		glVertex3f(sizeX, sizeY, -sizeX); //top right

		//Right Triangle
		glTexCoord2f(1.0f, 0.0f);
		glVertex3f(sizeX, sizeY, sizeX); // bot right

		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(-sizeX, sizeY, sizeX); //bot left

		glTexCoord2f(1.0f, 1.0f);
		glVertex3f(sizeX, sizeY, -sizeX); // top right

		//-------------------------------------------------

		//bot
		//Left Triangle
		//------------------------------------------------
		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(-sizeX, -sizeY, -sizeX); //bot left

		glTexCoord2f(0.0f, 1.0f);
		glVertex3f(-sizeX, -sizeY, sizeX); //top left

		glTexCoord2f(1.0f, 1.0f);
		glVertex3f(sizeX, -sizeY, sizeX); //top right

		//Right Triangle
		glTexCoord2f(1.0f, 0.0f);
		glVertex3f(sizeX, -sizeY, -sizeX); // bot right

		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(-sizeX, -sizeY, -sizeX); //bot left

		glTexCoord2f(1.0f, 1.0f);
		glVertex3f(sizeX, -sizeY, sizeX); // top right

		//-------------------------------------------------

		glEnd();
		glDisable(GL_TEXTURE_2D);
	}

	void drawPokemon(const Matrix& viewMatrix) {

		m_rotation += 0.1;
		translateX = 0;
		translateZ = -5;

		scaleX = 1;
		scaleY = 1;
		scaleZ = 1;

		rotateX = 0;
		rotateY = 1;
		rotateZ = 0;

		jumpHeight = 5;

		translateY = cos(m_rotation) * jumpHeight;
		if (translateY < 0) {

			translateY = 0;

		}

		//translateY += 0.01;

		if (translateY > 0) {

			bodyRotation += 11;

		}
		else {

			bodyRotation = 0;

		}

		Matrix translateAll = Matrix::makeTranslationMatrix(translateX, translateY, translateZ);
		Matrix rotateAll = Matrix::makeRotateMatrix(bodyRotation, Vector(rotateX, rotateY, rotateZ));
		Matrix scaleAll = Matrix::makeScaleMatrix(scaleX, scaleY, scaleZ);
		Matrix modelMatrixAll = translateAll * rotateAll * scaleAll;

		//= ====================================Body============================================
		glColor3f(0.66f, 0.57f, 0.49f);

		Matrix scaleBody = Matrix::makeScaleMatrix(2.1, 2, 2.1);
		Matrix rotateBody = Matrix::makeRotateMatrix(0, Vector(1, 0, 0));
		Matrix modelMatrixBody = modelMatrixAll * rotateBody * scaleBody;

		Matrix viewSpaceMatrixBody = viewMatrix * modelMatrixBody;
		glLoadMatrixf((GLfloat*)viewSpaceMatrixBody.mVal);

		drawSphere(0, 0, 0, 1.0f, 30);

		//=============================================Hat============================================

		float hatPosY = -0.3f;
		float hatScaleY = 1.111f;

		glColor3f(0.47f, 0.46f, 0.45f);

		Matrix translateHat1 = Matrix::makeTranslationMatrix(0, hatPosY + 0.8, 0);
		Matrix scaleHat1 = Matrix::makeScaleMatrix(hatScaleY * 2.5, hatScaleY * 0.4, hatScaleY * 2.5);
		Matrix modelMatrixHat1 = modelMatrixAll * translateHat1 * scaleHat1;

		Matrix viewSpaceMatrixHat1 = viewMatrix * modelMatrixHat1;
		glLoadMatrixf((GLfloat*)viewSpaceMatrixHat1.mVal);

		drawSphere(0, 0, 0, 1.0f, 30);

		glColor3f(0.0f, 0.0f, 0.0f);

		Matrix translateHatLine1 = Matrix::makeTranslationMatrix(0, hatPosY + 1.1, 0);
		Matrix scaleHatLine1 = Matrix::makeScaleMatrix(hatScaleY * 1.91, hatScaleY * 0.1, hatScaleY * 1.91);
		Matrix modelMatrixHatLine1 = modelMatrixAll * translateHatLine1 * scaleHatLine1;

		Matrix viewSpaceMatrixHatLine1 = viewMatrix * modelMatrixHatLine1;
		glLoadMatrixf((GLfloat*)viewSpaceMatrixHatLine1.mVal);

		drawSphere(0, 0, 0, 1.0f, 30);

		Matrix translateHatLine2 = Matrix::makeTranslationMatrix(0, hatPosY + 1.8, 0);
		Matrix scaleHatLine2 = Matrix::makeScaleMatrix(hatScaleY * 1.63, hatScaleY * 0.1, hatScaleY * 1.63);
		Matrix modelMatrixHatLine2 = modelMatrixAll * translateHatLine2 * scaleHatLine2;

		Matrix viewSpaceMatrixHatLine2 = viewMatrix * modelMatrixHatLine2;
		glLoadMatrixf((GLfloat*)viewSpaceMatrixHatLine2.mVal);

		drawSphere(0, 0, 0, 1.0f, 30);

		Matrix translateHatLine3 = Matrix::makeTranslationMatrix(0, hatPosY + 2.4, 0);
		Matrix scaleHatLine3 = Matrix::makeScaleMatrix(hatScaleY * 1.07, hatScaleY * 0.1, hatScaleY * 1.07);
		Matrix modelMatrixHatLine3 = modelMatrixAll * translateHatLine3 * scaleHatLine3;

		Matrix viewSpaceMatrixHatLine3 = viewMatrix * modelMatrixHatLine3;
		glLoadMatrixf((GLfloat*)viewSpaceMatrixHatLine3.mVal);

		drawSphere(0, 0, 0, 1.0f, 30);

		glColor3f(0.47f, 0.46f, 0.45f);

		Matrix translateHat3 = Matrix::makeTranslationMatrix(0, hatPosY + 0.75, 0);
		Matrix scaleHat3 = Matrix::makeScaleMatrix(hatScaleY * 1.9, hatScaleY * 1.8, hatScaleY * 1.9);
		Matrix modelMatrixHat3 = modelMatrixAll * translateHat3 * scaleHat3;

		Matrix viewSpaceMatrixHat3 = viewMatrix * modelMatrixHat3;
		glLoadMatrixf((GLfloat*)viewSpaceMatrixHat3.mVal);

		drawSphere(0, 0, 0, 1.0f, 30);

		Matrix translateHatStem1 = Matrix::makeTranslationMatrix(-0.1, 2, 0);
		Matrix scaleHatStem1 = Matrix::makeScaleMatrix(0.3, 2, 0.3);
		Matrix rotateHatStem1 = Matrix::makeRotateMatrix(180, Vector(1.0f, 0.0f, 0.0f));
		Matrix modelMatrixHatStem1 = modelMatrixAll * translateHatStem1 * scaleHatStem1 * rotateHatStem1;

		Matrix viewSpaceMatrixHatStem1 = viewMatrix * modelMatrixHatStem1;

		glLoadMatrixf((GLfloat*)viewSpaceMatrixHatStem1.mVal);

		drawPyramid();

		//====================================================Tail=============================================================

		glColor3f(0.66f, 0.57f, 0.49f);

		Matrix translateTail1 = Matrix::makeTranslationMatrix(0, -1.4, 0);
		Matrix scaleTail1 = Matrix::makeScaleMatrix(1, 1, 1);
		Matrix rotateTail1 = Matrix::makeRotateMatrix(180, Vector(1.0f, 0.0f, 0.0f));
		Matrix modelMatrixTail1 = modelMatrixAll * translateTail1 * scaleTail1 * rotateTail1;

		Matrix viewSpaceMatrixTail1 = viewMatrix * modelMatrixTail1;

		glLoadMatrixf((GLfloat*)viewSpaceMatrixTail1.mVal);

		drawCone(0, 0, 0, 1.0f, 30, 1);

		//====================================================Feet=============================================================

		if (translateY > 0) {

			jumpFeetAngle = -25;

		}
		else {

			jumpFeetAngle = 0;

		}

		Matrix jumpFeet = Matrix::makeRotateMatrix(jumpFeetAngle, Vector(1.0f, 0.0f, 0.0f));


		glColor3f(0.95f, 0.89f, 0.73f);

		Matrix translateFeetLeft1 = Matrix::makeTranslationMatrix(-0.8, -2.1, 0.5);
		Matrix scaleFeetLeft1 = Matrix::makeScaleMatrix(0.6, 0.5, 1.1);
		Matrix rotateFeetLeft1 = Matrix::makeRotateMatrix(90, Vector(0.0f, 1.0f, 0.0f));

		Matrix modelMatrixFeetLeft1 = modelMatrixAll * translateFeetLeft1 * jumpFeet * scaleFeetLeft1 * rotateFeetLeft1;

		Matrix viewSpaceMatrixFeetLeft1 = viewMatrix * modelMatrixFeetLeft1;

		glLoadMatrixf((GLfloat*)viewSpaceMatrixFeetLeft1.mVal);

		drawSphere(0, 0, 0, 1.0f, 30);

		Matrix translateFeetLeftBottom1 = Matrix::makeTranslationMatrix(-0.8, -2.3, 0.5);
		Matrix scaleFeetLeftBottom1 = Matrix::makeScaleMatrix(0.6, 0.2, 1.1);
		Matrix rotateFeetLeftBottom1 = Matrix::makeRotateMatrix(90, Vector(0.0f, 1.0f, 0.0f));
		Matrix modelMatrixFeetLeftBottom1 = modelMatrixAll * translateFeetLeftBottom1 * jumpFeet * scaleFeetLeftBottom1;

		Matrix viewSpaceMatrixFeetLeftBottom1 = viewMatrix * modelMatrixFeetLeftBottom1;

		glLoadMatrixf((GLfloat*)viewSpaceMatrixFeetLeftBottom1.mVal);

		drawCylinder(0, 0, 1, 1, 30);

		glColor3f(0.95f, 0.89f, 0.73f);

		Matrix translateFeetRight1 = Matrix::makeTranslationMatrix(0.8, -2.1, 0.5);
		Matrix scaleFeetRight1 = Matrix::makeScaleMatrix(0.6, 0.5, 1.1);
		Matrix rotateFeetRight1 = Matrix::makeRotateMatrix(90, Vector(0.0f, 1.0f, 0.0f));
		Matrix modelMatrixFeetRight1 = modelMatrixAll * translateFeetRight1 * jumpFeet * scaleFeetRight1 * rotateFeetRight1;

		Matrix viewSpaceMatrixFeetRight1 = viewMatrix * modelMatrixFeetRight1;

		glLoadMatrixf((GLfloat*)viewSpaceMatrixFeetRight1.mVal);

		drawSphere(0, 0, 0, 1.0f, 30);

		Matrix translateFeetRightBottom1 = Matrix::makeTranslationMatrix(0.8, -2.3, 0.5);
		Matrix scaleFeetRightBottom1 = Matrix::makeScaleMatrix(0.6, 0.2, 1.1);
		Matrix rotateFeetRightBottom1 = Matrix::makeRotateMatrix(90, Vector(0.0f, 1.0f, 0.0f));
		Matrix modelMatrixFeetRightBottom1 = modelMatrixAll * translateFeetRightBottom1 * jumpFeet * scaleFeetRightBottom1;

		Matrix viewSpaceMatrixFeetRightBottom1 = viewMatrix * modelMatrixFeetRightBottom1;

		glLoadMatrixf((GLfloat*)viewSpaceMatrixFeetRightBottom1.mVal);

		drawCylinder(0, 0, 1, 1, 30);

		//====================================================Eye=============================================================

		glColor3f(0.95f, 0.89f, 0.73f);

		Matrix translateEyeLeft1 = Matrix::makeTranslationMatrix(-0.73, 0.05, 1.81);
		Matrix scaleEyeLeft1 = Matrix::makeScaleMatrix(0.9, 1.0, 0.1);
		Matrix rotateEyeLeft1 = Matrix::makeRotateMatrix(20, Vector(0.0f, 1.0f, 0.0f));
		Matrix modelMatrixEyeLeft1 = modelMatrixAll * translateEyeLeft1 * rotateEyeLeft1 * scaleEyeLeft1;

		Matrix viewSpaceMatrixEyeLeft1 = viewMatrix * modelMatrixEyeLeft1;

		glLoadMatrixf((GLfloat*)viewSpaceMatrixEyeLeft1.mVal);

		drawSphere(0, 0, 0, 1.0f, 30);

		glColor3f(0.0f, 0.0f, 0.0f);

		Matrix translateEyeBlackLeft1 = Matrix::makeTranslationMatrix(-0.73, 0, 1.91);
		Matrix scaleEyeBlackLeft1 = Matrix::makeScaleMatrix(0.615, 0.6, 0.05);
		Matrix rotateEyeBlackLeft1 = Matrix::makeRotateMatrix(20, Vector(0.0f, 1.0f, 0.0f));
		Matrix modelMatrixEyeBlackLeft1 = modelMatrixAll * translateEyeBlackLeft1 * rotateEyeBlackLeft1 * scaleEyeBlackLeft1;

		Matrix viewSpaceMatrixEyeBlackLeft1 = viewMatrix * modelMatrixEyeBlackLeft1;

		glLoadMatrixf((GLfloat*)viewSpaceMatrixEyeBlackLeft1.mVal);

		drawSphere(0, 0, 0, 1.0f, 30);

		glColor3f(0.95f, 0.89f, 0.73f);

		Matrix translateEyePupilLeft1 = Matrix::makeTranslationMatrix(-0.73, 0.0, 1.92);
		Matrix scaleEyePupilLeft1 = Matrix::makeScaleMatrix(0.5, 0.5, 0.05);
		Matrix rotateEyePupilLeft1 = Matrix::makeRotateMatrix(20, Vector(0.0f, 1.0f, 0.0f));
		Matrix modelMatrixEyePupilLeft1 = modelMatrixAll * translateEyePupilLeft1 * rotateEyePupilLeft1 * scaleEyePupilLeft1;

		Matrix viewSpaceMatrixEyePupilLeft1 = viewMatrix * modelMatrixEyePupilLeft1;

		glLoadMatrixf((GLfloat*)viewSpaceMatrixEyePupilLeft1.mVal);

		drawSphere(0, 0, 0, 1.0f, 30);

		glColor3f(0.95f, 0.89f, 0.73f);

		Matrix translateEyeRight1 = Matrix::makeTranslationMatrix(0.73, 0, 1.81);
		Matrix scaleEyeRight1 = Matrix::makeScaleMatrix(0.9, 1.0, 0.1);
		Matrix rotateEyeRight1 = Matrix::makeRotateMatrix(-20, Vector(0.0f, 1.0f, 0.0f));
		Matrix modelMatrixEyeRight1 = modelMatrixAll * translateEyeRight1 * rotateEyeRight1 * scaleEyeRight1;

		Matrix viewSpaceMatrixEyeRight1 = viewMatrix * modelMatrixEyeRight1;

		glLoadMatrixf((GLfloat*)viewSpaceMatrixEyeRight1.mVal);

		drawSphere(0, 0, 0, 1.0f, 30);

		glColor3f(0.0f, 0.0f, 0.0f);

		Matrix translateEyeBlackRight1 = Matrix::makeTranslationMatrix(0.73, 0, 1.91);
		Matrix scaleEyeBlackRight1 = Matrix::makeScaleMatrix(0.615, 0.6, 0.05);
		Matrix rotateEyeBlackRight1 = Matrix::makeRotateMatrix(-20, Vector(0.0f, 1.0f, 0.0f));
		Matrix modelMatrixEyeBlackRight1 = modelMatrixAll * translateEyeBlackRight1 * rotateEyeBlackRight1 * scaleEyeBlackRight1;

		Matrix viewSpaceMatrixEyeBlackRight1 = viewMatrix * modelMatrixEyeBlackRight1;

		glLoadMatrixf((GLfloat*)viewSpaceMatrixEyeBlackRight1.mVal);

		drawSphere(0, 0, 0, 1.0f, 30);

		glColor3f(0.95f, 0.89f, 0.73f);

		Matrix translateEyePupilRight1 = Matrix::makeTranslationMatrix(0.73, 0.0, 1.92);
		Matrix scaleEyePupilRight1 = Matrix::makeScaleMatrix(0.5, 0.5, 0.05);
		Matrix rotateEyePupilRight1 = Matrix::makeRotateMatrix(-20, Vector(0.0f, 1.0f, 0.0f));
		Matrix modelMatrixEyePupilRight1 = modelMatrixAll * translateEyePupilRight1 * rotateEyePupilRight1 * scaleEyePupilRight1;

		Matrix viewSpaceMatrixEyePupilRight1 = viewMatrix * modelMatrixEyePupilRight1;

		glLoadMatrixf((GLfloat*)viewSpaceMatrixEyePupilRight1.mVal);

		drawSphere(0, 0, 0, 1.0f, 30);

	}

	void drawGrid(float newy) {

		float size = 1.0f;
		float y = 0.0f;
		int curSquare = -1;

		for (int u = 0; u < RECT_VERTICE_H; u++)
		{
			moveY = (2 * size) * u;

			for (int i = 0; i < RECT_VERTICE_W; i++)
			{
				curSquare++;
				turn = curSquare * 18;

				moveX = (2 * size) * i;

				vertices[turn] = -size + moveX;
				vertices[turn + 1] = 0;
				vertices[turn + 2] = size + moveY;

				vertices[turn + 3] = size + moveX;
				vertices[turn + 4] = newy;
				vertices[turn + 5] = size + moveY;

				vertices[turn + 6] = size + moveX;
				vertices[turn + 7] = newy;
				vertices[turn + 8] = -size + moveY;

				vertices[turn + 9] = size + moveX;
				vertices[turn + 10] = newy; //<<
				vertices[turn + 11] = -size + moveY;

				vertices[turn + 12] = -size + moveX;
				vertices[turn + 13] = 0;
				vertices[turn + 14] = -size + moveY;

				vertices[turn + 15] = -size + moveX;
				vertices[turn + 16] = 0;
				vertices[turn + 17] = size + moveY;

			}

		}

		glEnableClientState(GL_VERTEX_ARRAY);
		glVertexPointer(3, GL_FLOAT, 0, vertices);
		glDrawArrays(GL_TRIANGLES, 0, number_of_square * 6);
		glDisableClientState(GL_VERTEX_ARRAY);

	}

	float Clamp(float x, float min, float max) {

		float result;

		result = x;

		if (x <= min) {

			result = min;

		}
		else if (x >= max) {

			result = max;

		}

		return result;

	}

	float sClamp(float x, float max) {

		float result;

		result = x / max;

		if (result <= 0) {

			result = 0.00f;

		}
		else if (result >= 1.00f) {

			result = max;

		}

		return result;

	}

	void drawSquareWithVertex(float yPlacement, GLfloat pointA, GLfloat pointB, GLfloat leftPointY, GLfloat rightPointY, float zDistance) {

		static GLfloat vSquare[6];
		static GLfloat vTex[12] = {

			0.0f, 0.0f,
			1.0f, 0.0f,
			1.0f, 1.0f,
			1.0f, 1.0f,
			0.0f, 1.0f,
			0.0f, 0.0f,

		};

		float sizeY = 1.0f;

		for (int i = 0; i < 1; i++)
		{

			//Right Triangle
			//Left Bot
			vSquare[i] = pointA;
			vSquare[i + 1] = yPlacement + -sizeY + leftPointY;
			vSquare[i + 2] = zDistance;

			//RIght Bot
			vSquare[i + 3] = pointB;
			vSquare[i + 4] = yPlacement + -sizeY + rightPointY;
			vSquare[i + 5] = zDistance;

			//Right Top
			vSquare[i + 6] = pointB;
			vSquare[i + 7] = yPlacement + sizeY + rightPointY;
			vSquare[i + 8] = zDistance;


			//LeftTriangle
			//right top
			vSquare[i + 9] = pointB;
			vSquare[i + 10] = yPlacement + sizeY + rightPointY;
			vSquare[i + 11] = zDistance;

			//left top
			vSquare[i + 12] = pointA;
			vSquare[i + 13] = yPlacement + sizeY + leftPointY;
			vSquare[i + 14] = zDistance;

			//left bot
			vSquare[i + 15] = pointA;
			vSquare[i + 16] = yPlacement + -sizeY + leftPointY;
			vSquare[i + 17] = zDistance;

		}

		glEnable(GL_TEXTURE_2D);
		glEnable(GL_BLEND);

		glEnableClientState(GL_VERTEX_ARRAY);
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);

		glBlendFunc(GL_SRC_ALPHA, 0.0f);

		glBindTexture(GL_TEXTURE_2D, mTextureID[songNum]);
		glTexCoordPointer(2, GL_FLOAT, 0, vTex);

		glVertexPointer(3, GL_FLOAT, 0, vSquare);
		glDrawArrays(GL_TRIANGLES, 0, 6);

		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
		glDisableClientState(GL_VERTEX_ARRAY);

		glDisable(GL_TEXTURE_2D);
		glDisable(GL_BLEND);

	}

	void drawMusicLightning(float yDistance, float zDistance) {

		const int lightningNum = 40;

		float amp = 40.0f;
		float curAmp = 0;
		float height = 30.0f;

		float lightningHeightRight[lightningNum / 2 + 1];
		float lightningHeightLeft[lightningNum / 2 + 1];

		GLfloat length = 2.0f;
		GLfloat pointY[lightningNum + 1];

		int rightPlacer = 0;
		int leftPlacer = 0;

		if (songNum == 1) {

			amp = 40.0f;

		}
		else if (songNum == 2) {

			amp = 100.0f;

		}

		if (curAmp <= 0) {

				curAmp = 0;

		}

		for (int u = 0; u < lightningNum; u++)
		{

			if (u == 0 || u == 1) {

				if (songNum == 2) {
					curAmp = amp + 50;
				}
				else {

					curAmp = amp;;

				}


			}
			else {

				curAmp = amp;

			}

			if (u % 2 == 0) {


				lightningHeightRight[rightPlacer] = Clamp(((m_spectrumLeft[u] + m_spectrumRight[u]) / 2) * curAmp, -curAmp, 20);
				rightPlacer += 1;

			}
			else {

				lightningHeightLeft[leftPlacer] = Clamp(((m_spectrumLeft[u] + m_spectrumRight[u]) / 2) * curAmp, -curAmp, 20);
				leftPlacer += 1;

			}
		}

		float curHeightLeft;
		float curHeightRight;

		float curHeightLeftNext;
		float curHeightRightNext;

		for (int i = 0; i < lightningNum / 2; i++)
		{

			float origin = lightningHeightRight[0];

			curHeightLeft = lightningHeightLeft[i];
			curHeightLeftNext = lightningHeightLeft[i + 1];

			curHeightRight = lightningHeightRight[i];
			curHeightRightNext = lightningHeightRight[i + 1];

			if (i == 0) {

				curHeightRight = lightningHeightRight[0];
				curHeightLeft = lightningHeightRight[0];

			}

			drawSquareWithVertex(yDistance, (i)*length, (i + 1) * length, curHeightRight, curHeightRightNext, zDistance);
			drawSquareWithVertex(yDistance, (i) * -length, (i + 1) * -length, curHeightLeft, curHeightLeftNext, zDistance);
		}
	}

	float GetColor(float value) {

		return value / 255;

	}

	float lerpToColor(float curColor, float targetColor, float difference) {


		float result = 0;
		float curDifference = targetColor - curColor;

		if (curColor > targetColor) {

			result -= difference / curDifference;

		}
		else {

			result += difference / curDifference;

		}

		return Clamp(result, curColor, targetColor);

	}

	void drawBars(float yDistance, float zDistance) {

		const int bar_num = 25;

		static GLfloat vBars[bar_num * 18] = {};
		static GLfloat barsColors[bar_num * 18] = {};

		float moveX;
		float bars[256];

		int left = 0;
		int right = 0;
		float leftRight;
		float distance = 2.0f;
		float height = 15;
		float baseHeight = 2;
		float amp;
		float leftrightAmp;

		float curBarHeight;

		float colorBotR = 1.0f;
		float colorBotG = 1.0f;
		float colorBotB = 1.0f;

		float colorTopR = 1.0f;
		float colorTopG = 1.0f;
		float colorTopB = 1.0f;

		float gradR = 0.0f;
		float gradG = 0.0f;
		float gradB = 0.0f;


		if (songNum == 0) {

			amp = 55;
			leftrightAmp = 65;

			colorBotR = GetColor(128);
			colorBotG = GetColor(28);
			colorBotB = GetColor(242);

			colorTopR = GetColor(201);
			colorTopG = GetColor(185);
			colorTopB = GetColor(255);

			gradR = 1.0f;


		}
		else if (songNum == 1) {

			amp = 55;
			leftrightAmp = 65;

			colorBotR = GetColor(0);
			colorBotG = GetColor(210);
			colorBotB = GetColor(25);

			colorTopR = GetColor(148);
			colorTopG = GetColor(195);
			colorTopB = GetColor(153);

			gradR = 1.0f;


		}
		else if (songNum == 2) {

			amp = 155;
			leftrightAmp = 140;

			colorBotR = 0.2f;
			colorBotG = 0.2f;
			colorBotB = 1.0f;

			colorTopR = 0.65f;
			colorTopG = 1.0f;
			colorTopB = 1.0f;

			gradR = 1.0f;

		}
		else if (songNum == 3) {

			amp = 55;
			leftrightAmp = 65;

			colorBotR = GetColor(220);
			colorBotG = GetColor(35);
			colorBotB = GetColor(35);

			colorTopR = GetColor(224);
			colorTopG = GetColor(173);
			colorTopB = GetColor(199);

			gradR = 1.0f;

		}
		else {

			amp = 25;
			leftrightAmp = 30;

		}

		for (int u = 0; u < bar_num; u++)
		{

			if (u % 2 == 0) {


				leftRight = -1.11f * left;
				left += 1;

			}
			else {

				right += 1;
				leftRight = 1.11f * right;


			}

			if (u == 0) {

				leftRight = 0;

			}

			if (u == 0 || u == 1) {

				curBarHeight = Clamp(((m_spectrumLeft[u] + m_spectrumRight[u]) / 2) * amp, 0, height);

			}
			else {

				curBarHeight = Clamp(((m_spectrumLeft[u] + m_spectrumRight[u]) / 2) * leftrightAmp, 0, height);

			}

			bars[u] = curBarHeight;

			if (Clamp(((m_spectrumLeft[0] + m_spectrumRight[0]) / 2) * amp, 0, height) > height) {

				cout << Clamp(((m_spectrumLeft[0] + m_spectrumRight[0]) / 2) * amp, 0, height) << endl;

			}
			moveX = distance * leftRight;

			//Draw Bars
			for (int i = 0; i < 6; i++)
			{

				//1
				vBars[turn] = -1.0f + moveX;
				vBars[turn + 1] = yDistance + -1.0f;
				vBars[turn + 2] = zDistance;

				//2
				vBars[turn + 3] = 1.0f + moveX;
				vBars[turn + 4] = yDistance + -1.0f;
				vBars[turn + 5] = zDistance;

				//3
				vBars[turn + 6] = 1.0f + moveX;
				vBars[turn + 7] = yDistance + (baseHeight)+bars[u];
				vBars[turn + 8] = zDistance;

				//4
				vBars[turn + 9] = 1.0f + moveX;
				vBars[turn + 10] = yDistance + (baseHeight)+bars[u]; //
				vBars[turn + 11] = zDistance;

				//5
				vBars[turn + 12] = -1.0f + moveX;
				vBars[turn + 13] = yDistance + (baseHeight)+bars[u];//
				vBars[turn + 14] = zDistance;

				//6
				vBars[turn + 15] = -1.0f + moveX;
				vBars[turn + 16] = yDistance + -1.0f;
				vBars[turn + 17] = zDistance;

			}

			for (int i = 0; i < 6; i++)
			{
				//Right Triangle
				//bot left
				barsColors[turn] = colorBotR * 1.0f;
				barsColors[turn + 1] = colorBotG * 1.0f;
				barsColors[turn + 2] = colorBotB * 1.0f;

				//bot right
				barsColors[turn + 3] = colorBotR * 1.0f;
				barsColors[turn + 4] = colorBotG * 1.0f;
				barsColors[turn + 5] = colorBotB * 1.0f;


				//top right
				barsColors[turn + 6] = colorTopR;
				barsColors[turn + 7] = colorTopG;
				barsColors[turn + 8] = colorTopB;

				//Left Triangle
				//Top right
				barsColors[turn + 9] = colorTopR;
				barsColors[turn + 10] = colorTopG;
				barsColors[turn + 11] = colorTopB;

				//Top left
				barsColors[turn + 12] = colorTopR;
				barsColors[turn + 13] = colorTopG;
				barsColors[turn + 14] = colorTopB;


				//Bot Right
				barsColors[turn + 15] = colorBotR * 1.0f;
				barsColors[turn + 16] = colorBotG * 1.0f;
				barsColors[turn + 17] = colorBotB * 1.0f;

			}


			turn = u * 18;

		}

		glEnableClientState(GL_VERTEX_ARRAY);
		glEnableClientState(GL_COLOR_ARRAY);

		glVertexPointer(3, GL_FLOAT, 0, vBars);


		glColorPointer(3, GL_FLOAT, 0, barsColors);

		glDrawArrays(GL_TRIANGLES, 0, bar_num * 6);

		glDisableClientState(GL_VERTEX_ARRAY);
		glDisableClientState(GL_COLOR_ARRAY);
	}

	void blend(const Matrix& viewMatrix) {


		static GLfloat vertices1[] = {

		-1.0f, -1.0f, 2.0f,
		1.0f, -1.0f, 2.0f,
		1.0f,  1.0f,  2.0f,
		1.0f,  1.0f,  2.0f,
		-1.0f,  1.0f, 2.0f,
		-1.0f, -1.0f, 2.0f,

		};

		static GLfloat textCoords1[] = {

			0.0f, 0.0f,
			1.0f, 0.0f,
			1.0f, 1.0f,
			1.0f, 1.0f,
			0.0f, 1.0f,
			0.0f, 0.0f,

		};

		static GLfloat verticesCube[] = {

			// front
			-1.0f, -1.0f, 1.0f,
			1.0f, -1.0f, 1.0f,
			1.0f, 1.0f, 1.0f,
			1.0f, 1.0f, 1.0f,
			-1.0f, 1.0f, 1.0f,
			-1.0f, -1.0f, 1.0f,


			// right
			 1.0f, -1.0f,  1.0f,
			 1.0f, -1.0f, -1.0f,
			 1.0f,  1.0f, -1.0f,
			 1.0f,  1.0f, -1.0f,
			 1.0f,  1.0f,  1.0f,
			 1.0f, -1.0f,  1.0f,

			 // back
			  1.0f, -1.0f, -1.0f,
			 -1.0f, -1.0f, -1.0f,
			 -1.0f,  1.0f, -1.0f,
			 -1.0f,  1.0f, -1.0f,
			  1.0f,  1.0f, -1.0f,
			  1.0f, -1.0f, -1.0f,

			  // left
			  -1.0f, -1.0f, -1.0f,
			  -1.0f, -1.0f,  1.0f,
			  -1.0f,  1.0f,  1.0f,
			  -1.0f,  1.0f,  1.0f,
			  -1.0f,  1.0f, -1.0f,
			  -1.0f, -1.0f, -1.0f,

			  // top
			  -1.0f,  1.0f,  1.0f,
			   1.0f,  1.0f,  1.0f,
			   1.0f,  1.0f, -1.0f,
			   1.0f,  1.0f, -1.0f,
			  -1.0f,  1.0f, -1.0f,
			  -1.0f,  1.0f,  1.0f,

			  // bottom
			  -1.0f, -1.0f,  1.0f,
			   1.0f, -1.0f,  1.0f,
			   1.0f, -1.0f, -1.0f,
			   1.0f, -1.0f, -1.0f,
			  -1.0f, -1.0f, -1.0f,
			  -1.0f, -1.0f,  1.0f,

		};

		static GLfloat textCoordsCube[] = {

			0.0f, 0.0f,
			1.0f, 0.0f,
			1.0f, 1.0f,
			1.0f, 1.0f,
			0.0f, 1.0f,
			0.0f, 0.0f,

			0.0f, 0.0f,
			1.0f, 0.0f,
			1.0f, 1.0f,
			1.0f, 1.0f,
			0.0f, 1.0f,
			0.0f, 0.0f,

			0.0f, 0.0f,
			1.0f, 0.0f,
			1.0f, 1.0f,
			1.0f, 1.0f,
			0.0f, 1.0f,
			0.0f, 0.0f,

			0.0f, 0.0f,
			1.0f, 0.0f,
			1.0f, 1.0f,
			1.0f, 1.0f,
			0.0f, 1.0f,
			0.0f, 0.0f,

			0.0f, 0.0f,
			1.0f, 0.0f,
			1.0f, 1.0f,
			1.0f, 1.0f,
			0.0f, 1.0f,
			0.0f, 0.0f,

			0.0f, 0.0f,
			1.0f, 0.0f,
			1.0f, 1.0f,
			1.0f, 1.0f,
			0.0f, 1.0f,
			0.0f, 0.0f,

		};

		static GLfloat colorsCube[] = {

			1.0f, 1.0f, 1.0f,
			1.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f,
			0.0f, 1.0f, 0.0f,
			1.0f, 0.0f, 0.0f,
			1.0f, 1.0f, 1.0f,

			1.0f, 1.0f, 1.0f,
			1.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f,
			0.0f, 1.0f, 0.0f,
			1.0f, 0.0f, 0.0f,
			1.0f, 1.0f, 1.0f,

			1.0f, 1.0f, 1.0f,
			1.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f,
			0.0f, 1.0f, 0.0f,
			1.0f, 0.0f, 0.0f,
			1.0f, 1.0f, 1.0f,

			1.0f, 1.0f, 1.0f,
			1.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f,
			0.0f, 1.0f, 0.0f,
			1.0f, 0.0f, 0.0f,
			1.0f, 1.0f, 1.0f,

			1.0f, 1.0f, 1.0f,
			1.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f,
			0.0f, 1.0f, 0.0f,
			1.0f, 0.0f, 0.0f,
			1.0f, 1.0f, 1.0f,

			1.0f, 1.0f, 1.0f,
			1.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f,
			0.0f, 1.0f, 0.0f,
			1.0f, 0.0f, 0.0f,
			1.0f, 1.0f, 1.0f,

		};


		static GLfloat colors1[] = {

			1.0f, 1.0f, 1.0f,
			1.0f, 1.0f, 1.0f,
			1.0f, 1.0f, 1.0f,
			1.0f, 1.0f, 1.0f,
			1.0f, 1.0f, 1.0f,
			1.0f, 1.0f, 1.0f,

		};

		static GLfloat vertices2[] = {

			  1.0f, -1.0f, -2.0f,
			-1.0f, -1.0f, -2.0f,
			-1.0f,  1.0f, -2.0f,
			-1.0f,  1.0f, -2.0f,
			1.0f,  1.0f, -2.0f,
			1.0f, -1.0f, -2.0f,

		};

		static GLfloat textCoords2[] = {

			0.0f, 0.0f,
			1.0f, 0.0f,
			1.0f, 1.0f,
			1.0f, 1.0f,
			0.0f, 1.0f,
			0.0f, 0.0f,

		};

		static GLfloat colors2[] = {

			1.0f, 1.0f, 1.0f,
			1.0f, 1.0f, 1.0f,
			1.0f, 1.0f, 1.0f,
			1.0f, 1.0f, 1.0f,
			1.0f, 1.0f, 1.0f,
			1.0f, 1.0f, 1.0f,

		};
		glDepthMask(false);
		glLoadMatrixf((GLfloat*)viewMatrix.mVal);

		glEnable(GL_TEXTURE_2D);

		glEnableClientState(GL_VERTEX_ARRAY);
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		glEnableClientState(GL_COLOR_ARRAY);

		glEnable(GL_BLEND);

		//===================================================================CUBE===================================================================
		glEnable(GL_BLEND);
		//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); //alpha
		glBlendFunc(GL_SRC_ALPHA, GL_ONE); //add
		//glBlendFunc(GL_SRC_ALPHA, GL_ZERO); //multiply

		glBindTexture(GL_TEXTURE_2D, mTextureID[0]);

		glVertexPointer(3, GL_FLOAT, 0, verticesCube);
		glTexCoordPointer(2, GL_FLOAT, 0, textCoordsCube);
		glColorPointer(3, GL_FLOAT, 0, colorsCube);

		glDrawArrays(GL_TRIANGLES, 0, 36);
		glDisable(GL_BLEND);

		//===================================================================REct1===================================================================
		//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); //alpha
		//glBlendFunc(GL_SRC_ALPHA, GL_ONE); //add
		//glBlendFunc(GL_SRC_ALPHA, GL_ZERO); //multiply


		glBindTexture(GL_TEXTURE_2D, mTextureID[2]);

		glVertexPointer(3, GL_FLOAT, 0, vertices1);
		glTexCoordPointer(2, GL_FLOAT, 0, textCoords1);
		glColorPointer(3, GL_FLOAT, 0, colors1);

		glDrawArrays(GL_TRIANGLES, 0, 6);
		glDisable(GL_BLEND);


		//===================================================================REct2===================================================================

		//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); //alpha
		//glBlendFunc(GL_SRC_ALPHA, GL_ONE); //add
		//glBlendFunc(GL_SRC_ALPHA, GL_ZERO); //multiply

		glBindTexture(GL_TEXTURE_2D, mTextureID[1]);

		glVertexPointer(3, GL_FLOAT, 0, vertices2);
		glTexCoordPointer(2, GL_FLOAT, 0, textCoords2);
		glColorPointer(3, GL_FLOAT, 0, colors2);

		glDrawArrays(GL_TRIANGLES, 0, 6);
		glDisable(GL_BLEND);

		glDisableClientState(GL_COLOR_ARRAY);
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
		glDisableClientState(GL_VERTEX_ARRAY);

		glDisable(GL_TEXTURE_2D);
		glDisable(GL_BLEND);

	}

	void drawLightning(GLfloat pointA, GLfloat pointB) {

		GLfloat length = 2.0f;
		GLfloat pointY[11];

		//cache last Y
		for (int i = 0; i < 10; i++)
		{
			GLfloat randY = rand() % 3;
			pointY[i] = randY;
			pointY[i + 1] = randY;
		}

		GLfloat randY = rand() % 3;

		pointY[10] = randY;

		for (int i = 0; i < 10; i++)
		{

			drawSquareWithVertex(-10, (i)*length, (i + 1) * length, pointY[i], pointY[i + 1], -15);

		}
	}

	float findMidPoint(GLfloat pointA, GLfloat pointB) {

		return (pointB - pointA) / 2;

	}

	void drawBackGround(float x, float y, float z, float sizeX, float sizeY, int textureID) {

		static GLfloat verticesBackground[18];
		static GLfloat textCoordsBackground[12] = {

			0.0f, 0.0f,
			1.0f, 0.0f,
			1.0f, 1.0f,
			1.0f, 1.0f,
			0.0f, 1.0f,
			0.0f, 0.0f,

		};

		for (int i = 0; i < 1; i++)
		{

			//Right Triangle
			//Left Bot
			verticesBackground[i] = x - sizeX;
			verticesBackground[i + 1] = y - sizeY;
			verticesBackground[i + 2] = z;

			//RIght Bot
			verticesBackground[i + 3] = x + sizeX;
			verticesBackground[i + 4] = y - sizeY;
			verticesBackground[i + 5] = z;

			//Right Top
			verticesBackground[i + 6] = x + sizeX;
			verticesBackground[i + 7] = y + sizeY;
			verticesBackground[i + 8] = z;


			//LeftTriangle
			//right top
			verticesBackground[i + 9] = x + sizeX;
			verticesBackground[i + 10] = y + sizeY;
			verticesBackground[i + 11] = z;

			//left top
			verticesBackground[i + 12] = x - sizeX;
			verticesBackground[i + 13] = y + sizeY;
			verticesBackground[i + 14] = z;

			//left bot
			verticesBackground[i + 15] = x - sizeX;
			verticesBackground[i + 16] = y - sizeY;
			verticesBackground[i + 17] = z;

		}

		glEnable(GL_TEXTURE_2D);
		glEnable(GL_BLEND);

		glEnableClientState(GL_VERTEX_ARRAY);
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);

		//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); //alpha
		glBlendFunc(GL_SRC_ALPHA, 1.0f); //add
		//glBlendFunc(GL_SRC_ALPHA, GL_ZERO); //multiply

		glBindTexture(GL_TEXTURE_2D, mTextureID[textureID]);
		glTexCoordPointer(2, GL_FLOAT, 0, textCoordsBackground);

		glVertexPointer(3, GL_FLOAT, 0, verticesBackground);
		glDrawArrays(GL_TRIANGLES, 0, 18);

		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
		glDisableClientState(GL_VERTEX_ARRAY);

		glDisable(GL_TEXTURE_2D);
		glDisable(GL_BLEND);

	}

	void drawDeco(float x, float y, float z, float sizeX, float sizeY, float sizeZ, float rotateX, float rotateY, float rotateZ, int textureID, const Matrix& viewMatrix) {

		 GLfloat verticesDeco[] = {

		//Front
			-1.0f,
			-1.0f,
			 1.0f,

			1.0f,
			-1.0f,
			1.0f,

			0.0f,
			1.0f,
			0.0f,

		//right
			1.0f,
			-1.0f,
			1.0f,

			1.0f,
			-1.0f,
			-1.0f,

			0.0f,
			1.0f,
			0.0f,

		//Back
			-1.0f,
			-1.0f,
			-1.0f,

			1.0f,
			-1.0f,
			-1.0f,

			0.0f,
			1.0f,
			0.0f,

		//left
			-1.0f,
			-1.0f,
			1.0f,

			-1.0f,
			-1.0f,
			-1.0f,

			0.0f,
			1.0f,
			0.0f,

		//====

		//Front
			-1.0f,
			-1.0f,
			1.0f,

			1.0f,
			-1.0f,
			1.0f,

			0.0f,
			-2.0f,
			0.0f,

		//right
			1.0f,
			-1.0f ,
			1.0f,

			1.0f,
			-1.0f,
			-1.0f,

			0.0f,
			-2.0f,
			0.0f,

		//Back
			-1.0f,
			-1.0f,
			-1.0f,

			 1.0f,
			-1.0f,
			-1.0f,

			 0.0f,
			-2.0f,
			 0.0f,

		//left
			-1.0f,
			-1.0f,
			 1.0f,

			-1.0f,
			-1.0f,
			-1.0f,

			 0.0f,
			-2.0f,
			 0.0f,

	};

		GLfloat texxtCoordsDeco[] = {

		0.0f, 0.0f,
		1.0f, 0.0f,
		1.0f, 1.0f,
		1.0f, 1.0f,
		0.0f, 1.0f,
		0.0f, 0.0f,
	};

		floatRate += 0.01;

		Matrix rotate = Matrix::makeRotateMatrix(rotateX, Vector(1.0f, 0.0f, 0.0f));
		Matrix rotate1 = Matrix::makeRotateMatrix(rotateY, Vector(0.0f, 1.0f, 0.0f));
		Matrix rotate2 = Matrix::makeRotateMatrix(rotateZ, Vector(0.0f, 0.0f, 1.0f));

		Matrix rotateSpin= Matrix::makeRotateMatrix(floatRate * 10, Vector(0.0f, 1.0f, 0.0f));

		Matrix translate = Matrix::makeTranslationMatrix(x , y + sin(floatRate)/3, z);
		Matrix scale = Matrix::makeScaleMatrix(sizeX, sizeY, sizeZ);
		Matrix scaleMusic = Matrix::makeScaleMatrix(Clamp(((m_spectrumLeft[1] + m_spectrumRight[1]) / 2) * 2, 1, 2), Clamp(((m_spectrumLeft[0] + m_spectrumRight[0]) / 2) * 3, 1, 3), sizeZ);

		Matrix viewSpaceMatrix = viewMatrix * translate * rotate * rotate1 * rotate2  * scaleMusic * rotateSpin * scale;
		glLoadMatrixf((GLfloat*)viewSpaceMatrix.mVal);

		glDepthMask(false);

		glEnable(GL_BLEND);
		glEnable(GL_TEXTURE_2D);

		glEnableClientState(GL_VERTEX_ARRAY);
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);

		//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); //alpha
		glBlendFunc(GL_SRC_ALPHA, GL_ONE); //add
		//glBlendFunc(GL_SRC_ALPHA, GL_ZERO); //multiply

		glBindTexture(GL_TEXTURE_2D, mTextureID[textureID]);

		glVertexPointer(3, GL_FLOAT, 0, verticesDeco);
		glTexCoordPointer(2, GL_FLOAT, 0, texxtCoordsDeco);
		glDrawArrays(GL_TRIANGLES, 0, 24);

		glDisableClientState(GL_VERTEX_ARRAY);
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);

		glDisable(GL_BLEND);
		glDisable(GL_TEXTURE_2D);

		glDepthMask(true);

	}

	float DeltaTime() {

		deltaTime = clock() - oldTime;
		float fps = (1 / deltaTime) * 1000;
		oldTime = clock();

		return fps;

	}


	void draw(const Matrix& viewMatrix)
	{

		//drawAxis(viewMatrix);

		drawDeco(3, 3, 0, 0.4, 0.5, 0.5, 0, 0, 0, 20 + songNum, viewMatrix);
		drawDeco(-4.5, -1, 0, 0.4, 0.4, 0.4, 0, 0, -25, 20 + songNum, viewMatrix);

		glLoadMatrixf((GLfloat*)viewMatrix.mVal);

		//drawLightning(0, 30.0f);

		//enable wireframe mode
		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

		//drawLightning(0, 5.0f);
		updateFmod();
		drawBars(-20, -45);
		drawMusicLightning(0, -55);
		drawBackGround(0, 0, -56, 37, 25, songNum + 10);

		//blend(viewMatrix);
		
		
#pragma region clear



		//drawBars();

		/*
		glColor3f(1.0f, 1.0f, 1.0f);

		m_speed += 0.01;

		float wave = sin(m_speed);

		drawGrid(wave);
		*/

		//glEnable(GL_TEXTURE_2D);
		//glBindTexture(GL_TEXTURE_2D, mTextureID[0]);

		//glEnableClientState(GL_VERTEX_ARRAY);
		//glEnableClientState(GL_TEXTURE_COORD_ARRAY);

		//glVertexPointer(3, GL_FLOAT, 0, vertices);
		//glTexCoordPointer(2, GL_FLOAT, 0, textCoords);

		//glDrawArrays(GL_TRIANGLES, 0, number_of_square * 6);

		//glDisableClientState(GL_TEXTURE_COORD_ARRAY);
		//glDisableClientState(GL_VERTEX_ARRAY);

		//glDisable(GL_TEXTURE_2D);

		//======================================================================================================================================
		/*
		static GLfloat vertices[] = {

			// front
				-1.0f, -1.0f,  1.0f,
				 1.0f, -1.0f,  1.0f,
				 1.0f,  1.0f,  1.0f,
				 1.0f,  1.0f,  1.0f,
				-1.0f,  1.0f,  1.0f,
				-1.0f, -1.0f,  1.0f,

				// right
				 1.0f, -1.0f,  1.0f,
				 1.0f, -1.0f, -1.0f,
				 1.0f,  1.0f, -1.0f,
				 1.0f,  1.0f, -1.0f,
				 1.0f,  1.0f,  1.0f,
				 1.0f, -1.0f,  1.0f,

				 // back
				  1.0f, -1.0f, -1.0f,
				 -1.0f, -1.0f, -1.0f,
				 -1.0f,  1.0f, -1.0f,
				 -1.0f,  1.0f, -1.0f,
				  1.0f,  1.0f, -1.0f,
				  1.0f, -1.0f, -1.0f,

				  // left
				  -1.0f, -1.0f, -1.0f,
				  -1.0f, -1.0f,  1.0f,
				  -1.0f,  1.0f,  1.0f,
				  -1.0f,  1.0f,  1.0f,
				  -1.0f,  1.0f, -1.0f,
				  -1.0f, -1.0f, -1.0f,

				  // top
				  -1.0f,  1.0f,  1.0f,
				   1.0f,  1.0f,  1.0f,
				   1.0f,  1.0f, -1.0f,
				   1.0f,  1.0f, -1.0f,
				  -1.0f,  1.0f, -1.0f,
				  -1.0f,  1.0f,  1.0f,

				  // bottom
				  -1.0f, -1.0f,  1.0f,
				   1.0f, -1.0f,  1.0f,
				   1.0f, -1.0f, -1.0f,
				   1.0f, -1.0f, -1.0f,
				  -1.0f, -1.0f, -1.0f,
				  -1.0f, -1.0f,  1.0f


		};

		static GLfloat textCoords[] = {

			0.0f, 0.0f,
			1.0f, 0.0f,
			1.0f, 1.0f,
			1.0f, 1.0f,
			0.0f, 1.0f,
			0.0f, 0.0f,

			0.0f, 0.0f,
			1.0f, 0.0f,
			1.0f, 1.0f,
			1.0f, 1.0f,
			0.0f, 1.0f,
			0.0f, 0.0f,

			0.0f, 0.0f,
			1.0f, 0.0f,
			1.0f, 1.0f,
			1.0f, 1.0f,
			0.0f, 1.0f,
			0.0f, 0.0f,

			0.0f, 0.0f,
			1.0f, 0.0f,
			1.0f, 1.0f,
			1.0f, 1.0f,
			0.0f, 1.0f,
			0.0f, 0.0f,

			0.0f, 0.0f,
			1.0f, 0.0f,
			1.0f, 1.0f,
			1.0f, 1.0f,
			0.0f, 1.0f,
			0.0f, 0.0f,

			0.0f, 0.0f,
			1.0f, 0.0f,
			1.0f, 1.0f,
			1.0f, 1.0f,
			0.0f, 1.0f,
			0.0f, 0.0f,

		};

		static GLfloat colors[] = {

			1.0f, 1.0f, 1.0f,
			1.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f,
			0.0f, 1.0f, 0.0f,
			1.0f, 0.0f, 0.0f,
			1.0f, 1.0f, 1.0f,

			1.0f, 1.0f, 1.0f,
			1.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f,
			0.0f, 1.0f, 0.0f,
			1.0f, 0.0f, 0.0f,
			1.0f, 1.0f, 1.0f,

			1.0f, 1.0f, 1.0f,
			1.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f,
			0.0f, 1.0f, 0.0f,
			1.0f, 0.0f, 0.0f,
			1.0f, 1.0f, 1.0f,

			1.0f, 1.0f, 1.0f,
			1.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f,
			0.0f, 1.0f, 0.0f,
			1.0f, 0.0f, 0.0f,
			1.0f, 1.0f, 1.0f,

			1.0f, 1.0f, 1.0f,
			1.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f,
			0.0f, 1.0f, 0.0f,
			1.0f, 0.0f, 0.0f,
			1.0f, 1.0f, 1.0f,

			1.0f, 1.0f, 1.0f,
			1.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f,
			0.0f, 1.0f, 0.0f,
			1.0f, 0.0f, 0.0f,
			1.0f, 1.0f, 1.0f,

		};

		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, mTextureID[0]);

		glEnableClientState(GL_VERTEX_ARRAY);
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		glEnableClientState(GL_COLOR_ARRAY);

		glVertexPointer(3, GL_FLOAT, 0, vertices);
		glTexCoordPointer(2, GL_FLOAT, 0, textCoords);
		glColorPointer(3, GL_FLOAT, 0, colors);

		glDrawArrays(GL_TRIANGLES, 0, 36);

		glDisableClientState(GL_COLOR_ARRAY);
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
		glDisableClientState(GL_VERTEX_ARRAY);

		glDisable(GL_TEXTURE_2D);
	*/
	//drawTextureCuboid(8.0f, 0.5f);



	//Matrix translateWhole = Matrix::makeTranslationMatrix(m_rotation, 0, 0);
	//Matrix modelMatrixWhole = translateWhole;

	//Matrix viewSpaceMatrixWhole = viewMatrix * modelMatrixWhole;

	//glLoadMatrixf((GLfloat*)viewSpaceMatrixWhole.mVal);

	//drawPokemon(viewMatrix);

	//drawTextureCuboid(2.0f, 0.5f);

	//drawTriangle();

	//drawSquare();

	//drawCube();

	//drawPyramid();

	//drawCircle(0.0,0.0,0.5, 20);

	//drawCircleFan(0.0,0.0,0.5, 20);

	//drawCylinder(0.0,0.0,0.5, 0.5 ,20);

	/*
	if (onWayUp && m_speed >= 1) {

		onWayUp = false;

	}

	if (!onWayUp && m_speed <= -1) {

		onWayUp = true;

	}


	if (onWayUp) {

		m_speed += 0.05f;

	}
	else {

		m_speed -= 0.05f;

	}
	*/

	/*

	m_speed += 1;
	m_rotation += 0.05f;

	Matrix rotate1 = Matrix::makeRotateMatrix(m_rotation, Vector(0.0f, 1.0f, 0.0f));
	Matrix translate1 = Matrix::makeTranslationMatrix(cosf(m_rotation)* 3, 0, sinf(m_rotation)*10);


	//Note on OpenGL Matrix model:
	//Screen = Proj * View * Model
	//Model = TransformA(3rd) * TransformB(2nd) * TransformC(1st) (Transform could be Rotate, Scale, Translate, etc)

	//Perform model transformation
	Matrix modelMatrix = rotate1 * translate1;

	//Convert model space to view space
	Matrix viewSpaceMatrix = viewMatrix * modelMatrix;
	glLoadMatrixf((GLfloat*)viewSpaceMatrix.mVal);

	drawPyramid();



	Matrix rotate2 = Matrix::makeRotateMatrix(m_rotation, Vector(0.0f, 1.0f, 0.0f));

	Matrix modelMatrix2 = rotate2;
	Matrix viewSpaceMatrix2 = viewMatrix * modelMatrix2;
	glLoadMatrixf((GLfloat*)viewSpaceMatrix2.mVal);
	drawPyramid();
	*/

	/*
	m_rotation += 0.05f;

	int cube_num = 45;

	for (int i = 0; i < cube_num; i++)
	{
		float entityRotation = m_rotation - 0.5f * (float)i;

		Matrix translate1 = Matrix::makeTranslationMatrix(3.0 * cos(entityRotation), i /2.0 ,(3.0 * sin(entityRotation)));

		Matrix modelMatrix = translate1;

		Matrix viewSpaceMatrix = viewMatrix * modelMatrix;
		glLoadMatrixf((GLfloat*)viewSpaceMatrix.mVal);

		drawCube();
	}

	*/

	/*
	glColor3f(1.0f, 1.0f, 1.0f);

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, mTextureID[0]);

	float size = 0.5f;

	/*
	glBegin(GL_TRIANGLES);

		glTexCoord2f(0.0f, 0.0f); glVertex3f(-size, -size, size);
		glTexCoord2f(size, 0.0f); glVertex3f(size, -size, size);
		glTexCoord2f(size, size); glVertex3f(size, size, size);

		glTexCoord2f(size, size); glVertex3f(size, size, size);
		glTexCoord2f(0.0f, size); glVertex3f(-size, size, size);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(-size, -size, size);

	glEnd();
	*/

	/*
	mUShift += 0.01f;

	glBegin(GL_TRIANGLES);								// Drawing Using Triangles

	//front
	//Left Triangle
	//------------------------------------------------
	glTexCoord2f(0.0f + mUShift, 0.0f);
	glVertex3f(-size, -size, size); //bot left

	glTexCoord2f(0.0f + mUShift, 1.0f);
	glVertex3f(-size, size, size); //top left

	glTexCoord2f(1.0f + mUShift, 1.0f);
	glVertex3f(size, size, size); //top right


	//Right Triangle
	glTexCoord2f(1.0f + mUShift, 0.0f);
	glVertex3f(size, -size, size); // bot right

	glTexCoord2f(0.0f + mUShift, 0.0f);
	glVertex3f(-size, -size, size); //bot left

	glTexCoord2f(1.0f + mUShift, 1.0f);
	glVertex3f(size, size, size); // top right
	//--------------------------------------------------

	//right
	//Left Triangle
	//------------------------------------------------
	glTexCoord2f(0.0f + mUShift, 0.0f);
	glVertex3f(size, -size, size); //bot left

	glTexCoord2f(0.0f + mUShift, 1.0f);
	glVertex3f(size, size, size); //top left

	glTexCoord2f(1.0f + mUShift, 1.0f);
	glVertex3f(size, size, -size); //top right

	//Right Triangle
	glTexCoord2f(1.0f + mUShift, 0.0f);
	glVertex3f(size, -size, -size); // bot right

	glTexCoord2f(0.0f + mUShift, 0.0f);
	glVertex3f(size, -size, size); //bot left

	glTexCoord2f(1.0f + mUShift, 1.0f);
	glVertex3f(size, size, -size); // top right
	//-------------------------------------------------

	//left
	//Left Triangle
	//------------------------------------------------
	glTexCoord2f(0.0f + mUShift, 0.0f);
	glVertex3f(-size, -size, -size); //bot left

	glTexCoord2f(0.0f + mUShift, 1.0f);
	glVertex3f(-size, size, -size); //top left

	glTexCoord2f(1.0f + mUShift, 1.0f);
	glVertex3f(-size, size, size); //top right

	//Right Triangle
	glTexCoord2f(1.0f + mUShift, 0.0f);
	glVertex3f(-size, -size, size); // bot right

	glTexCoord2f(0.0f + mUShift, 0.0f);
	glVertex3f(-size, -size, -size); //bot left

	glTexCoord2f(1.0f + mUShift, 1.0f);
	glVertex3f(-size, size, size); // top right
	//-------------------------------------------------

	//back
	//Left Triangle
	//------------------------------------------------
	glTexCoord2f(0.0f + mUShift, 0.0f);
	glVertex3f(size, -size, -size); //bot left

	glTexCoord2f(0.0f + mUShift, 1.0f);
	glVertex3f(size, size, -size); //top left

	glTexCoord2f(1.0f + mUShift, 1.0f);
	glVertex3f(-size, size, -size); //top right

	//Right Triangle
	glTexCoord2f(1.0f + mUShift, 0.0f);
	glVertex3f(-size, -size, -size); // bot right

	glTexCoord2f(0.0f + mUShift, 0.0f);
	glVertex3f(size, -size, -size); //bot left

	glTexCoord2f(1.0f + mUShift, 1.0f);
	glVertex3f(-size, size, -size); // top right

	//-------------------------------------------------

	//top
	//Left Triangle
	//------------------------------------------------
	glTexCoord2f(0.0f + mUShift, 0.0f);
	glVertex3f(-size, size, size); //bot left

	glTexCoord2f(0.0f + mUShift, 1.0f);
	glVertex3f(-size, size, -size); //top left

	glTexCoord2f(1.0f + mUShift, 1.0f);
	glVertex3f(size, size, -size); //top right

	//Right Triangle
	glTexCoord2f(1.0f + mUShift, 0.0f);
	glVertex3f(size, size, size); // bot right

	glTexCoord2f(0.0f + mUShift, 0.0f);
	glVertex3f(-size, size, size); //bot left

	glTexCoord2f(1.0f + mUShift, 1.0f);
	glVertex3f(size, size, -size); // top right

	//-------------------------------------------------

	//bot
	//Left Triangle
	//------------------------------------------------
	glTexCoord2f(0.0f + mUShift, 0.0f);
	glVertex3f(-size, -size, -size); //bot left

	glTexCoord2f(0.0f + mUShift, 1.0f);
	glVertex3f(-size, -size, size); //top left

	glTexCoord2f(1.0f + mUShift, 1.0f);
	glVertex3f(size, -size, size); //top right

	//Right Triangle
	glTexCoord2f(1.0f + mUShift, 0.0f);
	glVertex3f(size, -size, -size); // bot right

	glTexCoord2f(0.0f + mUShift, 0.0f);
	glVertex3f(-size, -size, -size); //bot left

	glTexCoord2f(1.0f + mUShift, 1.0f);
	glVertex3f(size, -size, size); // top right

	//-------------------------------------------------
	*/

	/*
	mUShift += 0.01f;

	glBegin(GL_TRIANGLES);								// Drawing Using Triangles

	//front
	//Left Triangle
	//------------------------------------------------
	glTexCoord2f(0.0f * mUShift, 0.0f * mUShift);
	glVertex3f(-size, -size, size); //bot left

	glTexCoord2f(0.0f * mUShift, 1.0f * mUShift);
	glVertex3f(-size, size, size); //top left

	glTexCoord2f(1.0f * mUShift, 1.0f * mUShift);
	glVertex3f(size, size, size); //top right


	//Right Triangle
	glTexCoord2f(1.0f * mUShift, 0.0f * mUShift);
	glVertex3f(size, -size, size); // bot right

	glTexCoord2f(0.0f * mUShift, 0.0f * mUShift);
	glVertex3f(-size, -size, size); //bot left

	glTexCoord2f(1.0f * mUShift, 1.0f * mUShift);
	glVertex3f(size, size, size); // top right
	//--------------------------------------------------

	//right
	//Left Triangle
	//------------------------------------------------
	glTexCoord2f(0.0f * mUShift, 0.0f * mUShift);
	glVertex3f(size, -size, size); //bot left

	glTexCoord2f(0.0f * mUShift, 1.0f * mUShift);
	glVertex3f(size, size, size); //top left

	glTexCoord2f(1.0f * mUShift, 1.0f * mUShift);
	glVertex3f(size, size, -size); //top right

	//Right Triangle
	glTexCoord2f(1.0f * mUShift, 0.0f * mUShift);
	glVertex3f(size, -size, -size); // bot right

	glTexCoord2f(0.0f * mUShift, 0.0f * mUShift);
	glVertex3f(size, -size, size); //bot left

	glTexCoord2f(1.0f * mUShift, 1.0f * mUShift);
	glVertex3f(size, size, -size); // top right
	//-------------------------------------------------

	//left
	//Left Triangle
	//------------------------------------------------
	glTexCoord2f(0.0f * mUShift, 0.0f * mUShift);
	glVertex3f(-size, -size, -size); //bot left

	glTexCoord2f(0.0f * mUShift, 1.0f * mUShift);
	glVertex3f(-size, size, -size); //top left

	glTexCoord2f(1.0f * mUShift, 1.0f * mUShift);
	glVertex3f(-size, size, size); //top right

	//Right Triangle
	glTexCoord2f(1.0f * mUShift, 0.0f * mUShift);
	glVertex3f(-size, -size, size); // bot right

	glTexCoord2f(0.0f * mUShift, 0.0f * mUShift);
	glVertex3f(-size, -size, -size); //bot left

	glTexCoord2f(1.0f * mUShift, 1.0f * mUShift);
	glVertex3f(-size, size, size); // top right
	//-------------------------------------------------

	//back
	//Left Triangle
	//------------------------------------------------
	glTexCoord2f(0.0f * mUShift, 0.0f * mUShift);
	glVertex3f(size, -size, -size); //bot left

	glTexCoord2f(0.0f * mUShift, 1.0f * mUShift);
	glVertex3f(size, size, -size); //top left

	glTexCoord2f(1.0f * mUShift, 1.0f * mUShift);
	glVertex3f(-size, size, -size); //top right

	//Right Triangle
	glTexCoord2f(1.0f * mUShift, 0.0f * mUShift);
	glVertex3f(-size, -size, -size); // bot right

	glTexCoord2f(0.0f * mUShift, 0.0f * mUShift);
	glVertex3f(size, -size, -size); //bot left

	glTexCoord2f(1.0f * mUShift, 1.0f * mUShift);
	glVertex3f(-size, size, -size); // top right

	//-------------------------------------------------

	//top
	//Left Triangle
	//------------------------------------------------
	glTexCoord2f(0.0f * mUShift, 0.0f * mUShift);
	glVertex3f(-size, size, size); //bot left

	glTexCoord2f(0.0f * mUShift, 1.0f * mUShift);
	glVertex3f(-size, size, -size); //top left

	glTexCoord2f(1.0f * mUShift, 1.0f * mUShift);
	glVertex3f(size, size, -size); //top right

	//Right Triangle
	glTexCoord2f(1.0f * mUShift, 0.0f * mUShift);
	glVertex3f(size, size, size); // bot right

	glTexCoord2f(0.0f * mUShift, 0.0f * mUShift);
	glVertex3f(-size, size, size); //bot left

	glTexCoord2f(1.0f * mUShift, 1.0f * mUShift);
	glVertex3f(size, size, -size); // top right

	//-------------------------------------------------

	//bot
	//Left Triangle
	//------------------------------------------------
	glTexCoord2f(0.0f + mUShift, 0.0f * mUShift);
	glVertex3f(-size, -size, -size); //bot left

	glTexCoord2f(0.0f + mUShift, 1.0f * mUShift);
	glVertex3f(-size, -size, size); //top left

	glTexCoord2f(1.0f + mUShift, 1.0f * mUShift);
	glVertex3f(size, -size, size); //top right

	//Right Triangle
	glTexCoord2f(1.0f + mUShift, 0.0f * mUShift);
	glVertex3f(size, -size, -size); // bot right

	glTexCoord2f(0.0f + mUShift, 0.0f * mUShift);
	glVertex3f(-size, -size, -size); //bot left

	glTexCoord2f(1.0f + mUShift, 1.0f * mUShift);
	glVertex3f(size, -size, size); // top right

	//-------------------------------------------------

//aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa

		/*
		//left
		glTexCoord2f(size, -size);  glVertex3f(-size, size, size);
		glTexCoord2f(0.0f, -size);  glVertex3f(-size, -size, size);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(-size, -size, -size);

		glTexCoord2f(0.0f, size); glVertex3f(-size, -size, -size);
		glTexCoord2f(size, 0.0f); glVertex3f(-size, size, size);
		glTexCoord2f(size, size); glVertex3f(-size, size, -size);

		//right

		glTexCoord2f(size, -size); glVertex3f(size, size, size);
		glTexCoord2f(0.0f, -size); glVertex3f(size, -size, size);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(size, -size, -size);

		glTexCoord2f(0.0f, size); glVertex3f(size, -size, -size);
		glTexCoord2f(size, 0.0f); glVertex3f(size, size, size);
		glTexCoord2f(size, size); glVertex3f(size, size, -size);

		//bot
		glTexCoord2f(size, size); glVertex3f(-size, -size, size);
		glTexCoord2f(size, 0.0f); glVertex3f(size, -size, size);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(size, -size, -size);

		glTexCoord2f(0.0f, 0.0f); glVertex3f(-size, -size, size);
		glTexCoord2f(0.0f, -size);  glVertex3f(-size, -size, -size);
		glTexCoord2f(-size, -size); glVertex3f(size, -size, -size);

		//top
		glTexCoord2f(-size, -size); glVertex3f(-size, size, size);
		glTexCoord2f(-size, 0.0f); glVertex3f(size, size, size);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(size, size, -size);

		glTexCoord2f(0.0f, 0.0f); glVertex3f(-size, size, size); //origin corner
		glTexCoord2f(0.0f, size); glVertex3f(-size, size, -size); //back corner
		glTexCoord2f(size, size); glVertex3f(size, size, -size);



		glEnd();
		*/
#pragma endregion
	}
};

#endif
