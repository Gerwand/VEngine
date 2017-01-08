#version 450 core

layout(location = 6) in VsOut {
	int character;
	vec2 texCoord;
	vec2 texOffset;
} vsOut;

out vec4 color;

layout (binding = 1) uniform sampler2D font;


uniform float fontSize;

void main()
{
	vec2 resultUV;

	resultUV.x = (vsOut.texOffset.x * fontSize) + mod(vsOut.texCoord.x, 1.0) * (tileSize);
	resultUV.y = (vsOut.texOffset.y * fontSize) + mod(vsOut.texCoord.y, 1.0) * (tileSize);

	color = texture(tex1, resultUV);
}