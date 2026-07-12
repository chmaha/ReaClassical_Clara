/*
 * ReaClassical Clara (a minimal screen-reader announcer fork of OSARA)
 * Exported API functions code
 * Exports functions to be called by REAPER extensions and scripts.
 * Copyright 2020-2023 James Teh
 * License: GNU General Public License version 2.0
 */

#include "clara.h"

// The _vararg_ version is needed for ReaScript.

void clara_outputMessage(const char* message) {
	_outputMessage(message);
}
void* _vararg_clara_outputMessage(void** args, int nArgs) {
	clara_outputMessage((const char*)args[0]);
	return nullptr;
}

void registerExports(reaper_plugin_info_t* rec) {
	rec->Register("API_clara_outputMessage", (void*)clara_outputMessage);
	rec->Register("APIvararg_clara_outputMessage",
		(void*)_vararg_clara_outputMessage);
	rec->Register("APIdef_clara_outputMessage",
		(void*)"void\0const char*\0message\0"
		"Output a message to screen readers.\n"
		"This should only be used in consultation with screen reader users. "
		"Note that this may not work on Windows when certain GUI controls have "
		"focus such as list boxes and trees.");
}
