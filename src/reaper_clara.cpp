/*
 * ReaClassical Clara (a minimal screen-reader announcer fork of OSARA)
 * Main plug-in code
 * Copyright 2014-2024 NV Access Limited, James Teh
 * License: GNU General Public License version 2.0
 */

#ifdef _WIN32
#include <initguid.h>
#include <coguid.h>
// Must be defined before any C++ STL header is included.
#define _SILENCE_CXX17_CODECVT_HEADER_DEPRECATION_WARNING
#include <oleacc.h>
// We only need this on Windows and it apparently causes compilation issues on Mac.
#include <codecvt>
#include "uia.h"
#else
#include "osxa11y_wrapper.h" // NSA11y wrapper for OS X accessibility API
#endif
#include <string>
#include <sstream>
#define REAPERAPI_IMPLEMENT
#include "clara.h"

using namespace std;

HINSTANCE pluginHInstance;
HWND mainHwnd;
#ifdef _WIN32
CComPtr<IAccPropServices> accPropServices;
#endif

/*** Utilities */

#ifdef _WIN32

wstring_convert<codecvt_utf8_utf16<WCHAR>, WCHAR> utf8Utf16;
wstring widen(const string& text) {
	try {
		return utf8Utf16.from_bytes(text);
	} catch (range_error) {
		// #38: Invalid UTF-8. This really shouldn't happen,
		// but it seems REAPER/extensions sometimes use ANSI strings instead of UTF-8.
		// This hack just widens the string without any encoding conversion.
		// This may result in strange characters, but it's better than a crash.
		wostringstream s;
		s << text.c_str();
		return s.str();
	}
}
string narrow(const wstring& text) {
	return utf8Utf16.to_bytes(text);
}

string lastMessage;
HWND lastMessageHwnd = nullptr;
void _outputMessage(const string& message, bool interrupt) {
	if (!hasTriedToInitializeUia()) {
		// #1173: This is very early in startup and we haven't called initializeUia()
		// yet. We can't call it now, but we also don't want to fall back to MSAA.
		// A message this early isn't useful anyway, so just ignore it.
		return;
	}
	if (shouldUseUiaNotifications()) {
		if (sendUiaNotification(message, interrupt)) {
			return;
		}
	}
	// Tweak the MSAA accName for the current focus.
	HWND focus = GetFocus();
	if (!focus) {
		return;
	}
	if (lastMessage.compare(message) == 0) {
		// The last message was the same.
		// Clients may ignore a nameChange event if the name didn't change.
		// Append a space to make it different.
		string procMessage = message;
		procMessage += ' ';
		accPropServices->SetHwndPropStr(focus, OBJID_CLIENT, CHILDID_SELF, PROPID_ACC_NAME, widen(procMessage).c_str());
		lastMessage = procMessage;
	} else {
		accPropServices->SetHwndPropStr(focus, OBJID_CLIENT, CHILDID_SELF, PROPID_ACC_NAME, widen(message).c_str());
		lastMessage = message;
	}
	// Fire a nameChange event so ATs will report this text.
	NotifyWinEvent(EVENT_OBJECT_NAMECHANGE, focus, OBJID_CLIENT, CHILDID_SELF);
	lastMessageHwnd = focus;
}

bool isClassName(HWND hwnd, string className) {
	char buffer[50];
	if (GetClassName(hwnd, buffer, sizeof(buffer)) == 0) {
		return false;
	}
	return className.compare(buffer) == 0;
}

#else // _WIN32

void _outputMessage(const string& message, bool interrupt) {
	NSA11yWrapper::osxa11y_announce(message);
}

#endif // _WIN32

extern "C" {

REAPER_PLUGIN_DLL_EXPORT int REAPER_PLUGIN_ENTRYPOINT(REAPER_PLUGIN_HINSTANCE hInstance, reaper_plugin_info_t* rec) {
	if (rec) {
		// Load.
#if defined(_WIN32) && defined(_M_X64) && !defined(_M_ARM64EC)
		using IsWow64Process2Fn = BOOL (WINAPI*)(HANDLE, USHORT*, USHORT*);
		auto fn = reinterpret_cast<IsWow64Process2Fn>(GetProcAddress(GetModuleHandleW(L"kernel32.dll"), "IsWow64Process2"));
		if (fn) {
			USHORT processMachine = 0;
			USHORT nativeMachine = 0;
			fn(GetCurrentProcess(), &processMachine, &nativeMachine);
			if (nativeMachine == IMAGE_FILE_MACHINE_ARM64) {
				return 0; // Don't load on ARM64, use ARM64EC build instead.
			}
		}
#endif
		if (rec->caller_version != REAPER_PLUGIN_VERSION || !rec->GetFunc || REAPERAPI_LoadAPI(rec->GetFunc) != 0) {
			return 0; // Incompatible.
		}
		pluginHInstance = hInstance;
		mainHwnd = rec->hwnd_main;
#ifdef _WIN32
		if (accPropServices.CoCreateInstance(CLSID_AccPropServices) != S_OK) {
			return 0;
		}
		initializeUia();
#else
		NSA11yWrapper::init();
#endif
		registerExports(rec);
		return 1;

	} else {
		// Unload.
#ifdef _WIN32
		terminateUia();
		accPropServices = nullptr;
#else
		NSA11yWrapper::destroy();
#endif
		return 0;
	}
}

}
