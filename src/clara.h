/*
 * ReaClassical Clara (a minimal screen-reader announcer fork of OSARA)
 * Main header
 * Copyright 2014-2023 NV Access Limited, James Teh
 * License: GNU General Public License version 2.0
 */

#ifndef _CLARA_H
#define _CLARA_H

#ifdef _WIN32
# include <windows.h>
#else
// Disable warnings for SWELL, since we don't have any control over those.
# pragma clang diagnostic push
# pragma clang diagnostic ignored "-Weverything"
# include <windows.h>
# pragma clang diagnostic pop
#endif
#include <string>
#include <sstream>

#define REAPERAPI_MINIMAL
#define REAPERAPI_WANT_GetExtState

#include <reaper/reaper_plugin.h>
#include <reaper/reaper_plugin_functions.h>

extern HINSTANCE pluginHInstance;
extern HWND mainHwnd;

void _outputMessage(const std::string& message, bool interrupt = true);

#ifdef _WIN32
#include <atlcomcli.h>
#include <oleacc.h>

std::wstring widen(const std::string& text);
std::string narrow(const std::wstring& text);

extern CComPtr<IAccPropServices> accPropServices;

#endif

bool isClassName(HWND hwnd, std::string className);

// exports.cpp
void registerExports(reaper_plugin_info_t* rec);

#endif
