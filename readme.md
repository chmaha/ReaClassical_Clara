# ReaClassical Clara

Clara is a minimal REAPER extension that exposes a few ReaScript API functions for accessibility, used by [ReaClassical](https://reaclassical.org) to give screen reader users spoken feedback and window focus recovery from its own scripts:

- `clara_getVersion()` — the installed Clara version (e.g. "1.1.2"), so callers can check whether they're already on the latest release.
- `clara_outputMessage(message)` — send text directly to screen readers (via MSAA/UIA on Windows, NSAccessibility on Mac).
- `clara_findWindow(title)` — find a top-level window by exact title match, returning its HWND (or NULL).
- `clara_setFocus(hwnd)` — give OS focus to a window.

It does none of OSARA's automatic reporting — no track/item/action announcements, no key map. It's just a manual announcer/focus building block; Clara users don't need js_ReaScriptAPI installed for window-focus recovery, since `clara_findWindow`/`clara_setFocus` cover it directly.

## Origin

Clara started as a fork of [OSARA](https://github.com/jcsteh/osara) by James Teh and contributors, stripped down to only the manual-announcement API. Licensed under the GNU General Public License version 2.0 — see [copying.txt](copying.txt).

## Links

- ReaClassical: https://reaclassical.org
- ReaClassical on GitHub: https://github.com/chmaha/ReaClassical
