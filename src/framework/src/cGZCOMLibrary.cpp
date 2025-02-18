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

#include "cGZCOMLibrary.h"
#include "cIGZCOMDirector.h"

static bool GZCOMLibrary(HINSTANCE& handle, const char* path)
{
	__try
	{
		handle = LoadLibrary(path);
	}
	__except (EXCEPTION_EXECUTE_HANDLER)
	{
		return false;
	}

	return true;
}

static bool GZCOMLibraryW(HINSTANCE& handle, const wchar_t* path)
{
	__try
	{
		handle = LoadLibraryW(path);
	}
	__except (EXCEPTION_EXECUTE_HANDLER)
	{
		return false;
	}

	return true;
}

static void GZCOMFreeLibrary(HINSTANCE handle, const char* path)
{
	__try
	{
		FreeLibrary(handle);
	}
	__except (EXCEPTION_EXECUTE_HANDLER)
	{
	}
}

static bool GZCOMGetProcAddress(cIGZCOMDirector*(*&func)(), const HINSTANCE& handle, const char* funcName)
{
	__try
	{
		func = (cIGZCOMDirector*(*)())GetProcAddress(handle, funcName);
	}
	__except (EXCEPTION_EXECUTE_HANDLER)
	{
		return false;
	}

	return true;
}

cGZCOMLibrary::cGZCOMLibrary()
{
}

cGZCOMLibrary::cGZCOMLibrary(const cIGZString& libraryPath)
	: libraryPath(libraryPath)
{
}

cGZCOMLibrary::~cGZCOMLibrary()
{
}

bool cGZCOMLibrary::QueryInterface(GZIID iid, void** outPtr)
{
	if (iid == GZIID_cIGZCOMLibrary)
	{
		*outPtr = static_cast<cIGZCOMLibrary*>(this);
		AddRef();
		return true;
	}
	else if (iid == GZIID_cIGZUnknown)
	{
		*outPtr = static_cast<cIGZUnknown*>(this);
		AddRef();
		return true;
	}

	return false;
}

uint32_t cGZCOMLibrary::AddRef()
{
	return ++refCount;
}

uint32_t cGZCOMLibrary::Release()
{
	return --refCount;
}

bool cGZCOMLibrary::Load()
{
	if (!loaded)
	{
		// TODO
	}

	return true;
}

bool cGZCOMLibrary::Free()
{
	if (loaded)
	{
		if (director)
		{
			director = NULL;
		}

		GZCOMFreeLibrary(handle, libraryPath.ToChar());
		handle = NULL;
		loaded = false;
	}

	return true;
}

void cGZCOMLibrary::GetPath(cIGZString& path) const
{
	path.Copy(this->libraryPath);
}

bool cGZCOMLibrary::SetPath(const cIGZString& path)
{
	if (!loaded)
	{
		this->libraryPath.Copy(path);
		return true;
	}

	return false;
}

#if defined(_WIN32)
HINSTANCE cGZCOMLibrary::GetHandle() const
#elif defined(__linux__)
void* cGZCOMLibrary::GetHandle() const
#endif
{
    return handle;
}

#if defined(_WIN32)
void
cGZCOMLibrary::SetHandle(HINSTANCE handle)
#elif defined(__linux__)
void cGZCOMLibrary::SetHandle(void* handle)
#endif
{
    this->handle = handle;
}

cIGZCOMDirector* cGZCOMLibrary::GetDirector() const
{
	return director;
}

void cGZCOMLibrary::SetDirector(cIGZCOMDirector* director)
{
	if (this->director != director)
	{
		if (director)
		{
			director->AddRef();
		}

		if (this->director)
		{
			this->director->Release();
		}

		this->director = director;
	}
}

bool cGZCOMLibrary::IsLoaded() const
{
	return loaded;
}

void cGZCOMLibrary::SetLoaded(bool loaded)
{
	this->loaded = loaded;
}

bool cGZCOMLibrary::operator< (const cGZCOMLibrary& other) const
{
	cRZString comparand;
	other.GetPath(comparand);
	return libraryPath < comparand;
}

bool cGZCOMLibrary::operator> (const cGZCOMLibrary& other) const
{
	cRZString comparand;
	other.GetPath(comparand);
	return libraryPath > comparand;
}

bool cGZCOMLibrary::operator== (const cGZCOMLibrary& other) const
{
	cRZString comparand;
	other.GetPath(comparand);
	return libraryPath == comparand;
}