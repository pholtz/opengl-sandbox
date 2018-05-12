#ifndef LEVEL_H
#define LEVEL_H
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <vector>
#include <iostream>
#include <memory>

class CubeMap;
class Cube;
class Text;
class Model;
class ModelContainer;
class Entity;

class Level
{
public:
	int id;
	GLuint width, height;
	CubeMap *cubeMap;
	Cube *cube;
	Text *text;
	Entity *entity;
	std::vector<std::unique_ptr<ModelContainer>> models;
	std::vector<glm::vec3> cubes;
	Level(int id, GLuint width, GLuint height);
	~Level();
	void Init();
	void ProcessInput(GLfloat dt, GLboolean keys[1024]);
	void Update(GLfloat dt);
	void render(GLboolean debugEnabled);
	void renderCoordinates(Entity* entity);
};

Level* parseLevelFile(int id, GLuint width, GLuint height);
#endif