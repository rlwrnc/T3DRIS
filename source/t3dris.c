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

typedef struct {
	u8 *space[8];
	u8 *one;
	u8 *two;
	u8 *three;
	u8 *four;
} Tetronimo;
Tetronimo tetronimo;

void game_initialize(GameMemory *memory, OpenGLFunctions *gl_funcs)
{
	gl = *gl_funcs;
	GameState *state = (GameState *) arena_peek(&memory->permanent);
	*state = (GameState) {
		.model = IDENTITY4, .view = IDENTITY4, .projection = IDENTITY4
	};

	state->program = compile_and_link_shader_program("shaders/vertex.glsl",
			"shaders/fragment.glsl");

	state->camera = (Camera) {
		.theta = CAMERA_INITIAL_THETA, .target_theta = CAMERA_INITIAL_THETA,
		.phi = CAMERA_INITIAL_THETA, .radius = 45.0,
		.rotation_speed = 10.0, .rotation_step = 1.57079632679
	};
	state->camera.position[0] = 45.0 * sinf(CAMERA_INITIAL_THETA) * sinf(CAMERA_INITIAL_THETA) + 4.5;
	state->camera.position[1] = 45.0 * cosf(CAMERA_INITIAL_THETA) + 10.0;
	state->camera.position[2] = 45.0 * sinf(CAMERA_INITIAL_THETA) * cosf(CAMERA_INITIAL_THETA) + 4.5;

	mat4_lookat(state->view, (vec3) {-0.0, -0.0, 6.0}, (vec3) {0.0});
	mat4_perspective(state->projection, 0.125, 1920.0 / 1080.0, 1.0, 100.0);

	// NOTE: don't forget to bind the program when setting uniforms!
	gl.UseProgram(state->program);
	state->view_location = gl.GetUniformLocation(state->program, "view");
	state->projection_location = gl.GetUniformLocation(state->program, "projection");
	state->view_position_location = gl.GetUniformLocation(state->program, "view_position");
	state->light_position_location = gl.GetUniformLocation(state->program, "light_position");
	gl.UniformMatrix4fv(state->view_location, 1, GL_FALSE, (float *) state->view);
	gl.UniformMatrix4fv(state->projection_location, 1, GL_FALSE, (float *) state->projection);
	gl.Uniform3f(state->light_position_location, 4.5, 25.0, 4.5);
	gl.UseProgram(0);

	initialize_cube(state->program);

	tetronimo.space[0] = &game_board[4][19][4];
	tetronimo.space[1] = &game_board[4][19][5];
	tetronimo.space[2] = &game_board[5][19][4];
	tetronimo.space[3] = &game_board[5][19][5];
	tetronimo.space[4] = &game_board[4][18][4];
	tetronimo.space[5] = &game_board[4][18][5];
	tetronimo.space[6] = &game_board[5][18][4];
	tetronimo.space[7] = &game_board[5][18][5];

	game_board[4][19][4] = 1;
	game_board[5][19][4] = 1;
	game_board[5][19][5] = 1;
	game_board[4][19][5] = 1;
	
	/* game_board[0][0][0] = 1; */
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
	static float count = 0.0;
	GameState *state = (GameState *) arena_peek(&memory->permanent);
	/* glClearColor(0.4f, 0.6f, 0.0f, 1.0f); */
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	/* if (input->rotate_cw && play_turn_animation == 0) { */
	/* 	play_turn_animation = 1; */
	/* 	new_theta = theta + (PI / 2); */
	/* } */
	/**/
	/* if (input->rotate_ccw && play_turn_animation == 0) { */
	/* 	play_turn_animation = -1; */
	/* 	new_theta = theta - (PI / 2); */
	/* } */
	/**/
	/* if (play_turn_animation > 0) { */
	/* 	theta += 10.0 * delta_time; */
	/* 	if (theta > new_theta) { */
	/* 		theta = new_theta; */
	/* 		play_turn_animation = 0; */
	/* 	} */
	/* } */
	/**/
	/* if (play_turn_animation < 0) { */
	/* 	theta -= 10.0 * delta_time; */
	/* 	if (theta < new_theta) { */
	/* 		theta = new_theta; */
	/* 		play_turn_animation = 0; */
	/* 	} */
	/* } */

	if (input->rotate_cw && state->camera.rotation_direction == 0) {
		state->camera.rotation_direction = 1;
		state->camera.target_theta += state->camera.rotation_step;
	}

	if (input->rotate_ccw && state->camera.rotation_direction == 0) {
		state->camera.rotation_direction = -1;
		state->camera.target_theta -= state->camera.rotation_step;
	}

	if (state->camera.rotation_direction != 0)
		camera_rotate(&state->camera, delta_time);

	if (input->scroll_down)
		state->camera.radius += 50.0 * delta_time;
	if (input->scroll_up)
		state->camera.radius += 50.0 * delta_time;

	mat4 view = {0};
	mat4_lookat(view, state->camera.position, (vec3) {4.5, 10.0, 4.5});


	static bool cubes_should_fall = false;

	if (count < 0.5) {
		count += delta_time;
		cubes_should_fall = false;
	} else {
		count -= 0.5;
		cubes_should_fall = true;
	}

	/* for (int i = 0; i < 10; i++) { */
	/* 	for (int j = 0; j < 20; j++) { */
	/* 		for (int k = 0; k < 10; k++) { */
	/* 			if (cubes_should_fall && j > 0 && game_board[i][j][k] != 0 && game_board[i][j-1][k] == 0) { */
	/* 				game_board[i][j][k] = 0; */
	/* 				game_board[i][j-1][k] = 1; */
	/* 			} */
	/* 		} */
	/* 	} */
	/* } */

	gl.UseProgram(state->program);
	gl.UniformMatrix4fv(state->view_location, 1, GL_FALSE, (float *) view);
	gl.Uniform3fv(state->view_position_location, 1, (float *) state->camera.position);
	for (int i = 0; i < 10; i++) {
		for (int j = 0; j < 20; j++) {
			for (int k = 0; k < 10; k++) {
				if (game_board[i][j][k] == 1) {
					// TODO: instance cube render calls?
					render_cube((vec3) {i, j + 0.5, k}, 0, (vec3) {1.0, 1.0, 1.0}, (vec4) {1.0, 0.0, 1.0, 1.0});
				}
			}
		}
	}
	/* render_cube((vec3) {0}, 0, (vec3) {500.0, 0.0, 500.0}, (vec4) {0.4, 0.6, 0.0, 1.0}); */

	static float angle = 0.0;
	angle += 0.25 * delta_time;
	static quat r;
	quat_rotate(r, angle, (vec3) {0.0, 0.0, 1.0});
	render_cube((vec3) {4.5, 5.0, 4.5}, r, (vec3) {20.0, 1.0, 2.0}, (vec4) {1.0, 1.0, 1.0, 1.0});

	render_cube((vec3) {4.5, -0.5, 4.5}, 0, (vec3) {10.0, 1.0, 10.0}, (vec4) {0.5, 0.5, 0.5, 1.0});
	render_cube((vec3) {0, 0.5, 0}, 0, 0, (vec4) {1, 0, 0, 1});
	gl.UseProgram(0);
}
