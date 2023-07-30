#include "t3dris.h"

GLuint compile_and_link_shader_program(const char *vertex_path, const char *fragment_path)
{
	GLuint program = glCreateProgram();
	GLuint vertex = 0, fragment = 0;

	if (vertex_path == NULL) {
		
	}
	return program;
}

void game_initialize(GameMemory *memory)
{
	gladLoadGL(memory->glfwGetProcAddress);
}

void game_update_and_render(GameMemory *memory)
{
	printf("hello mom!\n");
#if 0
	glClearColor(0.4f, 0.6f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	static float vertices[] = {
		-0.5, -0.5, 0.0,
		0.0, 0.5, 0.0,
		0.5, 0.5, 0.0
	};

	GLuint vbo;
	glCreateBuffers(1, &vbo);
	glNamedBufferStorage(vbo, 9 * sizeof (float), vertices, 0);

	GLuint vao;
	glCreateVertexArrays(1, &vao);
	glVertexArrayAttribBinding(vao, 0, 0);
	glVertexArrayVertexBuffer(vao, 0, vbo, 0, 3 * sizeof (float));
	glVertexArrayAttribFormat(vao, 0, 3, GL_FLOAT, GL_FALSE, 0);
#endif
}
