#ifndef _TRIANGLE_DEMO_H
#define _TRIANGLE_DEMO_H

#include "demo_base.h"
#include "bitmap.h"
#include <vector>
#include <iostream>
using namespace std;

#define TEXTURE_COUNT 1

#define RECT_VERTICE_W 10
#define RECT_VERTICE_H 10

//every quad has 6
const int RECT_VERTEX_COUNT = (RECT_VERTICE_W - 1) * (RECT_VERTICE_H - 1) *6;

const int RECT_VERTEX_ARRAY_SIZE = RECT_VERTEX_COUNT * 3;

const int RECT_UV_ARRAY_SIZE = RECT_VERTEX_COUNT * 2;

const int number_of_square = RECT_VERTICE_W * RECT_VERTICE_H;

const int number_of_vertices = number_of_square * 18;

const int number_of_texture = number_of_square * 12;

const int nunmber_of_color = number_of_square* 18;

class TriangleDemo : public DemoBase
{

private: 
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
	void init()
	{
		/*
			-1.0f, -1.0f, 1.0f,
			1.0f, -1.0f, 1.0f,
			1.0f, -1.0f, -1.0f,

			1.0f, -1.0f, -1.0f,
			-1.0f, -1.0f, -1.0f,
			-1.0f, -1.0f, 1.0f
		*/

		m_rotation = 0.0f;
		m_speed = 0.0f;
		onWayUp = true;
		walkSpeed = 0;
		wanToGoX = 1;

		glGenTextures(TEXTURE_COUNT, mTextureID);
		loadTexture("../media/glass.bmp", mTextureID[0]);

	}

	void deinit()
	{
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

			glVertex3f( 0.0f, 1.0f, 0.0f);				
			glVertex3f( 1.0f,0.0f, 0.0f);					
			glVertex3f( 0.0f, 0.0f, 0.0f);	

		glEnd();	
	}

