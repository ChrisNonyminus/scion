/*
 *  Scion - an open-source implementation of the Maxis GZCOM/RZCOM framework
 *  Copyright (C) 2021  Nelson Gomez (nsgomez) <nelson@ngomez.me>
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation, under
 *  version 2.1 of the License.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 */

#pragma once

#ifdef _WIN32
#include <Windows.h>
#elif __linux__
#include <pthread.h>
#endif

class cIGZString;

class cRZMutex
{
public:
	cRZMutex(cIGZString const& mutexName);

	virtual void Release();
	virtual bool Lock();
	virtual bool Unlock();
	virtual bool TryLock();
	virtual bool IsValid();
	virtual ~cRZMutex();

#ifdef _WIN32
	virtual HANDLE GetMutexHandle();

protected:
	HANDLE mutex;
#elif __linux__
    virtual pthread_mutex_t* GetMutexHandle();

protected:
    pthread_mutex_t mutex;
#endif
};