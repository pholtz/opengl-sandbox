#include "level.hpp"
#include <sstream>
#include <iomanip>
#include <fstream>
#include <iterator>
#include <thread>
#include <math.h>
#include <stdlib.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/vector_angle.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/euler_angles.hpp>
#include "model_container.hpp"
#include "resource_manager.hpp"
#include "texture_3d.hpp"
#include "cube_map.hpp"
#include "entity.hpp"
#include "texture.hpp"
#include "cube.hpp"
#include "text.hpp"
#include "physics.hpp"

Level::Level(int id, GLuint width, GLuint height) {
	this->id = id;
	this->width = width;
	this->height = height;
	srand(time(NULL));
}
Level::~Level() {}

void Level::Init() {
	Shader shaderMvp = ResourceManager::GetShader("mvp");
	Shader shaderCubeMap = ResourceManager::GetShader("cubemap");
	Shader shaderText = ResourceManager::GetShader("text");

	cube = new Cube(shaderMvp);
	cubeMap = new CubeMap(shaderCubeMap);
	text = new Text(shaderText, this->width, this->height);
	text->Load("resources/fonts/Inconsolata-Regular.ttf", 16);

	entity = new Entity(glm::vec3(0.0f, 0.0f, 10.0f), glm::vec3(0.0f, 0.0f, -1.0f));

	for(int i = 0; i < 100; i++) {
		this->cubes.push_back(glm::vec3((float) (rand() % 100 - 50), (float) (rand() % 100 - 50), (float) (rand() % 100 - 50)));
	}
}

void Level::ProcessInput(GLfloat dt, GLboolean keys[1024]) {
	if(keys[GLFW_KEY_W] == GL_TRUE) {
		entity->movePositionForward(dt);
	}
	if(keys[GLFW_KEY_S] == GL_TRUE) {
		entity->movePositionBackward(dt);
	}
	if(keys[GLFW_KEY_A] == GL_TRUE) {
		if(keys[GLFW_KEY_LEFT_SHIFT] == GL_TRUE) {
			entity->decreaseRoll(dt);
		} else {
			entity->movePositionLeft(dt);
		}
	}
	if(keys[GLFW_KEY_D] == GL_TRUE) {
		if(keys[GLFW_KEY_LEFT_SHIFT] == GL_TRUE) {
			entity->increaseRoll(dt);
		} else {
			entity->movePositionRight(dt);
		}
	}

	if(keys[GLFW_KEY_UP] == GL_TRUE) {
		entity->decreasePitch(dt);
	}
	if(keys[GLFW_KEY_DOWN] == GL_TRUE) {
		entity->increasePitch(dt);
	}
	if(keys[GLFW_KEY_LEFT] == GL_TRUE) {
		entity->decreaseYaw(dt);
	}
	if(keys[GLFW_KEY_RIGHT] == GL_TRUE) {
		entity->increaseYaw(dt);
	}

	if(keys[GLFW_KEY_C] == GL_TRUE) {
		entity->reset();
	}
}

void Level::Update(GLfloat dt)
{
	entity->updatePosition(dt);
}

void Level::render(GLboolean debugEnabled)
{
	glm::mat4 view = physics::calculateViewMatrix(entity->position, entity->orientation, entity->up);
	glm::mat4 projection = physics::calculateProjectionMatrix(45.0f, (float) this->width / (float) this->height, 0.1f, 1000.0f);

	ResourceManager::GetShader("mvp").Use();
	ResourceManager::GetShader("mvp").SetMatrix4("view", view);
	ResourceManager::GetShader("mvp").SetMatrix4("projection", projection);
	for(auto const& cubePos : this->cubes) {
		cube->render(ResourceManager::GetTexture("kitten"), cubePos, glm::vec3(1.0f, 1.0f, 1.0f));
	}

	//Shader modelShader = ResourceManager::GetShader("model");
	ResourceManager::GetShader("model").Use();
	ResourceManager::GetShader("model").SetMatrix4("view", view);
	ResourceManager::GetShader("model").SetMatrix4("projection", projection);
	ResourceManager::GetShader("model").SetVector3f("viewPos", entity->position);
	ResourceManager::GetShader("model").SetVector3f("directionalLight.direction", -1000.0f, -1000.0f, -1000.0f);
	ResourceManager::GetShader("model").SetVector3f("directionalLight.ambient", 0.25f, 0.25f, 0.25f);
	ResourceManager::GetShader("model").SetVector3f("directionalLight.diffuse", 0.5f, 0.5f, 0.5f);
	ResourceManager::GetShader("model").SetVector3f("directionalLight.specular", 0.5f, 0.5f, 0.5f);
	for(std::vector<std::unique_ptr<ModelContainer>>::size_type i = 0; i != this->models.size(); i++) {
		glm::mat4 modelMatrix;
		glm::mat4 translationMatrix = glm::translate(modelMatrix, this->models[i]->translation);
		glm::mat4 rotationMatrix = glm::toMat4(glm::toQuat(glm::orientate3(this->models[i]->rotation)));
		glm::mat4 scaleMatrix = glm::scale(modelMatrix, this->models[i]->scale);
		modelMatrix = translationMatrix * rotationMatrix * scaleMatrix;
		ResourceManager::GetShader("model").SetMatrix4("model", modelMatrix);
		this->models[i]->model->render(ResourceManager::GetShader("model"));
	}

	if(debugEnabled) {
		renderCoordinates(this->entity);
	}
	cubeMap->render(ResourceManager::GetTexture3D("skybox"), view, projection);
}

