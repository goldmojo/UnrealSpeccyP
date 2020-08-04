/*
Portable ZX-Spectrum emulator.
Copyright (C) 2001-2010 SMT, Dexus, Alone Coder, deathsoft, djdron, scor

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

#include "../platform.h"
#include "../io.h"
#include "ui_main.h"
#include "ui_menu.h"
#include "ui_keyboard.h"
#include "ui_file_open.h"
#include "ui_web_browse.h"
#if defined RG350 || defined RETROFW
#include "ui_custom_joystick.h"
#endif
#include "../../tools/options.h"
#include "../../tools/profiler.h"
#include "../../options_common.h"

#ifdef USE_UI

namespace xUi
{

#ifdef USE_PROFILER
eDialog* CreateProfiler();
#endif//USE_PROFILER

static struct eOptionOpenFile : public xOptions::eOptionB
{
	eOptionOpenFile() : on(false) { storeable = false; }
	virtual const char* Name() const { return "open file"; }
	virtual const char*	Value() const { return ">"; }
	virtual void Change(bool next = true) { if(next) on = true; }
	bool on;
} op_open_file;

#if defined RG350 || defined RETROFW
static struct eOptionJCustom : public xOptions::eOptionB
{
	eOptionJCustom() : on(false) { storeable = false; }
	virtual const char* Name() const { return "custom joy"; }
	virtual const char*	Value() const { char* s = new char[8]; s = xPlatform::Handler()->CustomJoystick(); s[6] = '\0'; return strcat(s," >"); }
	virtual void Change(bool next = true) { if(next) on = true; }
	virtual int Order() const { return 12; }
	bool on;
} custom_joy;
#endif

#ifdef USE_WEB
static struct eOptionBrowseWeb : public xOptions::eOptionB
{
	eOptionBrowseWeb() : on(false) { storeable = false; }
	virtual const char* Name() const { return "browse web"; }
	virtual const char*	Value() const { return ">"; }
	virtual void Change(bool next = true) { if(next) on = true; }
	virtual int Order() const { return 1; }
	bool on;
} op_browse_web;
#endif//USE_WEB

//=============================================================================
//	eMainDialog::eMainDialog
//-----------------------------------------------------------------------------
eMainDialog::eMainDialog() : clear(false)
{
}
//=============================================================================
//	eMainDialog::Update
//-----------------------------------------------------------------------------
void eMainDialog::Update()
{
	eInherited::Update();
	if(clear)
	{
		clear = false;
		Clear();
	}
	if(op_open_file.on)
	{
		op_open_file.on = false;
		Clear();
		eDialog* d = new eFileOpenDialog(xPlatform::OpLastFolder());
		d->Id(D_FILE_OPEN);
		Insert(d);
	}
#if defined RG350 || defined RETROFW
	if(custom_joy.on)
	{
		custom_joy.on = false;
		Clear();
		eDialog* d = new eCustomJoystickDialog(xPlatform::Handler()->CustomJoystick());
		d->Id(D_CUSTOM_JOY);
		Insert(d);
	}
#endif
#ifdef USE_WEB
	if(op_browse_web.on)
	{
		op_browse_web.on = false;
		Clear();
		eDialog* d = new eWebBrowseDialog;
		d->Id(D_BROWSE_WEB);
		Insert(d);
	}
#endif//USE_WEB
}
//=============================================================================
//	eMainDialog::OnKey
//-----------------------------------------------------------------------------
bool eMainDialog::OnKey(char key, dword flags)
{
	bool f = Focused();
	byte id = f ? (*childs)->Id() : -1;
	switch(key)
	{
	case 'k':
		Clear();
		if(!f || id != D_KEYS)
		{
			eKeyboard* d = new eKeyboard;
			d->Id(D_KEYS);
			Insert(d);
		}
		return true;
	case 'm':
		Clear();
		if(!f || id != D_MENU)
		{
			eMenu* d = new eMenu;
			d->Id(D_MENU);
			Insert(d);
		}
		return true;
#ifdef USE_PROFILER
	case 'p':
		Clear();
		if(!f || id != D_PROFILER)
		{
			eDialog* d = CreateProfiler();
			d->Id(D_PROFILER);
			Insert(d);
		}
		return true;
#endif//USE_PROFILER
	}
	return eInherited::OnKey(key, flags);
}
//=============================================================================
//	eMainDialog::OnNotify
//-----------------------------------------------------------------------------
void eMainDialog::OnNotify(byte n, byte from)
{
	using namespace xPlatform;
	switch(from)
	{
	case D_FILE_OPEN:
		if(!clear)
		{
			eFileOpenDialog* d = (eFileOpenDialog*)*childs;
			Handler()->OnOpenFile(d->Selected());
			clear = true;
		}
		break;
#ifdef USE_WEB
	case D_BROWSE_WEB:
		if(!clear)
		{
			eWebBrowseDialog* d = (eWebBrowseDialog*)*childs;
			if(d->Selected())
				Handler()->OnOpenFile(d->Selected());
			clear = true;
		}
		break;
#endif//USE_WEB
	case D_KEYS:
		{
			eKeyboard* d = (eKeyboard*)*childs;
			byte key = d->Key();
			dword flags = d->Pressed() ? KF_DOWN : 0;
			flags |= d->Caps() ? KF_SHIFT : 0;
			flags |= d->Symbol() ? KF_ALT : 0;
			flags |= KF_UI_SENDER;
			Handler()->OnKey(key, flags);
		}
		break;
#if defined RG350 || defined RETROFW
	case D_CUSTOM_JOY:
		{
			eCustomJoystickDialog* d = (eCustomJoystickDialog*)*childs;
			if(d->Quit())
				clear = true;
		}
		break;
#endif
	}
}

}
//namespace xUi

#endif//USE_UI
