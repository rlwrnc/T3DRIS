#include "win32.h"

#if TED_RELEASE == 0

game_initialize_func *game_initialize = 0;
game_update_and_render_func *game_update_and_render = 0;

static inline HGLRC win32_initialize_opengl(HDC device_context);

OpenGLFunctions win32_load_opengl_functions()
{
	OpenGLFunctions result = {0};
	FOR_EACH_GL_FUNCTION(LOAD_GL_FUNCTION_WIN32)
	return result;
}

void *platform_hotload_load_game_code(const char *compile_path, const char *load_path)
{
	CopyFile(compile_path, load_path, false);
	HMODULE code_handle = LoadLibraryA(load_path);
	if (code_handle == 0) {
		DWORD error = GetLastError();
		fprintf(stderr, "%ld\n", error);
		exit(1);
	}

	game_initialize = (game_initialize_func *)
		GetProcAddress(code_handle, "game_initialize");
	assert(game_initialize);

	game_update_and_render = (game_update_and_render_func *)
		GetProcAddress(code_handle, "game_update_and_render");
	assert(game_update_and_render);

	return code_handle;
}

bool platform_hotload_game_has_been_compiled(const char *compile_path, const char *load_path)
{
	FILETIME compile_filetime = {0}, load_filetime = {0};
	WIN32_FILE_ATTRIBUTE_DATA file_data = {0};

	assert(GetFileAttributesExA(compile_path, GetFileExInfoStandard, &file_data));
	compile_filetime = file_data.ftLastWriteTime;
	assert(GetFileAttributesExA(load_path, GetFileExInfoStandard, &file_data));
	load_filetime = file_data.ftLastWriteTime;

	int time_difference = CompareFileTime(&compile_filetime, &load_filetime);
	return (time_difference != 0);
}

void platform_hotload_unload_game_code(void *code_handle)
{
	assert(FreeLibrary((HMODULE) code_handle));
	code_handle = 0;
	game_initialize = 0;
	game_update_and_render = 0;
}
#endif

GameMemory platform_initialize_game_memory(size_t size_permanent, size_t size_transient)
{
#if TED_RELEASE == 0
	LPVOID base_address = (LPVOID) terabytes(3);
#else
	LPVOID base_address = 0;
#endif

	GameMemory new_memory = {0};
	new_memory.size_total = size_permanent + size_transient;

	MemoryArena permanent = {0}, transient = {0};
	permanent.size = size_permanent;
	permanent.base = (u8 *) VirtualAlloc(
			base_address,
			new_memory.size_total,
			MEM_RESERVE | MEM_COMMIT,
			PAGE_READWRITE);
	permanent.top = permanent.base;
	transient.size = size_transient;
	transient.base = (u8 *) permanent.base + size_transient;
	transient.top = transient.base;
	new_memory.permanent = permanent;
	new_memory.transient = transient;

	return new_memory;
}

int platform_memory_game_deallocate(GameMemory *memory)
{
	int result = VirtualFree(
			memory->permanent.base,
			memory->size_total,
			MEM_DECOMMIT | MEM_RELEASE);
	*memory = (GameMemory) {0};
	return result;
}

static LRESULT CALLBACK window_callback(HWND window, UINT message, WPARAM wparam,
		LPARAM lparam)
{
	switch (message) {
		case WM_CREATE: {
			HDC current_dc = GetDC(window);
			// win_opengl_initialize(current_dc);
			ReleaseDC(window, current_dc);
		} break;
		case WM_SIZE: {
			u32 width = LOWORD(lparam);
			u32 height = HIWORD(lparam);
			if (height == 0)
				height = 1;
			/* glViewport(0, 0, width, height); */
		} break;
		case WM_DESTROY:
		case WM_CLOSE: {
			OutputDebugString("WM_CLOSE\n");
			PostQuitMessage(0);
		} break;
		default: {
			return DefWindowProc(window, message, wparam, lparam);
		} break;
	}
	return 0;
}

