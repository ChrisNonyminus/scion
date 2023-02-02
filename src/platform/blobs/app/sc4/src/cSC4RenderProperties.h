#pragma once

#include "cISC4RenderProperties.h"
#include "cRZUnknown.h"

#include <string>
#include <vector>

template<typename T>
struct cSC4RenderProperty {
    T unk0; // assume this is the value
    char* unk4; // name?
    char* unk8;
    T field_C, field_10, field_14;
};

static_assert(sizeof(cSC4RenderProperty<bool>) == 16);

class cSC4RenderProperties : public cISC4RenderProperties, public cRZUnknown {
    //int unk8;
    std::vector<cSC4RenderProperty<bool>> mBooleans;
    std::vector<cSC4RenderProperty<int>> mIntegers;
    std::vector<cSC4RenderProperty<float>> mFloats;
public:
    struct cProperties {
        cProperties();

        static std::vector<cSC4RenderProperty<bool>> m_boolProperties;
        static std::vector<cSC4RenderProperty<int>> m_intProperties;
        static std::vector<cSC4RenderProperty<float>> m_floatProperties;
    };
    cSC4RenderProperties();

public:
    virtual int NumBoolProperties();
    virtual int NumIntProperties();
    virtual int NumFloatProperties();
    virtual bool BoolValue(uint32_t id) const;
    virtual int  IntValue(uint32_t id);
    virtual float FloatValue(uint32_t id);
    virtual void SetBoolValue(uint32_t id, bool value);
    virtual void SetIntValue(uint32_t id, int value);
    virtual void SetFloatValue(uint32_t id, float value);
    virtual uint32_t BoolPropertyIDFromName(const char* name);
    virtual uint32_t IntPropertyIDFromName(const char* name);
    virtual uint32_t FloatPropertyIDFromName(const char* name);
    virtual const char* NameFromBoolPropertyID(uint32_t id);
    virtual const char* NameFromIntPropertyID(uint32_t id);
    virtual const char* NameFromFloatPropertyID(uint32_t id);
public:
	virtual bool QueryInterface(GZIID iid, void** outPtr);
	virtual uint32_t AddRef(void);
	virtual uint32_t Release(void);
};