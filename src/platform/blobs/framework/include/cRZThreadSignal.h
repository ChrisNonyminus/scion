/*
 *  Scion - an open-source implementation of the Maxis GZCOM/RZCOM framework
 *  Copyright (C) 2022  Nelson Gomez (nsgomez) <nelson@ngomez.me>
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
#include "cIGZThreadSignal.h"
#include "GZTypes.h"
#include "SignalPlatformData.h"

class cRZThreadSignal : public cIGZThreadSignal
{
public:
	cRZThreadSignal(bool manualReset);
	virtual ~cRZThreadSignal();

public:
	virtual uint32_t Release();
	virtual bool Signal();
	virtual bool Broadcast();
	virtual TimedWaitResult TimedWait(uint32_t waitTimeInMicroseconds);
	virtual bool IsValid();

public:
	virtual bool Unsignal();
	virtual bool GetEventHandle(void* eventOut);

protected:
	SignalPlatformData* signal;
};