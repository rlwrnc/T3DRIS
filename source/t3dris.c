#include "t3dris.h"

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

	GLuint shader = glCreateShader(type);
	glShaderSource(shader, 1, (const char **) &source, 0);
	glCompileShader(shader);

	int success;
	char info_log[512];
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(shader, 512, 0, info_log);
		fprintf(stderr, "%s\n", info_log);
	}

	fclose(file);
	free(source);
	return shader;
}

GLuint compile_and_link_shader_program(const char *vertex_path, const char *fragment_path)
{
	GLuint program = glCreateProgram();
	GLuint vertex = 0, fragment = 0;
	GLint success = 0;

	if (vertex_path != NULL) {
		vertex = compile_shader(vertex_path, GL_VERTEX_SHADER);
		glAttachShader(program, vertex);
	}

	if (fragment_path != NULL) {
		fragment = compile_shader(fragment_path, GL_FRAGMENT_SHADER);
		glAttachShader(program, fragment);
	}

	glLinkProgram(program);
	glGetProgramiv(program, GL_LINK_STATUS, &success);
	if (!success) {
		char info_log[512];
		glGetProgramInfoLog(program, 512, 0, info_log);
		fprintf(stderr, "%s\n", info_log);
	}

	return program;
}

void game_initialize(GameMemory *memory)
{
	gladLoadGL(memory->glfwGetProcAddress);
}

void game_update_and_render(GameMemory *memory)
{
	glClearColor(0.4f, 0.6f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	GLuint program = compile_and_link_shader_program("shaders/vertex.glsl", "shaders/fragment.glsl");

	static float vertices[] = {
		-0.5, 0.5, 0.0,
		0.5, 0.5,  0.0,
		0.5, -0.5, 0.0,

		0.5, -0.5,  0.0,
		-0.5, -0.5, 0.0,
		-0.5, 0.5,  0.0
	};

	GLuint vbo;
	glCreateBuffers(1, &vbo);
	glNamedBufferStorage(vbo, sizeof vertices, vertices, 0);

	GLuint vao;
	glCreateVertexArrays(1, &vao);
	glVertexArrayAttribBinding(vao, 0, 0);
	glVertexArrayVertexBuffer(vao, 0, vbo, 0, 3 * sizeof (float));
	glVertexArrayAttribFormat(vao, 0, 3, GL_FLOAT, GL_FALSE, 0);
	glEnableVertexArrayAttrib(vao, 0);

	glUseProgram(program);
	glBindVertexArray(vao);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);
	glUseProgram(0);
}