static InputState win32_handle_events(Win32State *state, HWND window, InputState previous)
{
	InputState input = previous;
	MSG message = {0};
	while (PeekMessage(&message, window, 0, 0, PM_REMOVE)) {
		switch (message.message) {
			case WM_QUIT: {
				state->is_running = false;
				state->return_code = message.wParam;
				OutputDebugString("WM_QUIT\n");
			} break;
			case WM_SYSKEYDOWN:
			case WM_SYSKEYUP:
			case WM_KEYDOWN:
			case WM_KEYUP: {
				u32 keycode = (u32) message.wParam;
				u16 key_flags = HIWORD(message.lParam);
				bool was_pressed = (key_flags & KF_REPEAT);
				bool is_pressed = !(key_flags & KF_UP);

				if (is_pressed != was_pressed) {
					// TODO: only exit on escape in debug mode
					if (keycode == VK_ESCAPE) {
						state->is_running = false;
						state->return_code = 0;
					}

					if (keycode == 'W') {
						if (is_pressed)  {
							input.up = true;
							input.pressed_button_count++;
							OutputDebugString("is_pressed\n");
						}

						if (was_pressed) {
							input.up = false;
							input.pressed_button_count--;
							OutputDebugString("was_pressed\n");
						}
					}

					if (keycode == 'D') {
						if (is_pressed)  {
							input.right = true;
							input.pressed_button_count++;
						}

						if (was_pressed) {
							input.right = false;
							input.pressed_button_count--;
						}
					}

					if (keycode == 'S') {
						if (is_pressed)  {
							input.down = true;
							input.pressed_button_count++;
							fprintf(stderr, "%d\n", input.pressed_button_count);
							OutputDebugString("is_pressed\n");
						}

						if (was_pressed) {
							input.down = false;
							input.pressed_button_count--;
							fprintf(stderr, "%d\n", input.pressed_button_count);
							OutputDebugString("was_pressed\n");
						}
					}

					if (keycode == 'A') {
						if (is_pressed)  {
							input.left = true;
							input.pressed_button_count++;
						}

						if (was_pressed) {
							input.left = false;
							input.pressed_button_count--;
						}
					}

					if (keycode == 'Q') {
						if (is_pressed) {
							input.rotate_ccw = true;
						}

						if (was_pressed) {
							input.rotate_ccw = false;
						}
					}

					if (keycode == 'E') {
						if (is_pressed)
							input.rotate_cw = true;
						if (was_pressed)
							input.rotate_cw = false;
					}
				}
			} break;
			default: {
				TranslateMessage(&message);
				DispatchMessage(&message);
			} break;
		}
	}
	return input;
}

static inline void win32_load_wgl_extensions()
{
	// make temporary window class for a temporary device context
	WNDCLASS temp_class = {0};
	temp_class.style = CS_HREDRAW | CS_VREDRAW;
	temp_class.lpfnWndProc = DefWindowProc;
	temp_class.hInstance = GetModuleHandle(0);
	temp_class.lpszClassName = "temp_window";
	assert(RegisterClass(&temp_class));
	
	HWND temp_window = CreateWindowExA(
			0, temp_class.lpszClassName, "temp window", 0,
			CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
			0, 0, temp_class.hInstance, 0);
	assert(temp_window);

	// make temporary device context for a pixel format
	HDC temp_dc = GetDC(temp_window);

	// set desired pixel format
	PIXELFORMATDESCRIPTOR desired_format;
	desired_format.nSize = sizeof(PIXELFORMATDESCRIPTOR);
	desired_format.nVersion = 1;
	desired_format.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
	desired_format.cColorBits = 32;
	desired_format.cAlphaBits = 8;
	desired_format.iLayerType = PFD_MAIN_PLANE;
	desired_format.cDepthBits = 24;
	desired_format.cStencilBits = 8;

	// get a pixel format based on our desired format
	int receivedFormat = ChoosePixelFormat(temp_dc, &desired_format);
	assert(receivedFormat);
	assert(SetPixelFormat(temp_dc, receivedFormat, &desired_format));

	// create a temporary rendering context so we can load WGL extensions
	HGLRC temp_rc = wglCreateContext(temp_dc);
	assert(temp_rc);
	assert(wglMakeCurrent(temp_dc, temp_rc));

	// this funciton allows us to only load the opengl core profile
	wglCreateContextAttribsARB = (wglCreateContextAttribsARB_func *)
		wglGetProcAddress("wglCreateContextAttribsARB");
	assert(wglCreateContextAttribsARB);

	// this function gives us a better way to get a pixel format
	wglChoosePixelFormatARB = (wglChoosePixelFormatARB_func *)
		wglGetProcAddress("wglChoosePixelFormatARB");
	assert(wglChoosePixelFormatARB);

	// this function allows us to vsync
	wglSwapIntervalEXT = (wglSwapIntervalEXT_func *)
		wglGetProcAddress("wglSwapIntervalEXT");
	assert(wglSwapIntervalEXT);

	// clean up all the nonsense we had to make
	wglMakeCurrent(temp_dc, 0);
	wglDeleteContext(temp_rc);
	ReleaseDC(temp_window, temp_dc);
	DestroyWindow(temp_window);
}

