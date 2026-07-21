/*
 * ReaClassical Clara (a minimal screen-reader announcer fork of OSARA)
 * Exported API functions code
 * Exports functions to be called by REAPER extensions and scripts.
 * Copyright 2020-2023 James Teh
 * License: GNU General Public License version 2.0
 */

#include "clara.h"

// The _vararg_ version is needed for ReaScript.

#ifndef CLARA_VERSION
#define CLARA_VERSION "unknown"
#endif

const char* clara_getVersion() {
	return CLARA_VERSION;
}
void* _vararg_clara_getVersion(void** args, int nArgs) {
	return (void*)clara_getVersion();
}

void clara_outputMessage(const char* message) {
	_outputMessage(message);
}
void* _vararg_clara_outputMessage(void** args, int nArgs) {
	clara_outputMessage((const char*)args[0]);
	return nullptr;
}

// Find a top-level window by exact title match, and give it OS focus.
// FindWindowEx/SetFocus resolve to native Win32 on Windows and to SWELL's
// equivalents on Mac (see clara.h's windows.h include), so this needs no
// platform-specific code. Together, these two functions cover the same
// ground js_ReaScriptAPI's JS_Window_Find/JS_Window_SetFocus were being
// used for elsewhere, without requiring that extra download for Clara
// users (who are Windows/macOS only).
HWND clara_findWindow(const char* title) {
	return FindWindowEx(nullptr, nullptr, nullptr, title);
}
void* _vararg_clara_findWindow(void** args, int nArgs) {
	return (void*)clara_findWindow((const char*)args[0]);
}

void clara_setFocus(HWND hwnd) {
	if (!hwnd) {
		return;
	}
	SetFocus(hwnd);
}
void* _vararg_clara_setFocus(void** args, int nArgs) {
	clara_setFocus((HWND)args[0]);
	return nullptr;
}

void registerExports(reaper_plugin_info_t* rec) {
	rec->Register("API_clara_getVersion", (void*)clara_getVersion);
	rec->Register("APIvararg_clara_getVersion",
		(void*)_vararg_clara_getVersion);
	rec->Register("APIdef_clara_getVersion",
		(void*)"const char*\0\0\0"
		"Returns the installed Clara version (e.g. \"1.1.2\").");

	rec->Register("API_clara_outputMessage", (void*)clara_outputMessage);
	rec->Register("APIvararg_clara_outputMessage",
		(void*)_vararg_clara_outputMessage);
	rec->Register("APIdef_clara_outputMessage",
		(void*)"void\0const char*\0message\0"
		"Output a message to screen readers.\n"
		"This should only be used in consultation with screen reader users. "
		"Note that this may not work on Windows when certain GUI controls have "
		"focus such as list boxes and trees.");

	rec->Register("API_clara_findWindow", (void*)clara_findWindow);
	rec->Register("APIvararg_clara_findWindow",
		(void*)_vararg_clara_findWindow);
	rec->Register("APIdef_clara_findWindow",
		(void*)"void*\0const char*\0title\0"
		"Find a top-level window by exact title match. "
		"Returns NULL if no matching window is found.");

	rec->Register("API_clara_setFocus", (void*)clara_setFocus);
	rec->Register("APIvararg_clara_setFocus",
		(void*)_vararg_clara_setFocus);
	rec->Register("APIdef_clara_setFocus",
		(void*)"void\0void*\0hwnd\0"
		"Give OS focus to the given window.");
}
