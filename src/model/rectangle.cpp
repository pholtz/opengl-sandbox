/*******************************************************************
** This code is part of Breakout.
**
** Breakout is free software: you can redistribute it and/or modify
** it under the terms of the CC BY 4.0 license as published by
** Creative Commons, either version 4 of the License, or (at your
** option) any later version.
******************************************************************/
#include "rectangle.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "resource_manager.hpp"


Rectangle::Rectangle(Shader &shader)
{
    this->shader = shader;
    this->initRenderData();
}

Rectangle::~Rectangle()
{
    glDeleteVertexArrays(1, &this->quadVAO);
}

void Rectangle::initRenderData()
{
    // Configure VBO
    GLuint VBO;
    GLfloat vertices[] = { 
        // Pos            // Tex
        0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
        1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 

        0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
        1.0f, 1.0f, 0.0f, 1.0f, 1.0f,
        1.0f, 0.0f, 0.0f, 1.0f, 0.0f
    };
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Configure VAO
    glGenVertexArrays(1, &this->quadVAO);
    glBindVertexArray(this->quadVAO);

    GLint positionAttribute = glGetAttribLocation(this->shader.ID, "position");
    glEnableVertexAttribArray(positionAttribute);
    glVertexAttribPointer(positionAttribute, 3, GL_FLOAT, GL_FALSE, 5*sizeof(float), 0);

    GLint textureAttribute = glGetAttribLocation(this->shader.ID, "textureCoordinates");
    glEnableVertexAttribArray(textureAttribute);
    glVertexAttribPointer(textureAttribute, 2, GL_FLOAT, GL_FALSE, 5*sizeof(float), (void*)(3*sizeof(float)));

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void Rectangle::render(const Texture2D &texture, glm::vec3 position, glm::vec3 color)
{
    glm::mat4 model;
    model = glm::translate(model, position);

    this->shader.Use();
    this->shader.SetMatrix4("model", model);
    this->shader.SetVector3f("customColor", color);

    glActiveTexture(GL_TEXTURE0);
    texture.Bind();

    glBindVertexArray(this->quadVAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
}
