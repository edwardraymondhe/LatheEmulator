#version 330 core
layout (location = 7) in vec3 aNormal;
layout (location = 8) in vec2 aTexCoord;
layout (location = 9) in vec3 aPos;

out vec3 FragPos;
out vec3 Normal;

out vec2 TexCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 transform;


void main()
{
	FragPos = vec3(model * vec4(aPos, 1.0));
	Normal = mat3(transpose(inverse(model))) * aNormal;

	// gl_Position = projection * view * vec4(FragPos, 1.0); // Before skybox

	gl_Position = projection * view * model * vec4(aPos, 1.0); // After skybox

	TexCoord = aTexCoord;
}