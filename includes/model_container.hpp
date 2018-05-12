#ifndef MODEL_CONTAINER_HPP
#define MODEL_CONTAINER_HPP

#include <glm/glm.hpp>
#include <string>
#include <vector>
#include "model.hpp"


class ModelContainer
{
public:
    Model* model;
    glm::vec3 translation;
    glm::vec3 rotation;
    glm::vec3 scale;
    ModelContainer(std::string modelPath, glm::vec3 translation, glm::vec3 rotation, glm::vec3 scale) {
        this->model = new Model(modelPath);
        this->translation = translation;
        this->rotation = rotation;
        this->scale = scale;
    }
};
#endif