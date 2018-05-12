#version 150 core
in vec2 TextureCoordinates;
out vec4 color;
uniform sampler2D image;
uniform vec3 customColor;
void main()
{
	color = vec4(customColor, 1.0) * texture(image, TextureCoordinates).rgb;
}