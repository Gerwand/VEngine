#version 450 core


layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normals;
layout (location = 2) in vec2 texCoord;
layout (location = 3) in vec3 color;
layout (location = 4) in uvec2 texOffset;

layout(location = 6) out VsOut {
	vec4 color;
	vec2 texCoord;
	vec2 texOffset;
	vec3 normal;
} vsOut;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out gl_PerVertex
{
  vec4 gl_Position;
};

void main()
{
   gl_Position = projection * view * model * vec4(position, 1.0);
   //gl_Position = vec4(position, 1.0);
   vsOut.color = vec4(color, 1.0);
   vsOut.texCoord = texCoord;
   vsOut.texOffset = texOffset;

   vsOut.normal = mat3(transpose(inverse(model))) * normals;  
}
