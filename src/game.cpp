/*******************************************************************
** This code is part of Breakout.
**
** Breakout is free software: you can redistribute it and/or modify
** it under the terms of the CC BY 4.0 license as published by
** Creative Commons, either version 4 of the License, or (at your
** option) any later version.
******************************************************************/
#include "game.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/vector_angle.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/euler_angles.hpp>
#include "resource_manager.hpp"
#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>
#include <iterator>
#include <iomanip>
#include <stdlib.h>
#include <math.h>
#include "rectangle.hpp"
#include "text.hpp"
#include "cube_map.hpp"
#include "cube.hpp"
#include "model.hpp"
#include "model_container.hpp"
#include "entity.hpp"
#include "physics.hpp"
#include "level.hpp"
#include "menu.hpp"

// 1 unit = 1 m/s
Game::Game(GLuint width, GLuint height) : state(GAME_MENU), keys(), Width(width), Height(height) {
	this->sigQuit = false;
	this->debugEnabled = false;
}
Game::~Game() {}

void Game::Init() {
	Shader shaderMvp = ResourceManager::LoadShader("resources/shaders/shader_mvp.vs", "resources/shaders/shader_mvp.fs", nullptr, "mvp");
	Shader shaderText = ResourceManager::LoadShader("resources/shaders/shader_text.vs", "resources/shaders/shader_text.fs", nullptr, "text");
	Shader shaderModel = ResourceManager::LoadShader("resources/shaders/shader_model.vs", "resources/shaders/shader_model.fs", nullptr, "model");
	Shader shaderCubeMap = ResourceManager::LoadShader("resources/shaders/shader_cubemap.vs", "resources/shaders/shader_cubemap.fs", nullptr, "cubemap");
	
	ResourceManager::LoadTexture("resources/images/sample.png", GL_TRUE, "kitten");
	ResourceManager::LoadTexture("resources/images/sample2.png", GL_TRUE, "puppy");
	ResourceManager::LoadTexture3D("resources/images/Starscape.png", "skybox");

	text = new Text(shaderText, this->Width, this->Height);
	text->Load("resources/fonts/Inconsolata-Regular.ttf", 16);
	cubeMap = new CubeMap(shaderCubeMap);

	this->menu = new Menu(this->Width, this->Height);
	this->menu->init();

	this->samples = 0;
	this->elapsed = 0.0f;

	this->loadingStream << "Loading";
}

void Game::ProcessInput(GLfloat dt) {
	// Calculate elapsed time to use for debouncing toggle keys
	this->elapsed = dt * this->samples;

	if(this->state == GAME_MENU && menu->transition) {
		this->state = GAME_LOADING;
		this->levelFuture = std::async(std::launch::async, parseLevelFile, 0, this->Width, this->Height);
	}

	if(this->state == GAME_LOADING) {
		std::future_status loadStatus = this->levelFuture.wait_for(std::chrono::milliseconds(25));
		if(loadStatus == std::future_status::ready) {
			this->state = GAME_ACTIVE;
			Level* level = this->levelFuture.get();
			if(level->models.size() < 1) {
				std::cout << "ERROR: Level was not loaded successfully" << std::endl;
				this->quit();
			}
			level->Init();
			this->levels.push_back(std::unique_ptr<Level>(level));
			std::cout << "Added level " << level->id << " to levels, which now has size " << this->levels.size() << std::endl;
		} else if(loadStatus == std::future_status::timeout) {
			this->loadingStream << ".";
		}
	}

	if(this->state == GAME_MENU) {
		this->menu->processInput(dt, this->keys);
	} else if(this->state == GAME_ACTIVE) {
		levels[0]->ProcessInput(dt, this->keys);
	}

	if(keys[GLFW_KEY_Q]) {
		this->quit();
	}
	if(keys[GLFW_KEY_F12] && this->elapsed > 0.25f) {
		this->samples = 0;
		this->debugEnabled = !this->debugEnabled;
	}
	if(this->samples < INT_MAX - 1) {
		this->samples++;
	}
}

void Game::Update(GLfloat dt)
{
	fps = 1.0f / dt;
	if(this->state == GAME_MENU) {
		this->menu->update(dt);
	} else if(this->state == GAME_ACTIVE) {
		levels[0]->Update(dt);
	}
}

void Game::render()
{
	if(this->state == GAME_MENU) {
		this->menu->render(this->debugEnabled);
	} else if(this->state == GAME_LOADING) {
		this->text->render(this->loadingStream.str(), (float) this->Width / 2 - 25.0f, (float) this->Height / 2 + 35.0f, 1.0f);
	} else if(this->state == GAME_ACTIVE) {
		this->levels[0]->render(this->debugEnabled);
	}

	if(this->debugEnabled) {
		renderFps();
	}
}

void Game::renderFps() {
	std::stringstream fpsStream;
	fpsStream << static_cast<int>(floor(fps));
	text->render(fpsStream.str(), 5.0f, 5.0f, 1.0f);
}

void Game::pressKey(int key) {
	this->keys[key] = GL_TRUE;
}

void Game::releaseKey(int key) {
	this->keys[key] = GL_FALSE;
}

void Game::quit() {
	this->sigQuit = true;
}