#ifndef CUBE_H
#define CUBE_H

#include "texture.hpp"
#include "shader.hpp"


class Cube
{
public:
    // Constructor (inits shaders/shapes)
    Cube(Shader &shader);
    // Destructor
    ~Cube();
    // Renders a defined quad textured with given sprite
    void render(const Texture2D &texture, glm::vec3 position, glm::vec3 color);
private:
    // Render state
    Shader shader;
    GLuint vao;
    // Initializes and configures the quad's buffer and vertex attributes
    void initRenderData();
};

#endif