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
#include <stdint.h>
#include "GZTypes.h"

static const GZIID GZIID_cIGZUnknown = 0x00000001;

class cIGZUnknown
{
public:
	virtual bool QueryInterface(GZIID iid, void** outPtr) = 0;
	virtual uint32_t AddRef() = 0;
	virtual uint32_t Release() = 0;
};

#define IUNKNOWN_COMMON_DEFS                                                   \
    uint32_t m_uiRefCount{0};                                                  \
    uint32_t AddRef() ;                                                \
    uint32_t Release() ;                                               \
    bool QueryInterface(uint32_t uiClsId, void **ppvObj) ;

#define GZ_IUNKNOWN_ID(classname, id) static const GZIID GZIID_##classname = id


#define IMPL_IGZUNKNOWN(ClassName, InterfaceClsid)                             \
    uint32_t ClassName::AddRef() { return ++m_uiRefCount; }                    \
    uint32_t ClassName::Release() {                                            \
        uint32_t uiRefCount = --m_uiRefCount;                                  \
        if (uiRefCount == 0) {                                                 \
            delete this;                                                       \
        }                                                                      \
        return uiRefCount;                                                     \
    }                                                                          \
    bool ClassName::QueryInterface(uint32_t uiClsId, void **ppvObj) {      \
        if (ppvObj == NULL) {                                                  \
            return 0;                                                          \
        }                                                                      \
        if (uiClsId == InterfaceClsid || uiClsId == 1) {                       \
            *ppvObj = this;                                                    \
        } else {                                                               \
            *ppvObj = NULL;                                                    \
            return false;                                                          \
        }                                                                      \
        AddRef();                                                              \
        return true;                                                              \
    }
#define CLSID_MATCH(clsid) || uiClsId == clsid
#define IMPL_IGZUNKNOWN_MULTIPLE_CLSIDS_LAZY_MACRO(ClassName, ...)             \
    uint32_t ClassName::AddRef() { return ++m_uiRefCount; }                    \
    uint32_t ClassName::Release() {                                            \
        uint32_t uiRefCount = --m_uiRefCount;                                  \
        if (uiRefCount == 0) {                                                 \
            delete this;                                                       \
        }                                                                      \
        return uiRefCount;                                                     \
    }                                                                          \
    bool ClassName::QueryInterface(uint32_t uiClsId, void **ppvObj) {      \
        if (ppvObj == NULL) {                                                  \
            return false;                                                          \
        }                                                                      \
        if (uiClsId == 1 FOR_EACH(CLSID_MATCH, __VA_ARGS__)) {                 \
            *ppvObj = this;                                                    \
        } else {                                                               \
            *ppvObj = NULL;                                                    \
            return false;                                                          \
        }                                                                      \
        AddRef();                                                              \
        return true;                                                              \
    }
#define CLSID_MATCH_BY_NAME(cls)                                               \
    if (uiClsId == GZIID_##cls) {                                                \
        *ppvObj = static_cast<cls *>(this);                                    \
    } else
#define IMPL_IGZUNKNOWN_MULTIPLE_CLSIDS_BY_NAME_LAZY_MACRO(ClassName, ...)     \
    uint32_t ClassName::AddRef() { return ++m_uiRefCount; }                    \
    uint32_t ClassName::Release() {                                            \
        uint32_t uiRefCount = --m_uiRefCount;                                  \
        if (uiRefCount == 0) {                                                 \
            delete this;                                                       \
        }                                                                      \
        return uiRefCount;                                                     \
    }                                                                          \
    bool ClassName::QueryInterface(uint32_t uiClsId, void **ppvObj) {      \
        if (ppvObj == NULL) {                                                  \
            return false;                                                          \
        }                                                                      \
        *ppvObj = NULL;                                                        \
        FOR_EACH(CLSID_MATCH_BY_NAME, __VA_ARGS__) { return 0; }               \
        AddRef();                                                              \
        return true;                                                              \
    }
#define IMPL_IGZUNKNOWN_CALL_SUPERCLASS_QUERYINTERFACE(ClassName, SuperClass,  \
                                                       InterfaceClsid)         \
    uint32_t ClassName::AddRef() { return ++m_uiRefCount; }                    \
    uint32_t ClassName::Release() {                                            \
        uint32_t uiRefCount = --m_uiRefCount;                                  \
        if (uiRefCount == 0) {                                                 \
            delete this;                                                       \
        }                                                                      \
        return uiRefCount;                                                     \
    }                                                                          \
    bool ClassName::QueryInterface(uint32_t uiClsId, void **ppvObj) {      \
        if (ppvObj == NULL) {                                                  \
            return false;                                                          \
        }                                                                      \
        if (uiClsId == InterfaceClsid || uiClsId == 1) {                       \
            *ppvObj = this;                                                    \
        } else {                                                               \
            return SuperClass::QueryInterface(uiClsId, ppvObj);                \
        }                                                                      \
        AddRef();                                                              \
        return true;                                                              \
    }
#define IMPL_IGZUNKNOWN_MULTIPLE_CLSIDS_CALL_SUPERCLASS_QUERYINTERFACE(        \
    ClassName, SuperClass, ...)                                                \
    uint32_t ClassName::AddRef() { return ++m_uiRefCount; }                    \
    uint32_t ClassName::Release() {                                            \
        uint32_t uiRefCount = --m_uiRefCount;                                  \
        if (uiRefCount == 0) {                                                 \
            delete this;                                                       \
        }                                                                      \
        return uiRefCount;                                                     \
    }                                                                          \
    bool ClassName::QueryInterface(uint32_t uiClsId, void **ppvObj) {      \
        if (ppvObj == NULL) {                                                  \
            return false;                                                          \
        }                                                                      \
        if (uiClsId == 1 FOR_EACH(CLSID_MATCH, __VA_ARGS__)) {                 \
            *ppvObj = this;                                                    \
        } else {                                                               \
            return SuperClass::QueryInterface(uiClsId, ppvObj);                \
        }                                                                      \
        AddRef();                                                              \
        return true;                                                              \
    }
#define IMPL_IGZUNKNOWN_CALL_SUPERCLASS_QUERYINTERFACE_BY_NAME(                \
    ClassName, SuperClass, ...)                                                \
    uint32_t ClassName::AddRef() { return ++m_uiRefCount; }                    \
    uint32_t ClassName::Release() {                                            \
        uint32_t uiRefCount = --m_uiRefCount;                                  \
        if (uiRefCount == 0) {                                                 \
            delete this;                                                       \
        }                                                                      \
        return uiRefCount;                                                     \
    }                                                                          \
    bool ClassName::QueryInterface(uint32_t uiClsId, void **ppvObj) {      \
        if (ppvObj == NULL) {                                                  \
            return false;                                                          \
        }                                                                      \
        *ppvObj = NULL;                                                        \
        FOR_EACH(CLSID_MATCH_BY_NAME, __VA_ARGS__) {                           \
            return SuperClass::QueryInterface(uiClsId, ppvObj);                \
        }                                                                      \
        AddRef();                                                              \
        return true;                                                              \
    }

