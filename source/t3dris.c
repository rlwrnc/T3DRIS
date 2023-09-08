#include "t3dris.h"
#include "memory.c"
#include "renderer.c"

#define PI 3.1415926535

#define CAMERA_RADIUS 5.0

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
		fprintf(stderr, "%s:\n", path);
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

void game_initialize(GameMemory *memory, OpenGLFunctions *gl_funcs)
{
	gl = *gl_funcs;
	GameState *state = (GameState *) arena_peek(&memory->permanent);
	*state = (GameState) {
		.model = IDENTITY4, .view = IDENTITY4, .projection = IDENTITY4
	};

	state->program = compile_and_link_shader_program("shaders/vertex.glsl",
			"shaders/fragment.glsl");

	mat4_lookat(state->view, (vec3) {-0.0, -0.0, 6.0}, (vec3) {0.0});
	mat4_perspective(state->projection, 0.125, 1920.0 / 1080.0, 0.1, 100.0);

	// NOTE: don't forget to bind the program when setting uniforms!
	gl.UseProgram(state->program);
	int retval = glGetError();
	state->view_location = gl.GetUniformLocation(state->program, "view");
	retval = glGetError();
	state->projection_location = gl.GetUniformLocation(state->program, "projection");
	retval = glGetError();
	state->view_position_location = gl.GetUniformLocation(state->program, "view_position");
	state->light_position_location = gl.GetUniformLocation(state->program, "light_position");
	gl.UniformMatrix4fv(state->view_location, 1, GL_FALSE, (float *) state->view);
	gl.UniformMatrix4fv(state->projection_location, 1, GL_FALSE, (float *) state->projection);
	gl.Uniform3f(state->light_position_location, 1.0, 1.0, 2.0);
	gl.UseProgram(0);

	initialize_cube(state->program);
	
	glEnable(GL_DEPTH_TEST);
}

static u8 game_board[2][2][2] = {0};

void game_update_and_render(GameMemory *memory, float delta_time)
{
	static float theta = 0.0, phi = PI / 6;
	GameState *state = (GameState *) arena_peek(&memory->permanent);
	/* glClearColor(0.4f, 0.6f, 0.0f, 1.0f); */
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	mat4 view = {0};
	theta += 1.0 * delta_time;
	/* theta = 0; */
	vec3 camera_location = {
		CAMERA_RADIUS * sinf(phi) * sinf(theta),
		CAMERA_RADIUS * cosf(phi),
		CAMERA_RADIUS * sinf(phi) * cosf(theta)
	};
	/* vec3 camera_location = {0, 0, 5}; */
	mat4_lookat(view, camera_location, (vec3) {0});

	game_board[0][0][0] = 1;
	game_board[0][1][0] = 1;
	game_board[1][1][0] = 1;
	game_board[1][1][1] = 1;

	gl.UseProgram(state->program);
	gl.UniformMatrix4fv(state->view_location, 1, GL_FALSE, (float *) view);
	gl.Uniform3fv(state->view_position_location, 1, (float *) camera_location);
	for (int i = 0; i < 2; i++)
		for (int j = 0; j < 2; j++)
			for (int k = 0; k < 2; k++)
				if (game_board[i][j][k] != 0)
					render_cube((vec3) {i, j, k}, 0, (vec3) {1.0, 1.0, 1.0}, (vec4) {1.0, 0.0, 1.0, 1.0});
	gl.UseProgram(0);
}
