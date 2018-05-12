/*******************************************************************
** This code is part of Breakout.
**
** Breakout is free software: you can redistribute it and/or modify
** it under the terms of the CC BY 4.0 license as published by
** Creative Commons, either version 4 of the License, or (at your
** option) any later version.
******************************************************************/
#ifndef GAME_H
#define GAME_H
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <vector>
#include <string>
#include <thread>
#include <future>
#include <sstream>

class Rectangle;
class Text;
class CubeMap;
class Cube;
class Model;
class ModelContainer;
class Entity;
class Level;
class Menu;

enum GameState {
	GAME_ACTIVE,
	GAME_LOADING,
	GAME_MENU,
	GAME_WIN
};

// Giant bag of globals
class Game
{
public:
	bool sigQuit;
	GameState state;	
	GLboolean keys[1024];
	GLuint Width, Height;
	Text *text;
	CubeMap *cubeMap;
	std::vector<std::unique_ptr<Level>> levels;
	Menu *menu;
	GLfloat fps;
	float elapsed;
	GLuint samples;

	std::thread worker;
	std::future<Level*> levelFuture;
	std::stringstream loadingStream;
	
	Game(GLuint width, GLuint height);
	~Game();
	void Init();
	void ProcessInput(GLfloat dt);
	void Update(GLfloat dt);
	void render();
	void renderFps();
	void pressKey(int key);
	void releaseKey(int key);
	void quit();
private:
	GLboolean debugEnabled;
};
#endif