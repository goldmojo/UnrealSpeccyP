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

#ifndef	__UI_CUSTOM_JOYSTICK_H__
#define	__UI_CUSTOM_JOYSTICK_H__

#if defined RG350 || defined RETROFW

#include "../../ui/ui_dialog.h"
#include "ui_keyboard.h"

#pragma once

namespace xUi
{

class eCJPad : public eDialog
{
	typedef eDialog eInherited;
public:
	eCJPad() { };
	virtual void Init();
protected:
	virtual void OnNotify(byte n, byte from);
	char text[9];
};

class eCustomJoystickDialog : public eDialog
{
	typedef eDialog eInherited;
public:
	eCustomJoystickDialog(char* oJoystick) : dpad(true), keyboard(false), quit(false), joy_key('\0'), clear(false), lastKey('.')
	{
		for(int i=0;i<5;i++)
			joystick[i] = oJoystick[i];
	}
	virtual void Update();
	const char* Joystick() const { return joystick; }
	//virtual bool OnKey(char key, dword flags);
	bool Quit(void) { return quit; };
	char* GetJoystick(void) { return joystick; };
	bool Focused() const
	{
		eDialog* d = (eDialog*)Childs()[0];
		if(d)
			return d->Childs()[0] != NULL;
		return false;
	}
protected:
	virtual void OnNotify(byte n, byte from);
	void ParseKey(char key, dword _flags);
protected:
	char  joystick[5];
	bool  dpad;
	bool  keyboard;
	bool  quit;
	byte  joy_key;
	bool  clear;
	char  lastKey;
};


}
//namespace xUi

#endif//RG350 - RETROFW


#endif//__UI_CUSTOM_JOYSTICK_H__
