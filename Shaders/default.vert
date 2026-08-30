#version 330 core

// Positions/Coordinates
layout (location = 0) in vec3 aPos;
// Normals (not normalized)
layout (location = 1) in vec3 aNormal;
// Colors
layout (location = 2) in vec3 aColor;
// Texture Coordinates
layout (location = 3) in vec2 aTex;

// current position for the Fragment Shader
out vec3 currentPos;
// normal for the Fragment Shader
out vec3 Normal;
// Outputs the color for the Fragment Shader
out vec3 color;
// texture coordinates for the Fragment Shader
out vec2 texCoord;

// camera matrix
uniform mat4 camMatrix;

// transformation model
uniform mat4 model;


void main() {
	// calculates current position
	currentPos = vec3(model * vec4(aPos, 1.0f));

	// Pass to Fragment Shader
	Normal = mat3(transpose(inverse(model))) * aNormal; // We need to do some strange transformation to the normals to make them work
	color = aColor;
	texCoord = aTex;

	// Outputs the position
	gl_Position = camMatrix * vec4(currentPos, 1.0);
}