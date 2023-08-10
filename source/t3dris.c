#include "t3dris.h"

OpenGLFunctions gl;

// TODO: move global variables to game memory
static mat4 model = IDENTITY4, view = IDENTITY4, projection = IDENTITY4;
static GLuint vbo, vao = 0, program;
static GLint model_location = 0, view_location = 0, projection_location = 0;
static float vertices[] = {
	-0.5f, -0.5f, -0.5f,
	0.5f, -0.5f, -0.5f,
	0.5f,  0.5f, -0.5f,
	0.5f,  0.5f, -0.5f,
	-0.5f,  0.5f, -0.5f,
	-0.5f, -0.5f, -0.5f,

	-0.5f, -0.5f,  0.5f,
	0.5f, -0.5f,  0.5f,
	0.5f,  0.5f,  0.5f,
	0.5f,  0.5f,  0.5f,
	-0.5f,  0.5f,  0.5f,
	-0.5f, -0.5f,  0.5f,

	-0.5f,  0.5f,  0.5f,
	-0.5f,  0.5f, -0.5f,
	-0.5f, -0.5f, -0.5f,
	-0.5f, -0.5f, -0.5f,
	-0.5f, -0.5f,  0.5f,
	-0.5f,  0.5f,  0.5f,

	0.5f,  0.5f,  0.5f,
	0.5f,  0.5f, -0.5f,
	0.5f, -0.5f, -0.5f,
	0.5f, -0.5f, -0.5f,
	0.5f, -0.5f,  0.5f,
	0.5f,  0.5f,  0.5f,

	-0.5f, -0.5f, -0.5f,
	0.5f, -0.5f, -0.5f,
	0.5f, -0.5f,  0.5f,
	0.5f, -0.5f,  0.5f,
	-0.5f, -0.5f,  0.5f,
	-0.5f, -0.5f, -0.5f,

	-0.5f,  0.5f, -0.5f,
	0.5f,  0.5f, -0.5f,
	0.5f,  0.5f,  0.5f,
	0.5f,  0.5f,  0.5f,
	-0.5f,  0.5f,  0.5f,
	-0.5f,  0.5f, -0.5f
};

inline GLuint compile_shader(const char *path, GLenum type)
{	
	FILE *file = 0;
	fopen_s(&file, path, "rb");
	assert(file);
	assert(fseek(file, 0, SEEK_END) == 0);
	long length = ftell(file);
	rewind(file);
	
	char *source = (char *) malloc(length+1);
	assert(fread(source, sizeof(char), length, file));
	source[length] = 0;

	GLuint shader = gl.CreateShader(type);
	gl.ShaderSource(shader, 1, (const char **) &source, 0);
	gl.CompileShader(shader);

	int success;
	char info_log[512];
	gl.GetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (!success) {
		gl.GetShaderInfoLog(shader, 512, 0, info_log);
		fprintf(stderr, "%s\n", info_log);
	}

	fclose(file);
	free(source);
	return shader;
}

GLuint compile_and_link_shader_program(const char *vertex_path, const char *fragment_path)
{
	GLuint program = gl.CreateProgram();
	GLuint vertex = 0, fragment = 0;
	GLint success = 0;

	if (vertex_path != NULL) {
		vertex = compile_shader(vertex_path, GL_VERTEX_SHADER);
		gl.AttachShader(program, vertex);
	}

	if (fragment_path != NULL) {
		fragment = compile_shader(fragment_path, GL_FRAGMENT_SHADER);
		gl.AttachShader(program, fragment);
	}

	gl.LinkProgram(program);
	gl.GetProgramiv(program, GL_LINK_STATUS, &success);
	if (!success) {
		char info_log[512];
		gl.GetProgramInfoLog(program, 512, 0, info_log);
		fprintf(stderr, "%s\n", info_log);
	}

	if (vertex)
		gl.DeleteShader(vertex);
	if (fragment)
		gl.DeleteShader(fragment);
	return program;
}

void game_initialize(GameMemory *memory)
{
	gl = *((OpenGLFunctions *) memory->permanent);
	/* GameState state = *((GameState *) (memory->permanent + sizeof *gl)); */

	program = compile_and_link_shader_program("shaders/vertex.glsl", "shaders/fragment.glsl");

	mat4 translation, rotation, scale;
	mat4_translate(translation, 0.0, 0.0, 0.0);
	vec3 rotation_axis = {1.0, 1.0, 0.0};
	mat4_rotate(rotation, 1.0 / 8.0, rotation_axis);
	mat4_scale(scale, 2.0, 2.0, 2.0);
	// translation * scale
	mat4_multiply(model, model, translation);
	mat4_multiply(model, model, rotation);
	mat4_multiply(model, model, scale);

	mat4_translate(view, 0.0, 0.0, -3.0);

	mat4_perspective(projection, 0.125, 1920.0 / 1080.0, 0.1, 100.0);

	// NOTE: don't forget to bind the program when setting uniforms!
	gl.UseProgram(program);
	model_location = gl.GetUniformLocation(program, "model");
	view_location = gl.GetUniformLocation(program, "view");
	projection_location = gl.GetUniformLocation(program, "projection");
	gl.UniformMatrix4fv(model_location, 1, GL_FALSE, (float *) model);
	gl.UniformMatrix4fv(view_location, 1, GL_FALSE, (float *) view);
	gl.UniformMatrix4fv(projection_location, 1, GL_FALSE, (float *) projection);
	gl.UseProgram(0);
	
	gl.GenBuffers(1, &vbo);
	gl.BindBuffer(GL_ARRAY_BUFFER, vbo);
	gl.BufferData(GL_ARRAY_BUFFER, sizeof vertices, vertices, GL_STATIC_DRAW);
	
	gl.GenVertexArrays(1, &vao);
	gl.BindVertexArray(vao);
	gl.VertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof (float), (void *) 0);
	gl.EnableVertexAttribArray(0);

	gl.BindBuffer(GL_ARRAY_BUFFER, 0);
	gl.BindVertexArray(0);
}

//
void game_update_and_render(GameMemory *memory)
{
	/* GameState state = *((GameState *) (memory->permanent + sizeof gl)); */
	glClearColor(0.4f, 0.6f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	gl.UseProgram(program);
	gl.BindVertexArray(vao);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	gl.BindVertexArray(0);
	gl.UseProgram(0);
}
