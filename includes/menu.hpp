#ifndef MENU_H
#define MENU_H
#include <GL/glew.h>

class CubeMap;
class Cube;
class Text;
class Entity;

class Menu
{
public:
	const float OFFSET_DIFFERENTIAL = 25.0f;
	GLuint width, height;
	CubeMap *cubeMap;
	Text *text;
	Entity *entity;
	int caretIndex;
	float xOffset, yOffset, titleOffset, playOffset, settingsOffset, quitOffset, caretOffset;
	GLuint samples;
	float elapsed;
	bool transition;
	Menu(GLuint width, GLuint height);
	~Menu();
	void init();
	void processInput(GLfloat dt, GLboolean keys[1024]);
	void update(GLfloat dt);
	void render(GLboolean debugEnabled);
};
#endif