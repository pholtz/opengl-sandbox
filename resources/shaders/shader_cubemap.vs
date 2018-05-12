#version 400
layout (location = 0) in vec3 position;
uniform mat4 view;
uniform mat4 projection;
out vec3 texcoords;

void main() {
	texcoords = position;
	vec4 pos = projection * view * vec4(position, 1.0);
	// The resulting normalized device coordinates will then always have a z 
	// value equal to 1.0: the maximum depth value. The skybox will as a 
	// result only be rendered wherever there are no objects visible (only 
	// then it will pass the depth test, everything else is in front of 
	// the skybox).

	// We do have to change the depth function a little by setting it to 
	// GL_LEQUAL instead of the default GL_LESS. The depth buffer will be 
	// filled with values of 1.0 for the skybox, so we need to make sure the 
	// skybox passes the depth tests with values less than or equal to the 
	// depth buffer instead of less than.
	gl_Position = pos.xyww;
}