	void drawSquare(void){

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

	void drawCircle(GLfloat x, GLfloat y, GLfloat z,float r, int numberOfTriangles) {
	
		GLfloat doublePi = 2.0f * M_PI;
		
		for (int i = 0; i <= numberOfTriangles; i++) {

			glBegin(GL_TRIANGLES);

			glVertex3f(x, y, z); // center of circle

			glVertex3f(x + (r * cos(doublePi * i / numberOfTriangles)), y,z + (r * sin(doublePi * i / numberOfTriangles)));

			glVertex3f(x + (r * cos(doublePi * (i + 1) / numberOfTriangles)), y,z + (r * sin(doublePi * (i + 1) / numberOfTriangles)));

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

			glVertex3f(x + (r * cos(doublePi * i / numberOfTriangles)), originy + height,y + (r * sin(doublePi * i / numberOfTriangles)));

			glVertex3f(x + (r * cos(doublePi * (i + 1) / numberOfTriangles)), originy + height ,y + (r * sin(doublePi * (i + 1) / numberOfTriangles)));

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

				normalX = (x + (newX * cos(M_2PI * i / numberOfTriangles))) * 1.0f/radius;
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
		glTexCoord2f(0.0f , 0.0f);
		glVertex3f(-sizeX, -sizeY, sizeX); //bot left

		glTexCoord2f(0.0f , 1.0f);
		glVertex3f(-sizeX, sizeY, sizeX); //top left

		glTexCoord2f(1.0f , 1.0f);
		glVertex3f(sizeX, sizeY, sizeX); //top right


		//Right Triangle
		glTexCoord2f(1.0f , 0.0f);
		glVertex3f(sizeX, -sizeY, sizeX); // bot right

		glTexCoord2f(0.0f , 0.0f);
		glVertex3f(-sizeX, -sizeY, sizeX); //bot left

		glTexCoord2f(1.0f , 1.0f);
		glVertex3f(sizeX, sizeY, sizeX); // top right
		//--------------------------------------------------

		//right
		//Left Triangle
		//------------------------------------------------
		glTexCoord2f(0.0f , 0.0f);
		glVertex3f(sizeX, -sizeY, sizeX); //bot left

		glTexCoord2f(0.0f , 1.0f);
		glVertex3f(sizeX, sizeY, sizeX); //top left

		glTexCoord2f(1.0f , 1.0f);
		glVertex3f(sizeX, sizeY, -sizeX); //top right

		//Right Triangle
		glTexCoord2f(1.0f , 0.0f);
		glVertex3f(sizeX, -sizeY, -sizeX); // bot right

		glTexCoord2f(0.0f , 0.0f);
		glVertex3f(sizeX, -sizeY, sizeX); //bot left

		glTexCoord2f(1.0f , 1.0f);
		glVertex3f(sizeX, sizeY, -sizeX); // top right
		//-------------------------------------------------

		//left
		//Left Triangle
		//------------------------------------------------
		glTexCoord2f(0.0f , 0.0f);
		glVertex3f(-sizeX, -sizeY, -sizeX); //bot left

		glTexCoord2f(0.0f , 1.0f);
		glVertex3f(-sizeX, sizeY, -sizeX); //top left

		glTexCoord2f(1.0f , 1.0f);
		glVertex3f(-sizeX, sizeY, sizeX); //top right

		//Right Triangle
		glTexCoord2f(1.0f , 0.0f);
		glVertex3f(-sizeX, -sizeY, sizeX); // bot right

		glTexCoord2f(0.0f , 0.0f);
		glVertex3f(-sizeX, -sizeY, -sizeX); //bot left

		glTexCoord2f(1.0f , 1.0f);
		glVertex3f(-sizeX, sizeY, sizeX); // top right
		//-------------------------------------------------

		//back
		//Left Triangle
		//------------------------------------------------
		glTexCoord2f(0.0f , 0.0f);
		glVertex3f(sizeX, -sizeY, -sizeX); //bot left

		glTexCoord2f(0.0f , 1.0f);
		glVertex3f(sizeX, sizeY, -sizeX); //top left

		glTexCoord2f(1.0f , 1.0f);
		glVertex3f(-sizeX, sizeY, -sizeX); //top right

		//Right Triangle
		glTexCoord2f(1.0f , 0.0f);
		glVertex3f(-sizeX, -sizeY, -sizeX); // bot right

		glTexCoord2f(0.0f , 0.0f);
		glVertex3f(sizeX, -sizeY, -sizeX); //bot left

		glTexCoord2f(1.0f , 1.0f);
		glVertex3f(-sizeX, sizeY, -sizeX); // top right

		//-------------------------------------------------

		//top
		//Left Triangle
		//------------------------------------------------
		glTexCoord2f(0.0f , 0.0f);
		glVertex3f(-sizeX, sizeY, sizeX); //bot left

		glTexCoord2f(0.0f , 1.0f);
		glVertex3f(-sizeX, sizeY, -sizeX); //top left

		glTexCoord2f(1.0f , 1.0f);
		glVertex3f(sizeX, sizeY, -sizeX); //top right

		//Right Triangle
		glTexCoord2f(1.0f , 0.0f);
		glVertex3f(sizeX, sizeY, sizeX); // bot right

		glTexCoord2f(0.0f , 0.0f);
		glVertex3f(-sizeX, sizeY, sizeX); //bot left

		glTexCoord2f(1.0f , 1.0f);
		glVertex3f(sizeX, sizeY, -sizeX); // top right

		//-------------------------------------------------

		//bot
		//Left Triangle
		//------------------------------------------------
		glTexCoord2f(0.0f , 0.0f);
		glVertex3f(-sizeX, -sizeY, -sizeX); //bot left

		glTexCoord2f(0.0f , 1.0f);
		glVertex3f(-sizeX, -sizeY, sizeX); //top left

		glTexCoord2f(1.0f , 1.0f);
		glVertex3f(sizeX, -sizeY, sizeX); //top right

		//Right Triangle
		glTexCoord2f(1.0f , 0.0f);
		glVertex3f(sizeX, -sizeY, -sizeX); // bot right

		glTexCoord2f(0.0f , 0.0f);
		glVertex3f(-sizeX, -sizeY, -sizeX); //bot left

		glTexCoord2f(1.0f , 1.0f);
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
		Matrix rotateAll = Matrix::makeRotateMatrix( bodyRotation, Vector(rotateX, rotateY, rotateZ));
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

		Matrix jumpFeet= Matrix::makeRotateMatrix(jumpFeetAngle, Vector(1.0f, 0.0f, 0.0f));


		glColor3f(0.95f, 0.89f, 0.73f);

		Matrix translateFeetLeft1 = Matrix::makeTranslationMatrix(-0.8, -2.1, 0.5);
		Matrix scaleFeetLeft1 = Matrix::makeScaleMatrix(0.6, 0.5, 1.1);
		Matrix rotateFeetLeft1 = Matrix::makeRotateMatrix(90, Vector(0.0f, 1.0f, 0.0f));

		Matrix modelMatrixFeetLeft1 =  modelMatrixAll *  translateFeetLeft1 * jumpFeet * scaleFeetLeft1 * rotateFeetLeft1;

		Matrix viewSpaceMatrixFeetLeft1 = viewMatrix * modelMatrixFeetLeft1;

		glLoadMatrixf((GLfloat*)viewSpaceMatrixFeetLeft1.mVal);

		drawSphere(0, 0, 0, 1.0f, 30);

		Matrix translateFeetLeftBottom1 = Matrix::makeTranslationMatrix(-0.8, -2.3, 0.5);
		Matrix scaleFeetLeftBottom1 = Matrix::makeScaleMatrix(0.6, 0.2, 1.1);
		Matrix rotateFeetLeftBottom1 = Matrix::makeRotateMatrix(90, Vector(0.0f, 1.0f, 0.0f));
		Matrix modelMatrixFeetLeftBottom1 = modelMatrixAll * translateFeetLeftBottom1 * jumpFeet * scaleFeetLeftBottom1;

		Matrix viewSpaceMatrixFeetLeftBottom1 = viewMatrix * modelMatrixFeetLeftBottom1;

		glLoadMatrixf((GLfloat*)viewSpaceMatrixFeetLeftBottom1.mVal);

		drawCylinder(0,0, 1, 1,30);

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

		/*
		for (int u = 0; u < number_of_square; u++)
		{
			textCoords[u] = 0.0f; textCoords[u + 1] = 0.0f;
			textCoords[u + 2] = 1.0f; textCoords[u + 3] = 0.0f;
			textCoords[u + 4] = 1.0f; textCoords[u + 5] = 1.0f;
			textCoords[u + 6] = 1.0f; textCoords[u + 7] = 1.0f;
			textCoords[u + 8] = 0.0f; textCoords[u + 9] = 1.0f;
			textCoords[u + 10] = 0.0f; textCoords[u + 11] = 0.0f;
		}
		*/
	}

	void draw(const Matrix& viewMatrix)
	{

		//drawAxis(viewMatrix);

		glLoadMatrixf((GLfloat*)viewMatrix.mVal);

		//enable wireframe mode
		glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
		glColor3f(1.0f, 1.0f, 1.0f);

		m_speed += 0.01;

		float wave = sin(m_speed);

		drawGrid(wave);

		//glEnable(GL_TEXTURE_2D);
		//glBindTexture(GL_TEXTURE_2D, mTextureID[0]);

		glEnableClientState(GL_VERTEX_ARRAY);
		//glEnableClientState(GL_TEXTURE_COORD_ARRAY);

		glVertexPointer(3, GL_FLOAT, 0, vertices);
		//glTexCoordPointer(2, GL_FLOAT, 0, textCoords);

		glDrawArrays(GL_TRIANGLES, 0, number_of_square * 6);

		//glDisableClientState(GL_TEXTURE_COORD_ARRAY);
		glDisableClientState(GL_VERTEX_ARRAY);

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

	}
};

#endif
