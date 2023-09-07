#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 vertex_normal;

out vec3 fragment_position;
out vec3 normal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	gl_Position = projection * view * model * vec4(position, 1.0);
	fragment_position = vec3(model * vec4(position, 1.0));
	normal = vertex_normal;
}
