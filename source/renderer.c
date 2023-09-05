#pragma once

#include "t3dris.h"

static float cube_vertices[] = {
	0.5, 0.5, 0.5,
	0.5, -0.5, 0.5,
	-0.5, -0.5, 0.5,
	-0.5, 0.5, 0.5,

	0.5, 0.5, -0.5,
	0.5, -0.5, -0.5,
	-0.5, -0.5, -0.5,
	-0.5, 0.5, -0.5
};

static GLuint cube_vbo, cube_vao, cube_ebo;
static GLuint model_location;

void initialize_cube(GLuint shader)
{
	model_location = gl.GetUniformLocation(shader, "model");

	gl.GenVertexArrays(1, &cube_vao);
	gl.BindVertexArray(cube_vao);

	gl.GenBuffers(1, &cube_vbo);
	gl.BindBuffer(GL_ARRAY_BUFFER, cube_vbo);
	gl.BufferData(GL_ARRAY_BUFFER, sizeof cube_vertices, cube_vertices, GL_DYNAMIC_DRAW);

	gl.VertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof (float), (void *) 0);
	gl.EnableVertexAttribArray(0);

	gl.BindBuffer(GL_ARRAY_BUFFER, 0);
	gl.BindVertexArray(0);
}

// TODO: dynamic cube color
void render_cube(vec3 position, quat rotation, vec3 scale)
{
	mat4 model = IDENTITY4;
	mat4 translation, rotation_transform, scale_transform;
	
	if (scale == 0) {
		mat4_identity(scale_transform);
	}

	mat4_translate(translation, position[0], position[1], position[2]);
	quat_to_mat4(rotation_transform, rotation);
	mat4_scale(scale_transform, scale[0], scale[1], scale[2]);
	mat4_multiply(model, model, translation);
	mat4_multiply(model, model, rotation_transform);
	mat4_multiply(model, model, scale_transform);

	gl.UniformMatrix4fv(model_location, 1, GL_FALSE, (float *) model);
	gl.BindVertexArray(cube_vao);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	gl.BindVertexArray(0);
}
