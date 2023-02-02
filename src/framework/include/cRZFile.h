//
// Created by chris on 1/17/2023.
//

#ifndef SCION_CRZFILE_H
#define SCION_CRZFILE_H

#include "cRZString.h"

#include <vector>

struct cRZFile {
    cRZString field_4{};   // file path
    int field_10{0};
    FILE *field_14{nullptr};   // file handle
    int field_18{0}, field_1C{0}, field_20{0}, field_24{0},
            field_28{0x20000000};
    cRZString field_2C{};
    size_t field_38{0}, field_3C{0}, field_40{512};
    std::vector<uint8_t> m_abUnkData1{};
    int field_50{0}, field_54{0}, field_58{512};
    std::vector<uint8_t> m_abUnkData2{};
    int field_68{0}, field_6C{0};

    inline bool IsOpen() const { return field_10; }

    void SetPath(const cRZString &path);
    bool Open(int a2, int a3, int a4);
    size_t Length();
    uint32_t SeekToPosition(uint32_t pos);
    uint32_t Seek(uint32_t offset, int whence);
    bool Write(void *src, uint32_t len);
    bool Read(void *dst, uint32_t len);
    bool Close();

    ~cRZFile();

    void FlushWriteBuffer();
};

void AdjustPathSeparatorsForPlatform(const cRZString &in,
                                cRZString &out);


#endif //SCION_CRZFILE_H
