#include "t3dris.h"
#include "memory.c"
#include "renderer.c"

/* TODO:
 * - add camera
 * - abstract renderer
 */

#define CAMERA_RADIUS 5.0


// TODO: move global variables to game memory
static mat4 model = IDENTITY4, view = IDENTITY4, projection = IDENTITY4;
GLint header[2];
static GLuint vbo, ebo, vao = 0, program;
static GLint view_location = 0, projection_location = 0;
static GLint view_position_location = 0, light_position_location = 0;
GLuint *cube_elements;

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

	program = compile_and_link_shader_program("shaders/vertex.glsl", "shaders/fragment.glsl");

	mat4 translation, rotation, scale;
	mat4_translate(translation, 0.0, 0.0, 0.0);
	vec3 rotation_axis = {1.0, 1.0, 0.0};
	mat4_rotate(rotation, 1.0 / 12.0, rotation_axis);
	mat4_scale(scale, 0.25, 0.25, 0.25);

	mat4_multiply(state->model, state->model, translation);
	mat4_multiply(state->model, state->model, rotation);
	mat4_multiply(state->model, state->model, scale);

	mat4_lookat(view, (vec3) {-0.0, -0.0, 6.0}, (vec3) {0.0});

	mat4_perspective(projection, 0.125, 1920.0 / 1080.0, 0.1, 100.0);

	// NOTE: don't forget to bind the program when setting uniforms!
	gl.UseProgram(program);
	int retval = glGetError();
	view_location = gl.GetUniformLocation(program, "view");
	retval = glGetError();
	projection_location = gl.GetUniformLocation(program, "projection");
	retval = glGetError();
	view_position_location = gl.GetUniformLocation(program, "view_position");
	light_position_location = gl.GetUniformLocation(program, "light_position");
	gl.UniformMatrix4fv(view_location, 1, GL_FALSE, (float *) view);
	gl.UniformMatrix4fv(projection_location, 1, GL_FALSE, (float *) projection);
	gl.Uniform3f(light_position_location, 1.0, 1.0, 2.0);
	gl.UseProgram(0);

	initialize_cube(program);
	
	glEnable(GL_DEPTH_TEST);
}

//

void game_update_and_render(GameMemory *memory, float delta_time)
{
	static float angle = 0;
	GameState *state = (GameState *) arena_peek(&memory->permanent);
	/* glClearColor(0.4f, 0.6f, 0.0f, 1.0f); */
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	mat4 view = {0};
	angle += 1.0 * delta_time;
	vec3 camera_location = { CAMERA_RADIUS * sin(angle), 0, CAMERA_RADIUS * cos(angle) };
	mat4_lookat(view, camera_location, (vec3) {0});

	gl.UseProgram(program);
	gl.UniformMatrix4fv(view_location, 1, GL_FALSE, (float *) view);
	gl.Uniform3fv(view_position_location, 1, (float *) camera_location);
	render_cube((vec3) {0}, 0, (vec3) {1.0, 1.0, 1.0}, (vec4) {1.0, 0.0, 1.0, 1.0});
	gl.UseProgram(0);
}
