#include "t3dris.h"
#include "memory.c"
#include "renderer.c"
#include "rules.c"

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

#define GAME_BOARD_XMAX 10
#define GAME_BOARD_YMAX 20
#define GAME_BOARD_ZMAX 10
// TODO: move to game memory
static u8 game_board[GAME_BOARD_XMAX][GAME_BOARD_YMAX][GAME_BOARD_ZMAX] = {0};

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

	game_board[4][19][4] = 1;
	game_board[5][19][4] = 1;
	game_board[5][19][5] = 1;
	game_board[4][19][5] = 1;
	
	/* game_board[0][0][0] = 1; */
	/* game_board[0][0][9] = 1; */
	game_board[0][19][0] = 1;
	game_board[0][19][9] = 1;
	/* game_board[9][0][0] = 1; */
	/* game_board[9][0][9] = 1; */
	game_board[9][19][0] = 1;
	game_board[9][19][9] = 1;

	glEnable(GL_DEPTH_TEST);
}

// NOTE: A note on coordinate systems:
// `NORTH` refers to the negative-z direction
// `EAST` refers to the positive-x direction
// `WEST` and `SOUTH` are the opposite of EAST and NORTH, respectively

void game_update_and_render(GameMemory *memory, float delta_time, InputState *input)
{
	static float count = 0.0;
	static SquareTetronimo current_tetronimo = {
		.position = {5.5, 1.0, 5.5},
		.active = {{5, 0, 5}, {5, 0, 6}, {6, 0, 5}, {6, 0, 6}}
	};
	GameState *state = (GameState *) arena_peek(&memory->permanent);
	/* glClearColor(0.4f, 0.6f, 0.0f, 1.0f); */
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	if (input->rotate_cw && state->camera.rotation_direction == 0) {
		state->camera.orientation = (state->camera.orientation + 1) % 4;
		state->camera.rotation_direction = 1;
		state->camera.target_theta += state->camera.rotation_step;
	}

	if (input->rotate_ccw && state->camera.rotation_direction == 0) {
		state->camera.orientation -= 1;
		if (state->camera.orientation < 0)
			state->camera.orientation = CAM_NE;
		state->camera.rotation_direction = -1;
		state->camera.target_theta -= state->camera.rotation_step;
	}

	if (state->camera.rotation_direction != 0)
		camera_rotate(&state->camera, delta_time);

	mat4 view = {0};
	mat4_lookat(view, state->camera.position, (vec3) {4.5, 10.0, 4.5});

	game_board[5][5][5] = 1;
	game_board[5][5][6] = 1;
	game_board[6][4][5] = 1;
	game_board[6][4][6] = 1;

	static float time_until_next_move = 0.0;
	if (time_until_next_move <= 0) {
		Movements movement = NO_MOVE;
		if (input->up) {
			if (state->camera.orientation == CAM_NW) {
				movement = NORTH;
			}

			if (state->camera.orientation == CAM_SW) {
				movement = WEST;
			}

			if (state->camera.orientation == CAM_SE) {
				movement = SOUTH;
			}

			if (state->camera.orientation == CAM_NE) {
				movement = EAST;
			}
		}

		if (input->down) {
			if (state->camera.orientation == CAM_NW) {
				movement = SOUTH;
			}

			if (state->camera.orientation == CAM_SW) {
				movement = EAST;
			}

			if (state->camera.orientation == CAM_SE) {
				movement = NORTH;
			}

			if (state->camera.orientation == CAM_NE) {
				movement = WEST;
			}
		}

		if (input->left) {
			if (state->camera.orientation == CAM_NW) {
				movement = WEST;
			}

			if (state->camera.orientation == CAM_SW) {
				movement = SOUTH;
			}

			if (state->camera.orientation == CAM_SE) {
				movement = EAST;
			}

			if (state->camera.orientation == CAM_NE) {
				movement = NORTH;
			}
		}

		if (input->right) {
			if (state->camera.orientation == CAM_NW) {
				movement = EAST;
			}

			if (state->camera.orientation == CAM_SW) {
				movement = NORTH;
			}

			if (state->camera.orientation == CAM_SE) {
				movement = WEST;
			}

			if (state->camera.orientation == CAM_NE) {
				movement = SOUTH;
			}
		}

		tet_move_square(&current_tetronimo, movement, game_board);
		time_until_next_move = 0.05;
	}

	/* for (int i = 0; i < 4; i++) { */
	/* 	u8 *active = current_tetronimo.active[i]; */
	/* 	fprintf(stderr, "active[%d]: {%d, %d, %d}\n", i, active[0], active[1], active[2]); */
	/* } */
	/* fprintf(stderr, "\n"); */

	if (time_until_next_move >= 0) {
		time_until_next_move -= delta_time;
	}

	static bool cubes_should_fall = false;

	if (count < 0.5) {
		count += delta_time;
		cubes_should_fall = false;
	} else {
		count -= 0.5;
		cubes_should_fall = true;
	}

	gl.UseProgram(state->program);
	gl.UniformMatrix4fv(state->view_location, 1, GL_FALSE, (float *) view);
	gl.Uniform3fv(state->view_position_location, 1, (float *) state->camera.position);
	for (int i = 0; i < GAME_BOARD_XMAX; i++) {
		for (int j = 0; j < GAME_BOARD_YMAX; j++) {
			for (int k = 0; k < GAME_BOARD_ZMAX; k++) {
				if (game_board[i][j][k] == 1) {
					// TODO: instance cube render calls?
					render_cube((vec3) {i, j + 0.5, k}, 0, (vec3) {1.0, 1.0, 1.0}, (vec4) {1.0, 0.0, 1.0, 1.0});
				}
			}
		}
	}
	/* render_cube((vec3) {0}, 0, (vec3) {500.0, 0.0, 500.0}, (vec4) {0.4, 0.6, 0.0, 1.0}); */

	render_cube((vec3) {1.0, 0.0, 0.0}, 0, 0, (vec4) {1.0, 0.0, 0.0, 1.0});
	render_cube((vec3) {0.0, 1.0, 0.0}, 0, 0, (vec4) {0.0, 1.0, 0.0, 1.0});
	render_cube((vec3) {0.0, 0.0, 1.0}, 0, 0, (vec4) {0.0, 0.0, 1.0, 1.0});

	/* render_square_tetronimo((vec3) {5.0, 5.0, 5.0}, r, (vec4) {1.0, 1.0, 1.0, 1.0}); */
	render_square_tetronimo(current_tetronimo.position, 0, (vec4) {1.0, 1.0, 1.0, 1.0});

	render_cube((vec3) {4.5, -0.5, 4.5}, 0, (vec3) {10.0, 1.0, 10.0}, (vec4) {0.5, 0.5, 0.5, 1.0});
	gl.UseProgram(0);
}
