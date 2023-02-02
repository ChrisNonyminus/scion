#include "cSC4RenderProperties.h"

#include <string.h>

namespace {
    // Iterates through each character in a string and, 
    // if it is an uppercase letter, add a lowercase version 
    // of it to the output string.
    void MakeShortcut(const std::string& in, std::string& out) {
        out.clear();
        for (auto& c : in) {
            if (c >= 'A' && c <= 'Z') {
                out.push_back(c + 32);
            }
        }
    }
}

cSC4RenderProperties::cSC4RenderProperties()
{
    cProperties props = cProperties();
    mBooleans = (cProperties::m_boolProperties);
    //cProperties::m_boolProperties.clear();
    mIntegers = (cProperties::m_intProperties);
    //cProperties::m_intProperties.clear();
    mFloats = (cProperties::m_floatProperties);
    //cProperties::m_floatProperties.clear();
    for (auto& b : mBooleans) {
        std::string tmp(b.unk8);
        MakeShortcut(b.unk4, tmp);
        strcpy(b.unk8, tmp.c_str());
        // make b.unk4 lowercase
        char* c = b.unk4;
        while (*c) {
            if (*c >= 'A' && *c <= 'Z') {
                *c += 32;
            }
            ++c;
        }
    }
    for (auto& i : mIntegers) {
        std::string tmp(i.unk8);
        MakeShortcut(i.unk4, tmp);
        strcpy(i.unk8, tmp.c_str());
        // make i.unk4 lowercase
        char* c = i.unk4;
        while (*c) {
            if (*c >= 'A' && *c <= 'Z') {
                *c += 32;
            }
            ++c;
        }
    }
    for (auto& f : mFloats) {
        std::string tmp(f.unk8);
        MakeShortcut(f.unk4, tmp);
        strcpy(f.unk8, tmp.c_str());
        // make f.unk4 lowercase
        char* c = f.unk4;
        while (*c) {
            if (*c >= 'A' && *c <= 'Z') {
                *c += 32;
            }
            ++c;
        }
    }

}
bool cSC4RenderProperties::BoolValue(uint32_t id) const {
  return mBooleans[id].field_C;
}
