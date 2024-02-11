#version 430 core
// Change to 410 for macOS

layout(location=0) out vec4 out_color;
 
in vec4 vertexColor; // Now interpolated across face
in vec2 interUV;

uniform sampler2D quadTexture;

void main()
{	
	// Just output interpolated color
	vec3 texColor = vec3(texture(quadTexture, interUV));
	out_color = vec4(texColor, 1.0f);
}
