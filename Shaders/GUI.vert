#version 450 core


layout (location = 0) in vec3 position;
layout (location = 3) in vec4 color;

layout(location = 6) out VsOut {
	vec4 color;
} vsOut;

uniform mat4 projection;

out gl_PerVertex
{
  vec4 gl_Position;
};

void main()
{
   gl_Position = projection * vec4(position, 1.0);

   vsOut.color = color;
}
