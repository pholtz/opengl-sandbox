/*******************************************************************
** This code is part of Breakout.
**
** Breakout is free software: you can redistribute it and/or modify
** it under the terms of the CC BY 4.0 license as published by
** Creative Commons, either version 4 of the License, or (at your
** option) any later version.
******************************************************************/
#ifndef RECTANGLE_H
#define RECTANGLE_H

#include <GL/glew.h>
#include "texture.hpp"
#include "shader.hpp"


class Rectangle
{
public:
    Rectangle(Shader &shader);
    ~Rectangle();
    void render(const Texture2D &texture, glm::vec3 position, glm::vec3 color);
private:
    Shader shader;
    GLuint quadVAO;
    void initRenderData();
};
#endif