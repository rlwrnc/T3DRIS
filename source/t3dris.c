#include "t3dris.h"
#include "memory.c"

/* TODO:
 * - add camera
 * - abstract renderer
 */


// TODO: move global variables to game memory
static mat4 model = IDENTITY4, view = IDENTITY4, projection = IDENTITY4;
GLint header[2];
static GLuint vbo, ebo, vao = 0, program;
static GLint model_location = 0, view_location = 0, projection_location = 0;
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

	FILE *mesh_file = fopen("cube.dsm", "rb");
	float *cube_vertices;
	fread(header, sizeof (float), 2, mesh_file);
	cube_vertices = calloc(header[0], sizeof (float));
	cube_elements = calloc(header[1], sizeof (GLuint));
	fread(cube_vertices, sizeof (float), header[0], mesh_file);
	fread(cube_elements, sizeof (GLuint), header[1], mesh_file);
	fclose(mesh_file);

	mat4 translation, rotation, scale;
	mat4_translate(translation, 0.0, 0.0, 0.0);
	vec3 rotation_axis = {1.0, 1.0, 0.0};
	mat4_rotate(rotation, 1.0 / 12.0, rotation_axis);
	mat4_scale(scale, 0.25, 0.25, 0.25);

	mat4_multiply(state->model, state->model, translation);
	mat4_multiply(state->model, state->model, rotation);
	mat4_multiply(state->model, state->model, scale);

	/* mat4 camera_rotation = IDENTITY4; */
	/* vec3 camera_rotation_axis = {1.0, 0.0, 0.0}; */
	/* mat4_rotate(camera_rotation, 1.0 / 8.5, camera_rotation_axis); */
	/* mat4 camera_translate; */
	/* mat4_multiply(view, view, camera_rotation); */
	/* mat4_multiply(view, view, camera_translate); */
	/* mat4_translate(camera_translate, 0.0, 1.0, -1.0); */
	/* mat4_multiply(view, view, camera_translate); */
	/* mat4_translate(view, 0.0, -1.0, -1.0); */

	mat4_lookat(view, (vec3) {0.0, 0.0, 10.0}, (vec3) {0.0});

	mat4_perspective(projection, 0.125, 1920.0 / 1080.0, 0.1, 100.0);

	// NOTE: don't forget to bind the program when setting uniforms!
	gl.UseProgram(program);
	model_location = gl.GetUniformLocation(program, "model");
	int retval = glGetError();
	view_location = gl.GetUniformLocation(program, "view");
	retval = glGetError();
	projection_location = gl.GetUniformLocation(program, "projection");
	retval = glGetError();
	gl.UniformMatrix4fv(model_location, 1, GL_FALSE, (float *) model);
	gl.UniformMatrix4fv(view_location, 1, GL_FALSE, (float *) view);
	gl.UniformMatrix4fv(projection_location, 1, GL_FALSE, (float *) projection);
	gl.UseProgram(0);

	// initialize_cube();
	
	gl.GenVertexArrays(1, &vao);
	gl.BindVertexArray(vao);

	gl.GenBuffers(1, &vbo);
	gl.BindBuffer(GL_ARRAY_BUFFER, vbo);
	gl.BufferData(GL_ARRAY_BUFFER, header[0] * sizeof(float), cube_vertices, GL_STATIC_DRAW);
	
	gl.GenBuffers(1, &ebo);
	gl.BindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	gl.BufferData(GL_ELEMENT_ARRAY_BUFFER, header[1] * sizeof(GLuint), cube_elements, GL_STATIC_DRAW);

	gl.VertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof (float), (void *) 0);
	gl.VertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof (float), (void *) (3 * sizeof (float)));
	gl.EnableVertexAttribArray(0);
	gl.EnableVertexAttribArray(1);

	gl.BindBuffer(GL_ARRAY_BUFFER, 0);
	gl.BindVertexArray(0);

	glEnable(GL_DEPTH_TEST);
}

//
void game_update_and_render(GameMemory *memory)
{
	GameState *state = (GameState *) arena_peek(&memory->permanent);
	glClearColor(0.4f, 0.6f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// gl.UseProgram(program);
	// for (int i = 0; i < game_board_size; i++) {
	//	if (game_board[i] != 0)
	//		render_cube(position(i), 0, 0);
	// }
	// gl.UseProgram(0);

	gl.UseProgram(program);
	gl.BindVertexArray(vao);
	/* glDrawArrays(GL_TRIANGLES, 0, 36); */
	glDrawElements(GL_TRIANGLES, header[1], GL_UNSIGNED_INT, 0);
	gl.BindVertexArray(0);
	gl.UseProgram(0);
}
