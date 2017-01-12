#version 450 core


layout(location = 6) in VsOut {
	vec4 color;
	vec2 texCoord;
	vec2 texOffset;
	vec3 normal;
} vsOut;


out vec4 color;

uniform float tileSize;

uniform bool wired;
uniform vec3 wireColor;

uniform bool textured;
// Texture samplers
uniform sampler2D tex1;

uniform vec3 globalLightDir; 

uniform vec3 ambientLightColor;
uniform float ambientStrength;

void main()
{
	vec4 ambient = vec4(ambientStrength * ambientLightColor, 1.0);
	float diff = max(dot(vsOut.normal, globalLightDir), 0.0);
    vec4 diffuse = vec4(diff * ambientLightColor, 1.0);


	if (wired) {
		color = vec4(wireColor, 1.0f);
	}
	else if (textured) {
		vec2 resultUV;
		resultUV.x = (vsOut.texOffset.x * tileSize) + mod(vsOut.texCoord.x, 1.0) * (tileSize);
		resultUV.y = (vsOut.texOffset.y * tileSize) + mod(vsOut.texCoord.y, 1.0) * (tileSize);

		color = (ambient + diffuse) * texture(tex1, resultUV);
	}
	else {
		color = (ambient + diffuse) *  vsOut.color;
	}
}