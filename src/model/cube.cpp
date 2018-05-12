#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "cube.hpp"
#include "resource_manager.hpp"


Cube::Cube(Shader &shader)
{
    this->shader = shader;
    this->initRenderData();
}

Cube::~Cube()
{
    glDeleteVertexArrays(1, &this->vao);
}

void Cube::initRenderData()
{
    float vertices[] = {
    //  X       Y       Z       U       V
        //Front Face
        -0.5f,  0.5f,   0.5f,   0.0f,   1.0f,
        -0.5f,  -0.5f,  0.5f,   0.0f,   0.0f,
        0.5f,   0.5f,   0.5f,   1.0f,   1.0f,
        0.5f,   0.5f,   0.5f,   1.0f,   1.0f,
        0.5f,   -0.5f,  0.5f,   1.0f,   0.0f,
        -0.5f,  -0.5f,  0.5f,   0.0f,   0.0f,

        // Left Face
        -0.5f,  0.5f,   -0.5f,  0.0f,   1.0f,
        -0.5f,  -0.5f,  -0.5f,  0.0f,   0.0f,
        -0.5f,  0.5f,   0.5f,   1.0f,   1.0f,
        -0.5f,  0.5f,   0.5f,   1.0f,   1.0f,
        -0.5f,  -0.5f,  0.5f,   1.0f,   0.0f,
        -0.5f,  -0.5f,  -0.5f,  0.0f,   0.0f,

        // Back Face
        0.5f,   0.5f,   -0.5f,  0.0f,   1.0f,
        0.5f,   -0.5f,  -0.5f,  0.0f,   0.0f,
        -0.5f,  0.5f,   -0.5f,  1.0f,   1.0f,
        -0.5f,  0.5f,   -0.5f,  1.0f,   1.0f,
        -0.5f,  -0.5f,  -0.5f,  1.0f,   0.0f,
        0.5f,   -0.5f,  -0.5f,  0.0f,   0.0f,

        // Right Face
        0.5f,   0.5f,   0.5f,   0.0f,   1.0f,
        0.5f,   -0.5f,  0.5f,   0.0f,   0.0f,
        0.5f,   0.5f,   -0.5f,  1.0f,   1.0f,
        0.5f,   0.5f,   -0.5f,  1.0f,   1.0f,
        0.5f,   -0.5f,  -0.5f,  1.0f,   0.0f,
        0.5f,   -0.5f,  0.5f,   0.0f,   0.0f,

        // Top Face
        -0.5f,  0.5f,   -0.5f,  0.0f,   1.0f,
        -0.5f,  0.5f,   0.5f,   0.0f,   0.0f,
        0.5f,   0.5f,   -0.5f,  1.0f,   1.0f,
        0.5f,   0.5f,   -0.5f,  1.0f,   1.0f,
        0.5f,   0.5f,   0.5f,   1.0f,   0.0f,
        -0.5f,  0.5f,   0.5f,   0.0f,   0.0f,

        // Bottom Face
        -0.5f,  -0.5f,  0.5f,   0.0f,   1.0f,
        -0.5f,  -0.5f,  -0.5f,  0.0f,   0.0f,
        0.5f,   -0.5f,  0.5f,   1.0f,   1.0f,
        0.5f,   -0.5f,  0.5f,   1.0f,   1.0f,
        0.5f,   -0.5f,  -0.5f,  1.0f,   0.0f,
        -0.5f,  -0.5f,  -0.5f,  0.0f,   0.0f
    };
    GLuint vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Configure VAO
    glGenVertexArrays(1, &this->vao);
    glBindVertexArray(this->vao);

    GLint positionAttribute = glGetAttribLocation(this->shader.ID, "position");
    glEnableVertexAttribArray(positionAttribute);
    glVertexAttribPointer(positionAttribute, 3, GL_FLOAT, GL_FALSE, 5*sizeof(float), 0);

    GLint textureAttribute = glGetAttribLocation(this->shader.ID, "textureCoordinates");
    glEnableVertexAttribArray(textureAttribute);
    glVertexAttribPointer(textureAttribute, 2, GL_FLOAT, GL_FALSE, 5*sizeof(float), (void*)(3*sizeof(float)));

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void Cube::render(const Texture2D &texture, glm::vec3 position, glm::vec3 color)
{
    glm::mat4 model;
    model = glm::translate(model, position);

    this->shader.Use();
    this->shader.SetMatrix4("model", model);
    this->shader.SetVector3f("customColor", color);

    glActiveTexture(GL_TEXTURE0);
    texture.Bind();

    glBindVertexArray(this->vao);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);
}
