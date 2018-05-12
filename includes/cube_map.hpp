#ifndef CUBE_MAP_H
#define CUBE_MAP_H
#include <GL/glew.h>
#include <glm/glm.hpp>
#include "texture_3d.hpp"
#include "shader.hpp"


class CubeMap
{
public:
	CubeMap(Shader &shader);
	~CubeMap();
	void render(const Texture3D &texture, glm::mat4 view, glm::mat4 projection);
private:
	Shader shader;
	GLuint vao;
	GLuint* tex_cube;
	void initRenderData();
};
#endif