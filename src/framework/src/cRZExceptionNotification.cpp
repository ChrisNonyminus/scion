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

#include "cRZExceptionNotification.h"

#ifdef APP_IS_SC4
#include "SC4Statics.h"
#endif

cRZExceptionNotification::cRZExceptionNotification()
{
}

cRZExceptionNotification::~cRZExceptionNotification()
{
}

bool cRZExceptionNotification::QueryInterface(GZIID iid, void** outPtr)
{
	if (iid == GZIID_cIGZUnknown)
	{
		*outPtr = static_cast<cIGZUnknown*>(this);
		AddRef();
		return true;
	}
	else if (iid == GZIID_cIGZExceptionNotification)
	{
		*outPtr = static_cast<cIGZExceptionNotification*>(this);
		AddRef();
		return true;
	}

	return false;
}

uint32_t cRZExceptionNotification::AddRef()
{
	return ++refCount;
}

uint32_t cRZExceptionNotification::Release()
{
	if (refCount == 1)
	{
		delete this;
		return 0;
	}
	else
	{
		return --refCount;
	}
}

bool cRZExceptionNotification::Notify(char const* exceptionText, int address)
{
#ifdef APP_IS_SC4
    cRZString str(exceptionText);
    SC4OSMessageBoxAlert(str);
#endif
	// TODO: This probably does actual work on W32
	return true;
}