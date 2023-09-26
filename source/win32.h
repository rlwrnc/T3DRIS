#pragma once

#include <windows.h>
#include <dwmapi.h>
#include <stdlib.h>
#include <crtdbg.h>
#include <stdbool.h>

#include "win32_wgl.h"
#include "t3dris.h"

typedef struct {
	bool is_running;
	WPARAM return_code;
} Win32State;
