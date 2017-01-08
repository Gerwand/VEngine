#version 450 core


layout(location = 6) in VsOut {
	vec4 color;
	vec2 texCoord;
	vec3 normal;
} vsOut;

out vec4 color;

void main()
{
	color = vsOut.color;
}