#version 430 core
// Change to 410 for macOS

layout(location=0) in vec3 position;
layout(location=1) in vec2 texcoords;

out vec4 vertexColor;
out vec2 interUV;

void main()
{		
	// Get position of vertex (object space)
	vec4 objPos = vec4(position, 1.0);

	// For now, just pass along vertex position (no transformations)
	gl_Position = objPos;

	// Output UV coordinates
	interUV = texcoords;
}
