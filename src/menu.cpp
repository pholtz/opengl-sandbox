#include "menu.hpp"
#include <sstream>
#include <iomanip>
#include <GLFW/glfw3.h>
#include "resource_manager.hpp"
#include "texture_3d.hpp"
#include "cube_map.hpp"
#include "entity.hpp"
#include "texture.hpp"
#include "cube.hpp"
#include "text.hpp"
#include "physics.hpp"

Menu::Menu(GLuint width, GLuint height) {
	this->width = width;
	this->height = height;

	this->yOffset = (float) height / 2 + 35.0f;
	this->xOffset = (float) width / 2;
	this->titleOffset = yOffset - 50.0f;
	this->playOffset = yOffset;
	this->settingsOffset = yOffset + 1 * OFFSET_DIFFERENTIAL;
	this->quitOffset = yOffset + 2 * OFFSET_DIFFERENTIAL;
	this->caretOffset = this->playOffset;
	this->caretIndex = 0;

	this->samples = 0;
	this->elapsed = 0.0f;
	this->transition = false;
}
Menu::~Menu() {}

void Menu::init() {
	Shader shaderCubeMap = ResourceManager::GetShader("cubemap");
	Shader shaderText = ResourceManager::GetShader("text");

	cubeMap = new CubeMap(shaderCubeMap);
	text = new Text(shaderText, this->width, this->height);
	text->Load("resources/fonts/Inconsolata-Regular.ttf", 22);

	entity = new Entity(glm::vec3(0.0f, 0.0f, 10.0f), glm::vec3(0.0f, 0.0f, -1.0f));
}

void Menu::processInput(GLfloat dt, GLboolean keys[1024]) {
	// Calculate elapsed time to use for debouncing toggle keys
	this->elapsed = dt * this->samples;
	if(keys[GLFW_KEY_UP] == GL_TRUE && this->elapsed > 0.25f) {
		this->samples = 0;
		this->caretIndex--;
	}
	if(keys[GLFW_KEY_DOWN] == GL_TRUE && this->elapsed > 0.25f) {
		this->samples = 0;
		this->caretIndex++;
	}
	if(keys[GLFW_KEY_ENTER] == GL_TRUE) {
		if(this->caretIndex == 0) {
			this->transition = true;
		}
	}
	if(keys[GLFW_KEY_C] == GL_TRUE) {
		entity->reset();
	}
	if(this->samples < INT_MAX - 1) {
		this->samples++;
	}
}

void Menu::update(GLfloat dt) {
	// Roll over caret alignment in either upwards or downwards direction
	if(this->caretIndex < 0) {
		this->caretIndex = 2;
	}
	if(this->caretIndex > 2) {
		this->caretIndex = 0;
	}
	// Calculate offset based on caret index
	this->caretOffset = this->caretIndex * OFFSET_DIFFERENTIAL + this->playOffset;
}

void Menu::render(GLboolean debugEnabled) {
	glm::mat4 view = physics::calculateViewMatrix(this->entity->position, this->entity->orientation, this->entity->up);
	glm::mat4 projection = physics::calculateProjectionMatrix(45.0f, (float) this->width / (float) this->height, 0.1f, 1000.0f);
	this->text->render("Spaceflight Simulator", this->xOffset, this->titleOffset, 1.0f);
	this->text->render("Play", this->xOffset, this->playOffset, 1.0f);
	this->text->render("Settings", this->xOffset, this->settingsOffset, 1.0f);
	this->text->render("Quit", this->xOffset, this->quitOffset, 1.0f);
	this->text->render(">", this->xOffset - 50.0f, this->caretOffset, 1.0f);
	this->cubeMap->render(ResourceManager::GetTexture3D("skybox"), view, projection);
}
