
/***************************************************************************
 *                                                                         *
 *                     _____     ____                                      *
 *                    |  __ \   / __ \   _     _ _____                     *
 *                    | |  \ \ / /  \_\ | |   | |  _  \                    *
 *                    | |   \ \| |      | |   | | |_| |                    *
 *                    | |   | || |      | |   | |  ___/                    *
 *                    | |   / /| |   __ | |   | |  _  \                    *
 *                    | |__/ / \ \__/ / | |___| | |_| |                    *
 *                    |_____/   \____/  |_____|_|_____/                    *
 *                                                                         *
 *                       Wiimms source code library                        *
 *                                                                         *
 ***************************************************************************
 *                                                                         *
 *        Copyright (c) 2012-2020 by Dirk Clemens <wiimm@wiimm.de>         *
 *                                                                         *
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   See file gpl-2.0.txt or http://www.gnu.org/licenses/gpl-2.0.txt       *
 *                                                                         *
 ***************************************************************************/

#ifndef DCLIB_SYSTEM_H
#define DCLIB_SYSTEM_H 1

#include <stdio.h>

///////////////////////////////////////////////////////////////////////////////

typedef enum enumSystemID
{
	SYSID_UNKNOWN		= 0x00000000,
	SYSID_I386		= 0x01000000,
	SYSID_X86_64		= 0x02000000,
	SYSID_CYGWIN32		= 0x03000000,
	SYSID_CYGWIN64		= 0x04000000,
	SYSID_APPLE		= 0x05000000,
	SYSID_LINUX		= 0x06000000,
	SYSID_UNIX		= 0x07000000,

} enumSystemID;

///////////////////////////////////////////////////////////////////////////////

#undef SYSTEM_LINUX

#ifdef __CYGWIN__
  #define SYSTEM "cygwin"
  #ifdef __x86_64
	#define SYSTEM2		"cygwin64"
	#define SYSTEMID	SYSID_CYGWIN64
  #else
	#define SYSTEM2		"cygwin32"
	#define SYSTEMID	SYSID_CYGWIN32
  #endif
#elif __APPLE__
	#define SYSTEM		"mac"
	#define SYSTEMID	SYSID_APPLE
#elif __linux__
  #define SYSTEM_LINUX 1
  #ifdef __i386__
	#define SYSTEM		"i386"
	#define SYSTEMID	SYSID_I386
  #elif __x86_64__
	#define SYSTEM		"x86_64"
	#define SYSTEMID	SYSID_X86_64
  #else
	#define SYSTEM		"linux"
	#define SYSTEMID	SYSID_LINUX
  #endif
#elif __unix__
	#define SYSTEM		"unix"
	#define SYSTEMID	SYSID_UNIX
#else
	#define SYSTEM		"unknown"
	#define SYSTEMID	SYSID_UNKNOWN
#endif

#ifndef SYSTEM2
	#define SYSTEM2		SYSTEM
#endif

///////////////////////////////////////////////////////////////////////////////

static inline void dclibPrintSystem ( FILE * f )
{
    fprintf(f,
	"SYSTEM\t\t:= %s\n"
	"SYSTEM2\t\t:= %s\n"
	"SYSTEMID\t:= 0x%x\n"
  #ifdef SYSTEM_LINUX
	"SYSTEM_LINUX\t:= 1\n"
  #endif
	,SYSTEM
	,SYSTEM2
	,SYSTEMID
	);
}

///////////////////////////////////////////////////////////////////////////////

#endif // DCLIB_SYSTEM_H 1

