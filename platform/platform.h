/*
Portable ZX-Spectrum emulator.
Copyright (C) 2001-2015 SMT, Dexus, Alone Coder, deathsoft, djdron, scor

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef __PLATFORM_H__
#define __PLATFORM_H__

#include "../std.h"

#pragma once


#if defined(_WINDOWS) || defined(_LINUX) || defined(_MAC)

#ifndef USE_BENCHMARK
#ifndef USE_LIBRARY
#ifndef USE_SDL
#ifndef USE_SDL2

#define USE_OAL
#define USE_GL
//#define USE_GLUT
#define USE_WXWIDGETS

#undef USE_PNG
#define USE_PNG

#endif//USE_SDL2
#endif//USE_SDL
#endif//USE_LIBRARY
#endif//USE_BENCHMARK

#undef USE_CONFIG
#define USE_CONFIG
#undef USE_ZIP
#define USE_ZIP

#endif//_WINDOWS || _LINUX || _MAC

class eSpeccy;

namespace xPlatform
{

enum eKeyFlags
{
	KF_DOWN			= 0x01,
	KF_SHIFT		= 0x02,
	KF_CTRL			= 0x04,
	KF_ALT			= 0x08,

	KF_CURSORENTER	= 0x10,
	KF_CURSOR		= 0x20,
	KF_KEMPSTON		= 0x40,
	KF_SINCLAIR2	= 0x80,
	KF_QAOPM		= 0x100,
	KF_QAOPSPACE	= 0x200,
#ifdef RG350
	KF_KCUSTOM      = 0x400,
#endif

	KF_UI_SENDER	= 0x1000
};
enum eMouseAction { MA_MOVE, MA_BUTTON, MA_WHEEL };
enum eAction
{
	A_RESET, A_TAPE_TOGGLE, A_TAPE_QUERY, A_DISK_QUERY
};
enum eActionResult
{
	AR_OK,
	AR_TAPE_STARTED, AR_TAPE_STOPPED, AR_TAPE_NOT_INSERTED,
	AR_DISK_CHANGED, AR_DISK_NOT_CHANGED,
	AR_ERROR = -1
};

struct eHandler
{
	eHandler();
	~eHandler();
	virtual void OnInit() = 0;
	virtual void OnDone() = 0;
	virtual const char* OnLoop() = 0; // error desc if not NULL
	virtual const char* WindowCaption() const = 0;
	virtual void OnKey(char key, dword flags) = 0;
	virtual void OnMouse(eMouseAction action, byte a, byte b) = 0;

	virtual bool OnOpenFile(const char* name, const void* data = NULL, size_t data_size = 0) = 0;
	virtual bool OnSaveFile(const char* name) = 0;
	virtual bool FileTypeSupported(const char* name) const = 0;
	virtual eActionResult OnAction(eAction action) = 0;

	virtual bool GetReplayProgress(dword* frame_current, dword* frames_total, dword* frames_cached) = 0;

	// data to draw
	virtual void* VideoData() const = 0;
	virtual void* VideoDataUI() const = 0;
	// pause/resume function for sync video by audio
	virtual void VideoPaused(bool paused) = 0;
	virtual int VideoFrame() const = 0;
	// audio
	virtual int	AudioSources() const = 0;
	virtual void* AudioData(int source) const = 0;
	virtual dword AudioDataReady(int source) const = 0;
	virtual void AudioDataUse(int source, dword size) = 0;
	virtual void AudioSetSampleRate(dword sample_rate) = 0;
#ifdef RG350
	//Custom Joystick
	virtual char* CustomJoystick(void) = 0;
	virtual void SetCustomJoystick(char* joystick) = 0;
#endif

	virtual bool FullSpeed() const = 0;

	virtual eSpeccy* Speccy() const = 0;
};

eHandler* Handler();

void GetScaleWithAspectRatio43(float* sx, float* sy, int _w, int _h);

}
//namespace xPlatform

#endif//__PLATFORM_H__
