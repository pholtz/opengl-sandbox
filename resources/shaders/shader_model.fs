#version 330 core

struct DirectionalLight {
    vec3 direction;
    vec3 color;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

in VS_OUT {
	vec3 FragPos;
	vec2 TexCoords;
	mat3 TBN;
	vec3 TangentViewPos;
	vec3 TangentFragPos;
} fsi;
out vec4 FragColor;

uniform DirectionalLight directionalLight;
uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;
uniform sampler2D texture_normal1;
uniform sampler2D texture_height1;

void main()
{
	vec4 diffuse1 = texture(texture_diffuse1, fsi.TexCoords);
	vec4 specular1 = texture(texture_specular1, fsi.TexCoords);
	vec4 normal1 = texture(texture_normal1, fsi.TexCoords);
	vec4 height1 = texture(texture_height1, fsi.TexCoords);

	// Normal calculations
	vec3 normal = texture(texture_normal1, fsi.TexCoords).rgb;
	normal = normalize(normal * 2.0 - 1.0);   
	normal = normalize(fsi.TBN * normal);

	vec3 ambientComponent = directionalLight.ambient * diffuse1.rgb;
	vec3 diffuseComponent = max(dot(normal, normalize(-directionalLight.direction)), 0.0) * directionalLight.diffuse * diffuse1.rgb;
	// vec3 viewDir = normalize(viewPos - FragPos);
	// vec3 reflectDir = reflect(-directionalLight.direction, normal1.xyz);  
	// float spec = pow(max(dot(viewDir, reflectDir), 0.0), 1);
	// vec3 specularComponent = directionalLight.specular * spec;

	//vec4 texMix = mix(diffuse1, specular1, 1.0);
	//texMix.xyz = texMix.xyz * (ambientComponent + diffuseComponent);
	vec3 result = ambientComponent + diffuseComponent;
    FragColor = vec4(result, 1.0);
}