static inline HGLRC win32_initialize_opengl(HDC device_context)
{
	win32_load_wgl_extensions();

	int desired_format_attributes[] = {
		WGL_DRAW_TO_WINDOW_ARB, GL_TRUE,
		WGL_SUPPORT_OPENGL_ARB, GL_TRUE,
		WGL_DOUBLE_BUFFER_ARB, GL_TRUE,
		WGL_ACCELERATION_ARB, WGL_FULL_ACCELERATION_ARB,
		WGL_PIXEL_TYPE_ARB, WGL_TYPE_RGBA_ARB,
		WGL_COLOR_BITS_ARB, 32,
		WGL_DEPTH_BITS_ARB, 24,
		WGL_STENCIL_BITS_ARB, 8,
		0
	};

	int received_format;
	UINT num_formats_received;
	wglChoosePixelFormatARB(device_context, desired_format_attributes,
			0, 1, &received_format, &num_formats_received);
	
	PIXELFORMATDESCRIPTOR pixel_format;
	DescribePixelFormat(device_context, received_format,
			sizeof (PIXELFORMATDESCRIPTOR), &pixel_format);
	SetPixelFormat(device_context, received_format, &pixel_format);

	int render_context_attributes[] = {
		WGL_CONTEXT_MAJOR_VERSION_ARB, 3,
		WGL_CONTEXT_MINOR_VERSION_ARB, 3,
		WGL_CONTEXT_LAYER_PLANE_ARB, 0,
		WGL_CONTEXT_FLAGS_ARB, 0,
		WGL_CONTEXT_PROFILE_MASK_ARB, WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
		0
	};

	HGLRC render_context = wglCreateContextAttribsARB(device_context, 0,
			render_context_attributes);
	wglSwapIntervalEXT(1);
	wglMakeCurrent(device_context, render_context);
	const unsigned char *string = glGetString(GL_VERSION);
	return render_context;
}

static inline HWND win32_create_window(HINSTANCE instance)
{
	WNDCLASS window_class = {0};
	window_class.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	window_class.lpfnWndProc = window_callback;
	window_class.hInstance = instance;
	window_class.lpszClassName = "t3dris";
	RegisterClass(&window_class);

	// TODO: make window resolution variable
	HWND window = CreateWindowExA(
		0, "t3dris", "T3DRIS",
		WS_OVERLAPPED | WS_MINIMIZEBOX | WS_SYSMENU | WS_VISIBLE,
		CW_USEDEFAULT, CW_USEDEFAULT, 1920, 1080,
		0, 0, instance, 0);
	return window;
}

int WINAPI WinMain(HINSTANCE current, HINSTANCE previous, LPSTR command, int show_code)
{
	Win32State win32 = {0};
	HWND window = win32_create_window(current);
	HDC window_dc = GetDC(window);
	win32_initialize_opengl(window_dc);
	ShowWindow(window, show_code);

	GameMemory game_memory = platform_initialize_game_memory(kilobytes(64), megabytes(128));
	OpenGLFunctions *gl = (OpenGLFunctions *)
		arena_allocate(&game_memory.permanent, sizeof (OpenGLFunctions));
	*gl = win32_load_opengl_functions();
	arena_allocate(&game_memory.permanent, sizeof (GameState));

	LARGE_INTEGER previous_time, current_time, performace_frequency;
	QueryPerformanceFrequency(&performace_frequency);
	QueryPerformanceCounter(&previous_time);

	void *game_code_handle = platform_hotload_load_game_code("game.dll", "game_load.dll");
	game_initialize(&game_memory, gl);
	
	// TODO: fixed-timestep facilities
	QueryPerformanceCounter(&current_time);
	u64 ms_elapsed = (current_time.QuadPart - previous_time.QuadPart);
	float delta_time = (float) ms_elapsed / performace_frequency.QuadPart;

	win32.is_running = true;
	MSG current_message = {0};
	// TODO: move this to game_memory?
	InputState input = {0};
	while (win32.is_running) {
		if (platform_hotload_game_has_been_compiled("game.dll", "game_load.dll")) {
			platform_hotload_unload_game_code(game_code_handle);
			game_code_handle = platform_hotload_load_game_code("game.dll",
					"game_load.dll");
			game_initialize(&game_memory, gl);
		}

		input = win32_handle_events(&win32, window, input);
		game_update_and_render(&game_memory, delta_time, &input);
		SwapBuffers(window_dc);

		previous_time = current_time;
		QueryPerformanceCounter(&current_time);
		ms_elapsed = (current_time.QuadPart - previous_time.QuadPart);
		delta_time = (float) ms_elapsed / performace_frequency.QuadPart;
	}

	return win32.return_code;
}
