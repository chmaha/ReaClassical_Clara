# ReaClassical Clara

Clara is a minimal REAPER extension that exposes a single ReaScript API function, `clara_outputMessage(message)`, for sending text directly to screen readers (via MSAA/UIA on Windows, NSAccessibility on Mac).

It does none of OSARA's automatic reporting — no track/item/action announcements, no key map. It's just a manual announcer building block, used by [ReaClassical](https://reaclassical.org) to give screen reader users spoken feedback from its own scripts.

## Origin

Clara started as a fork of [OSARA](https://github.com/jcsteh/osara) by James Teh and contributors, stripped down to only the manual-announcement API. Licensed under the GNU General Public License version 2.0 — see [copying.txt](copying.txt).

## Links

- ReaClassical: https://reaclassical.org
- ReaClassical on GitHub: https://github.com/chmaha/ReaClassical