void Level::renderCoordinates(Entity* entity) {
	std::stringstream positionStream;
	std::stringstream orientationStream;
	std::stringstream upStream;
	std::stringstream axisStream;
	std::stringstream velocityStream;
	positionStream << std::fixed << std::setprecision(2) << "X: " << entity->position[0] << " Y: " << entity->position[1] << " Z: " << entity->position[2];
	orientationStream << std::fixed << std::setprecision(2) << "or: " << entity->orientation[0] << " " << entity->orientation[1] << " " << entity->orientation[2];
	upStream << std::fixed << std::setprecision(2) << "up: " << entity->up[0] << " " << entity->up[1] << " " << entity->up[2];
	axisStream << std::fixed << std::setprecision(2) << "Roll: " << entity->rollAngle << " Pitch: " << entity->pitchAngle << " Yaw: " << entity->yawAngle;
	velocityStream << std::fixed << std::setprecision(2) << "velocity: " << entity->velocity[0] << " " << entity->velocity[1] << " " << entity->velocity[2];
	text->render(positionStream.str(), 5.0f, 25.0f, 1.0f);
	text->render(orientationStream.str(), 5.0f, 40.0f, 1.0f);
	text->render(upStream.str(), 5.0f, 55.0f, 1.0f);
	text->render(axisStream.str(), 5.0f, 70.0f, 1.0f);
	text->render(velocityStream.str(), 5.0f, 85.0f, 1.0f);
}

Level* parseLevelFile(int id, GLuint width, GLuint height) {
	auto t_start = std::chrono::high_resolution_clock::now();
	float deltaTime = 0.0f;
	
	Level* level = new Level(id, width, height);

	// Model File Parser
	std::stringstream filepath;
	filepath << "resources/levels/level_" << id << ".txt";
	std::ifstream file(filepath.str());
	std::string line;
	while (std::getline(file, line))
	{
		if(line.find("#") == 0) {
			std::cout << "Found a comment...skipping" << std::endl;
			continue;   // Found a comment
		}
		std::istringstream stream(line);
		std::vector<std::string> tokens(std::istream_iterator<std::string>{stream}, std::istream_iterator<std::string>());
		if(tokens[0] == "model") {
			if(tokens.size() != 11) {
				std::cout << "Model line has invalid number of tokens...skipping" << std::endl;
				continue;
			}
			std::cout << "Loading model with name " << tokens[1] << std::endl;
			std::string modelPath = "resources/models/" + tokens[1] + "/" + tokens[1] + ".obj";
			glm::vec3 translation = glm::vec3(::atof(tokens[2].c_str()), ::atof(tokens[3].c_str()), ::atof(tokens[4].c_str()));
			glm::vec3 rotation = glm::vec3(::atof(tokens[5].c_str()), ::atof(tokens[6].c_str()), ::atof(tokens[7].c_str()));
			glm::vec3 scale = glm::vec3(::atof(tokens[8].c_str()), ::atof(tokens[9].c_str()), ::atof(tokens[10].c_str()));
			ModelContainer* modelContainer = new ModelContainer(modelPath, translation, rotation, scale);
			level->models.push_back(std::unique_ptr<ModelContainer>(modelContainer));
		}
	}
	auto t_end = std::chrono::high_resolution_clock::now();
	deltaTime = std::chrono::duration_cast<std::chrono::duration<float>>(t_end - t_start).count();
	std::cout << "Read " << level->models.size() << " models into the level in " << deltaTime << " seconds" << std::endl;
	return level;
}