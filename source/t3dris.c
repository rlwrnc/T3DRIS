#include "t3dris.h"
#include "memory.c"
#include "renderer.c"

#define PI 3.1415926535

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

static u8 game_board[10][20][10] = {0};

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
	state->view_location = gl.GetUniformLocation(state->program, "view");
	state->projection_location = gl.GetUniformLocation(state->program, "projection");
	state->view_position_location = gl.GetUniformLocation(state->program, "view_position");
	state->light_position_location = gl.GetUniformLocation(state->program, "light_position");
	gl.UniformMatrix4fv(state->view_location, 1, GL_FALSE, (float *) state->view);
	gl.UniformMatrix4fv(state->projection_location, 1, GL_FALSE, (float *) state->projection);
	gl.Uniform3f(state->light_position_location, 5.0, 15.0, 5.0);
	gl.UseProgram(0);

	initialize_cube(state->program);

	game_board[5][19][4] = 1;
	
	game_board[0][0][0] = 1;
	game_board[0][0][9] = 1;
	game_board[0][19][0] = 1;
	game_board[0][19][9] = 1;
	game_board[9][0][0] = 1;
	game_board[9][0][9] = 1;
	game_board[9][19][0] = 1;
	game_board[9][19][9] = 1;

	glEnable(GL_DEPTH_TEST);
}

void game_update_and_render(GameMemory *memory, float delta_time, InputState *input)
{
	static float theta = PI / 4, phi = PI / 4;
	static float camera_radius = 45.0;
	static i32 play_turn_animation = 0;
	static float new_theta = 0.0;
	static float count = 0.0;
	static u32 p = 19;
	GameState *state = (GameState *) arena_peek(&memory->permanent);
	/* glClearColor(0.4f, 0.6f, 0.0f, 1.0f); */
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	if (input->left && play_turn_animation == 0) {
		play_turn_animation = 1;
		new_theta = theta + (PI / 2);
	}

	if (input->right && play_turn_animation == 0) {
		play_turn_animation = -1;
		new_theta = theta - (PI / 2);
	}

	if (play_turn_animation > 0) {
		theta += 10.0 * delta_time;
		if (theta > new_theta) {
			theta = new_theta;
			play_turn_animation = 0;
		}
	}

	if (play_turn_animation < 0) {
		theta -= 10.0 * delta_time;
		if (theta < new_theta) {
			theta = new_theta;
			play_turn_animation = 0;
		}
	}

	if (input->scroll_down)
		camera_radius += 50.0 * delta_time;
	if (input->scroll_up)
		camera_radius -= 50.0 * delta_time;

	mat4 view = {0};
	vec3 camera_location = {
		camera_radius * sinf(phi) * sinf(theta) + 5.0,
		camera_radius * cosf(phi) + 10.0,
		camera_radius * sinf(phi) * cosf(theta) + 5.0
	};
	mat4_lookat(view, camera_location, (vec3) {5.0, 10.0, 5.0});


	static bool cubes_should_fall = false;

	if (count < 0.5) {
		count += delta_time;
		cubes_should_fall = false;
	} else {
		count -= 0.5;
		cubes_should_fall = true;
	}

	for (int i = 0; i < 10; i++) {
		for (int j = 0; j < 20; j++) {
			for (int k = 0; k < 10; k++) {
				if (cubes_should_fall && j > 0 && game_board[i][j][k] != 0 && game_board[i][j-1][k] == 0) {
					game_board[i][j][k] = 0;
					game_board[i][j-1][k] = 1;
				}
			}
		}
	}

	gl.UseProgram(state->program);
	gl.UniformMatrix4fv(state->view_location, 1, GL_FALSE, (float *) view);
	gl.Uniform3fv(state->view_position_location, 1, (float *) camera_location);
	for (int i = 0; i < 10; i++) {
		for (int j = 0; j < 20; j++) {
			for (int k = 0; k < 10; k++) {
				if (game_board[i][j][k] == 1) {
					// TODO: instance cube render calls?
					// TODO: better tetronimos
					render_cube((vec3) {i, j + 0.5, k}, 0, (vec3) {1.0, 1.0, 1.0}, (vec4) {1.0, 0.0, 1.0, 1.0});
				}
			}
		}
	}
	/* render_cube((vec3) {0}, 0, (vec3) {500.0, 0.0, 500.0}, (vec4) {0.4, 0.6, 0.0, 1.0}); */

	render_cube((vec3) {4.5, -0.5, 4.5}, 0, (vec3) {10.0, 1.0, 10.0}, (vec4) {0.5, 0.5, 0.5, 1.0});
	gl.UseProgram(0);
}
