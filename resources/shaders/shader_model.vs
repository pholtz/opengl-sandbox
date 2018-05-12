#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;
layout (location = 3) in vec3 aTangent;
layout (location = 4) in vec3 aBitangent;

out VS_OUT {
	vec3 FragPos;
	vec2 TexCoords;
	mat3 TBN;
	vec3 TangentViewPos;
	vec3 TangentFragPos;
} vso;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform vec3 viewPos;

void main()
{
	vso.FragPos = vec3(model * vec4(aPos, 1.0));
	vso.TexCoords = aTexCoords;
	vec3 T = normalize(vec3(model * vec4(aTangent,   0.0)));
	vec3 B = normalize(vec3(model * vec4(aBitangent, 0.0)));
	vec3 N = normalize(vec3(model * vec4(aNormal,    0.0)));
	vso.TBN = transpose(mat3(T, B, N));
	vso.TangentViewPos = vso.TBN * viewPos;
	vso.TangentFragPos = vso.TBN * vec3(model * vec4(aPos, 1.0));
    gl_Position = projection * view * model * vec4(aPos, 1.0);
}