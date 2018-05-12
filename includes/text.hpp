/*******************************************************************
** This code is part of Breakout.
**
** Breakout is free software: you can redistribute it and/or modify
** it under the terms of the CC BY 4.0 license as published by
** Creative Commons, either version 4 of the License, or (at your
** option) any later version.
******************************************************************/
#ifndef TEXT_H
#define TEXT_H

#include <GL/glew.h>
#include <map>
#include <iostream>
#include <glm/glm.hpp>
#include "shader.hpp"


/// Holds all state information relevant to a character as loaded using FreeType
struct Character {
    GLuint TextureID;   // ID handle of the glyph texture
    glm::ivec2 Size;    // Size of glyph
    glm::ivec2 Bearing; // Offset from baseline to left/top of glyph
    GLuint Advance;     // Horizontal offset to advance to next glyph
};


class Text
{
public:
    std::map<GLchar, Character> Characters; 
    Shader TextShader;
    Text(Shader &shader, GLuint width, GLuint height);
    void Load(std::string font, GLuint fontSize);
    void render(std::string text, GLfloat x, GLfloat y, GLfloat scale, glm::vec3 color = glm::vec3(1.0f));
private:
    GLuint VAO, VBO;
};

#endif 