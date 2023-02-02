#pragma once
#include "cIGZUnknown.h"

class cISC4RenderProperties : public cIGZUnknown {
public:
    virtual int NumBoolProperties() = 0;
    virtual int NumIntProperties() = 0;
    virtual int NumFloatProperties() = 0;
    virtual bool BoolValue(uint32_t id) const = 0;
    virtual int  IntValue(uint32_t id) = 0;
    virtual float FloatValue(uint32_t id) = 0;
    virtual void SetBoolValue(uint32_t id, bool value) = 0;
    virtual void SetIntValue(uint32_t id, int value) = 0;
    virtual void SetFloatValue(uint32_t id, float value) = 0;
    virtual uint32_t BoolPropertyIDFromName(const char* name) = 0;
    virtual uint32_t IntPropertyIDFromName(const char* name) = 0;
    virtual uint32_t FloatPropertyIDFromName(const char* name) = 0;
    virtual const char* NameFromBoolPropertyID(uint32_t id) = 0;
    virtual const char* NameFromIntPropertyID(uint32_t id) = 0;
    virtual const char* NameFromFloatPropertyID(uint32_t id) = 0;
};