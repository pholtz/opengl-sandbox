/*******************************************************************
** This code is part of Breakout.
**
** Breakout is free software: you can redistribute it and/or modify
** it under the terms of the CC BY 4.0 license as published by
** Creative Commons, either version 4 of the License, or (at your
** option) any later version.
******************************************************************/
#include "resource_manager.hpp"
#include <string>
#include <iostream>
#include <sstream>
#include <fstream>
#include <SOIL.h>
#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <memory>
#include "shader.hpp"
#include "texture.hpp"
#include "texture_3d.hpp"


// Instantiate static variables
std::map<std::string, Texture2D>    ResourceManager::Textures;
std::map<std::string, Shader>       ResourceManager::Shaders;
std::map<std::string, Texture3D>    ResourceManager::Textures3D;


Shader ResourceManager::LoadShader(const GLchar *vShaderFile, const GLchar *fShaderFile, const GLchar *gShaderFile, std::string name)
{
    if(Shaders.find(name) != Shaders.end()) {
        std::cout << "Warning! Overwriting mapped shader with key " << name << std::endl;
    }
    Shaders[name] = loadShaderFromFile(vShaderFile, fShaderFile, gShaderFile);
    return Shaders[name];
}

Shader ResourceManager::GetShader(std::string name)
{
    return Shaders[name];
}

Texture2D ResourceManager::LoadTexture(const GLchar *file, GLboolean alpha, std::string name)
{
    if(Textures.find(name) != Textures.end()) {
        std::cout << "Warning! Overwriting mapped texture with key " << name << std::endl;
    }
    Textures[name] = loadTextureFromFile(file, alpha);
    return Textures[name];
}

Texture2D ResourceManager::GetTexture(std::string name)
{
    return Textures[name];
}

Texture3D ResourceManager::LoadTexture3D(const GLchar *file, std::string name)
{
    if(Textures3D.find(name) != Textures3D.end()) {
        std::cout << "Warning! Overwriting mapped 3D texture with key " << name << std::endl;
    }
    Textures3D[name] = loadTexture3DFromFile(file);
    return Textures3D[name];
}

Texture3D ResourceManager::GetTexture3D(std::string name)
{
    return Textures3D[name];
}

void ResourceManager::Clear()
{
    // (Properly) delete all shaders	
    for (auto iter : Shaders) {
        glDeleteProgram(iter.second.ID);
    }
    // (Properly) delete all textures
    for (auto iter : Textures) {
        glDeleteTextures(1, &iter.second.ID);
    }
    for(auto iter : Textures3D) {
        glDeleteTextures(1, &iter.second.ID);
    }
}

Shader ResourceManager::loadShaderFromFile(const GLchar *vShaderFile, const GLchar *fShaderFile, const GLchar *gShaderFile)
{
    // 1. Retrieve the vertex/fragment source code from filePath
    std::string vertexCode;
    std::string fragmentCode;
    std::string geometryCode;
    try
    {
        // Open files
        std::ifstream vertexShaderFile(vShaderFile);
        std::ifstream fragmentShaderFile(fShaderFile);
        std::stringstream vShaderStream, fShaderStream;
        // Read file's buffer contents into streams
        vShaderStream << vertexShaderFile.rdbuf();
        fShaderStream << fragmentShaderFile.rdbuf();
        // close file handlers
        vertexShaderFile.close();
        fragmentShaderFile.close();
        // Convert stream into string
        vertexCode = vShaderStream.str();
        fragmentCode = fShaderStream.str();
        // If geometry shader path is present, also load a geometry shader
        if (gShaderFile != nullptr)
        {
            std::ifstream geometryShaderFile(gShaderFile);
            std::stringstream gShaderStream;
            gShaderStream << geometryShaderFile.rdbuf();
            geometryShaderFile.close();
            geometryCode = gShaderStream.str();
        }
    }
    catch (std::exception e)
    {
        std::cout << "ERROR::SHADER: Failed to read shader files" << std::endl;
    }
    const GLchar *vShaderCode = vertexCode.c_str();
    const GLchar *fShaderCode = fragmentCode.c_str();
    const GLchar *gShaderCode = geometryCode.c_str();
    // 2. Now create shader object from source code
    Shader shader;
    shader.Compile(vShaderCode, fShaderCode, gShaderFile != nullptr ? gShaderCode : nullptr);
    return shader;
}

Texture2D ResourceManager::loadTextureFromFile(const GLchar *file, GLboolean alpha)
{
    // Create Texture object
    Texture2D texture;
    if (alpha)
    {
        texture.Internal_Format = GL_RGBA;
        texture.Image_Format = GL_RGBA;
    }
    // Load image
    int width, height;
    unsigned char* image = SOIL_load_image(file, &width, &height, 0, texture.Image_Format == GL_RGBA ? SOIL_LOAD_RGBA : SOIL_LOAD_RGB);
    // Now generate texture
    texture.Generate(width, height, image);
    // And finally free image data
    SOIL_free_image_data(image);
    return texture;
}

Texture3D ResourceManager::loadTexture3DFromFile(const GLchar *file)
{
    Texture3D texture;
    int width, height;
    unsigned char* image = SOIL_load_image(file, &width, &height, 0, SOIL_LOAD_RGBA);
    texture.Generate(width, height, image);
    SOIL_free_image_data(image);
    return texture;
}
