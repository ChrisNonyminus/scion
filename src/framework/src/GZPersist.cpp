//
// Created by chris on 1/17/2023.
//

#include "GZPersist.h"
#include "RZStatics.h"

#include "cIGZSerializable.h"

#include "cGZFramework.h"
#include "cRZExceptionNotification.h"

#include <memory.h>

// time.h
#if defined(_WIN32)
#include <time.h>
#else
#include <sys/time.h>
#endif

uint32_t cRZFastCompression3::AddRef() { return ++m_uiRefCount; }
uint32_t cRZFastCompression3::Release() {
    uint32_t uiRefCount = --m_uiRefCount;
    if (uiRefCount == 0) { delete this; }
    return uiRefCount;
}
bool cRZFastCompression3::QueryInterface(uint32_t uiClsId, void **ppvObj) {
    if (ppvObj == 0) { return false; }
    *ppvObj = 0;
    if (uiClsId == GZIID_cIGZCompressor) { *ppvObj = static_cast<cIGZCompressor *>(this); } else { return 0; }
    AddRef();
    return true;
}

int cRZFastCompression3::decoderef(char *a1, int32_t *a2, char *a3) {
    char *v3;              // ecx
    char *v5;              // ecx
    char *i;               // edi
    char v7;               // al
    char *v8;              // edx
    int j;                 // ecx
    char *v10;             // ebx
    char *v11;             // edx
    unsigned __int8 v12;   // dl
    char *v13;             // edi
    int k;                 // ecx
    char *v15;             // edx
    char *v16;             // ecx
    char *v17;             // ecx
    int m;                 // edx
    char *v19;             // ebx
    char *v20;             // edx
    unsigned int v21;      // ebx
    int n;                 // edx
    int ii;                // edx
    char v25;              // [esp+0h] [ebp-28h]
    unsigned __int8 v26;   // [esp+11h] [ebp-17h]
    unsigned __int8 v27;   // [esp+12h] [ebp-16h]
    unsigned __int8 v28;   // [esp+13h] [ebp-15h]
    int v29;               // [esp+14h] [ebp-14h]
    unsigned __int8 v30;   // [esp+1Ah] [ebp-Eh]
    unsigned __int8 v31;   // [esp+1Bh] [ebp-Dh]

    v3 = a1;
    if (a1) {
        v5 = a1 + 2;
        if ((*a1 & 1) != 0) v5 = a1 + 5;
        v29 = v5[2] + ((v5[1] + (*v5 << 8)) << 8);
        for (i = v5 + 3;; i = &v3[n]) {
            while (1) {
                while (1) {
                    while (1) {
                        v7 = *i;
                        v25 = *i;
                        v3 = i + 1;
                        if (*i < 0) break;
                        v30 = i[1];
                        v8 = i + 2;
                        for (j = 0; j != (v7 & 3); ++j)
                            *a3++ = v8[j];
                        i = &v8[j];
                        v10 = &a3[-v30 - 1 + -8 * (v25 & 0x60)];
                        v11 = a3;
                        a3 += ((v25 & 0x1C) >> 2) + 3;
                        do
                            *v11++ = *v10++;
                        while (a3 != v11);
                    }
                    if ((v25 & 0x40) != 0) break;
                    v12 = i[1];
                    v31 = i[2];
                    v13 = i + 3;
                    for (k = 0; k != v12 >> 6; ++k)
                        *a3++ = v13[k];
                    i = &v13[k];
                    v15 = &a3[-v31 - 1 + -256 * (v12 & 0x3F)];
                    v16 = a3;
                    do
                        *v16++ = *v15++;
                    while (&a3[(v25 & 0x3F) + 4] != v16);
                    a3 += (v25 & 0x3F) + 4;
                }
                if ((v25 & 0x20) != 0) break;
                v26 = i[1];
                v27 = i[2];
                v28 = i[3];
                v17 = i + 4;
                for (m = 0; m != (v25 & 3); ++m)
                    *a3++ = v17[m];
                i = &v17[m];
                v19 = &a3[-v27 - 1 + -256 * v26 + -65536 * ((v25 & 0x10) >> 4)];
                v20 = a3;
                do
                    *v20++ = *v19++;
                while (v20 != &a3[64 * (v25 & 0xC) + 5 + v28]);
                a3 = v20;
            }
            v21 = 4 * (v25 & 0x1F) + 4;
            if (v21 > 0x70) break;
            for (n = 0; v21 != n; ++n)
                *a3++ = v3[n];
        }
        for (ii = 0; (v25 & 3) != ii; ++ii)
            a3[ii] = *v3++;
    } else {
        v29 = 0;
    }
    if (a2) *a2 = v3 - a1;
    return v29;
}
int cRZFastCompression3::encoderef(char *a1, int32_t a2, char *a3,
                                   const int *a4) {
    int v4;     // edi
    char *v5;   // edx
    int v6;     // ebx
    int v7;     // ecx
    char *v8;   // ecx
    int v9;     // edx
    int v10;    // ebx

    v4 = *a4 & 1;
    v5 = a3;
    v6 = 284884992;
    v7 = 2;
    do {
        *v5++ = /*HIBYTE*/ (v6);
        v6 <<= 8;
        --v7;
    } while (v7);
    v8 = a3 + 2;
    v9 = a2 << 8;
    v10 = 3;
    do {
        *v8++ = /*HIBYTE*/ (v9);
        v9 <<= 8;
        --v10;
    } while (v10);
    return cRZFastCompression3::refpack((unsigned __int8 *)a1, a2,
                                        (unsigned __int8 *)a3 + 5, (int)131072,
                                        v4) +
           5;
}
int cRZFastCompression3::refpack(unsigned char *a1, int32_t a2,
                                 unsigned char *a3, int32_t a4, int32_t a5) {
    int32_t *v5;              // eax
    int v6;                   // edx
    unsigned __int8 *v7;      // esi
    unsigned __int8 *v8;      // edi
    int v9;                   // edx
    int v10;                  // eax
    int v11;                  // ecx
    unsigned __int8 *v12;     // edx
    unsigned int i;           // ebx
    unsigned int v14;         // ebx
    char *v15;                // esi
    int v16;                  // ebx
    unsigned __int8 *v17;     // edi
    unsigned int v18;         // ebx
    char *v19;                // esi
    unsigned __int8 *v20;     // ebx
    int32_t *v22;             // [esp+2Ch] [ebp-6Ch]
    int32_t *v23;             // [esp+40h] [ebp-58h]
    int32_t *v24;             // [esp+44h] [ebp-54h]
    int *v25;                 // [esp+48h] [ebp-50h]
    int v26;                  // [esp+4Ch] [ebp-4Ch]
    int v27;                  // [esp+50h] [ebp-48h]
    uint32_t __n;             // [esp+54h] [ebp-44h]
    unsigned int v29;         // [esp+58h] [ebp-40h]
    int v30;                  // [esp+5Ch] [ebp-3Ch]
    unsigned int v31;         // [esp+60h] [ebp-38h]
    unsigned int v32;         // [esp+64h] [ebp-34h]
    unsigned int v33;         // [esp+68h] [ebp-30h]
    unsigned __int8 *v34;     // [esp+6Ch] [ebp-2Ch]
    unsigned __int8 *__src;   // [esp+70h] [ebp-28h]
    signed int j;             // [esp+78h] [ebp-20h]
    int v37;                  // [esp+7Ch] [ebp-1Ch]

    v24 = static_cast<int32_t *>(malloc(0x40000u));
    if (!v24) return 0;
    v23 = static_cast<int32_t *>(malloc(0x80000u));
    if (!v23) return 0;
    v5 = v24;
    v6 = 4096;
    do {
        v5[15] = -1;
        v5[14] = -1;
        v5[13] = -1;
        v5[12] = -1;
        v5[11] = -1;
        v5[10] = -1;
        v5[9] = -1;
        v5[8] = -1;
        v5[7] = -1;
        v5[6] = -1;
        v5[5] = -1;
        v5[4] = -1;
        v5[3] = -1;
        v5[2] = -1;
        v5[1] = -1;
        *v5 = -1;
        v5 += 16;
        --v6;
    } while (v6);
    v7 = a3;
    __src = a1;
    v8 = a1;
    __n = 0;
    LABEL_53:
    if (a2 > 0) {
        v9 = 1028;
        if (a2 < 1029) v9 = a2;
        v33 = v9;
        v34 = v8 + 1;
        v25 = &v24[(16 * v8[1]) ^ (v8[2] | (*v8 << 8))];
        v37 = *v25;
        v26 = v8 - a1;
        v10 = 0;
        if (v8 - a1 - 0x1FFFF >= 0) v10 = v8 - a1 - 0x1FFFF;
        if (v37 < v10) goto LABEL_33;
        v11 = *v25;
        v30 = 0;
        v32 = 2;
        v31 = 2;
        while (1) {
            v12 = &a1[v11];
            if (v8[v31] != a1[v11 + v31]) goto LABEL_29;
            for (i = 0; v33 > i && v8[i] == v12[i]; ++i)
                ;
            if (v31 >= i) goto LABEL_29;
            v29 = v8 - v12 - 1;
            if (v29 > 0x3FF) {
                if (v29 > 0x3FFF) goto LABEL_25;
            } else if (i <= 0xA) {
                v27 = 2;
                goto LABEL_26;
            }
            if (i <= 0x43) {
                v27 = 3;
                goto LABEL_26;
            }
            LABEL_25:
            v27 = 4;
            LABEL_26:
            if (i - v27 + 4 > v31 - v32 + 4) {
                if (i > 0x403) {
                    v30 = v8 - v12 - 1;
                    v32 = v27;
                    v31 = i;
                    LABEL_32:
                    if (v31 > v32) {
                        while (__n > 3) {
                            v14 = __n & 0xFFFFFFFC;
                            if ((__n & 0xFFFFFFFC) >= 0x71) v14 = 112;
                            __n -= v14;
                            *v7 = (v14 >> 2) - 33;
                            v15 = ((char *)v7 + 1);
                            memcpy(v15, __src, v14);
                            __src += v14;
                            v7 = &((unsigned char *)v15)[v14];
                        }
                        if (v32 == 2) {
                            *v7 = __n + 32 * /*BYTE1*/ (v30) + 4 * v31 - 12;
                            v7[1] = v30;
                            v7 += 2;
                        } else if (v32 == 3) {
                            *v7 = v31 + 124;
                            v7[1] = /*BYTE1*/ (v30) + (__n << 6);
                            v7[2] = v30;
                            v7 += 3;
                        } else {
                            *v7 = 16 * /*BYTE2*/ (v30) + 4 * ((v31 - 5) >> 8) +
                                  __n - 64;
                            v7[1] = /*BYTE1*/ (v30);
                            v7[2] = v30;
                            v7[3] = v31 - 5;
                            v7 += 4;
                        }
                        if (__n) {
                            memcpy(v7, __src, __n);
                            v7 += __n;
                            __n = 0;
                        }
                        if (a5) {
                            v23[v26 & 0x1FFFF] = *v25;
                            *v25 = v26;
                            v17 = &v8[v31];
                        } else {
                            v16 = v8 - a1;
                            for (j = 0; j < v31; ++j) {
                                v22 = &v24[(16 * v8[j + 1]) ^
                                           (v8[j + 2] | (v8[j] << 8))];
                                v23[v16 & 0x1FFFF] = *v22;
                                *v22 = v16++;
                            }
                            v17 = &v8[j];
                        }
                        v34 = v17;
                        a2 -= v31;
                        __src = v17;
                    } else {
                        LABEL_33:
                        v23[v26 & 0x1FFFF] = v37;
                        *v25 = v26;
                        ++__n;
                        --a2;
                    }
                    v8 = v34;
                    goto LABEL_53;
                }
                v30 = v8 - v12 - 1;
                v32 = v27;
                v31 = i;
            }
            LABEL_29:
            v11 = v23[v11 & 0x1FFFF];
            if (v10 > v11) goto LABEL_32;
        }
    }
    while (__n > 3) {
        v18 = __n & 0xFFFFFFFC;
        if ((__n & 0xFFFFFFFC) >= 0x71) v18 = 112;
        __n -= v18;
        *v7 = (v18 >> 2) - 33;
        v19 = ((char *)v7 + 1);
        memcpy(v19, __src, v18);
        __src += v18;
        v7 = &((unsigned char *)v19)[v18];
    }
    *v7 = __n - 4;
    v20 = v7 + 1;
    if (__n) {
        memcpy(v7 + 1, __src, __n);
        v20 += __n;
    }
    free(v23);
    free(v24);
    return (v20 - a3);
}

bool cRZFastCompression3::DecompressData(const void *src, size_t srcLen,
                                         void *dst, size_t *dstLen) {
    int32_t srcLen2 = srcLen;
    int v5 = decoderef((char *)src + 4, &srcLen2, (char *)dst);
    if (v5) {
        *dstLen = v5;
        return true;
    }
    return false;
}
void cRZFastCompression3::CompressData(const void *src, size_t srcLen,
                                       void *dst, size_t *dstLen) {
    int32_t srcLen2 = srcLen;
    int v5 = encoderef((char *)src, srcLen2, (char *)dst + 4,
                       (const int *)&field_8) +
             4;
    if (v5) {
        *dstLen = v5;
        return;
    }
}
void cRZFastCompression3::GetCompressionVersionString(cRZString &str) {}
int cRZFastCompression3::isref(char *a1) {
    char *v1;   // ecx
    int v2;     // ebx
    int i;      // eax
    char *v4;   // ecx
    int v5;     // ebx
    int j;      // eax

    v1 = a1;
    v2 = 2;
    for (i = 0;; i = (i << 8) + (unsigned __int8)*(v1 - 1)) {
        --v2;
        ++v1;
        if (v2 == -1) break;
    }
    if (i == 4347) return 100;
    v4 = a1;
    v5 = 2;
    for (j = 0;; j = (j << 8) + (unsigned __int8)*(v4 - 1)) {
        --v5;
        ++v4;
        if (v5 == -1) break;
    }
    if (j == 4603)
        return 100;
    else
        return 0;
}
bool cRZFastCompression3::ValidateCompressedData(const unsigned char *data,
                                                 uint32_t len) {
    if (len <= 6) return false;
    if (!isref((char *)data)) return false;
    return true;
}
size_t
cRZFastCompression3::GetMaxLengthRequiredForCompressedData(size_t srcLen) {
    return 0;
}
size_t cRZFastCompression3::GetLengthOfDecompressedData(const void *src) {
    return sizeref((char *)src + 4);
}
size_t
cRZFastCompression3::GetMaxLengthRequiredForDecompressedData(const void *src) {
    return 0;
}
int cRZFastCompression3::sizeref(char *ref) {
    char *v1;         // ecx
    int v2;           // ebx
    __int32 result;   // eax
    char *v4;         // ecx
    int v5;           // ebx
    char *v6;         // ecx
    int v7;           // ebx

    v1 = ref;
    v2 = 2;
    for (result = 0;; result = (result << 8) + (unsigned __int8)*(v1 - 1)) {
        --v2;
        ++v1;
        if (v2 == -1) break;
    }
    if (result == 4347) {
        v4 = ref + 2;
        v5 = 3;
        result = 0;
        while (1) {
            --v5;
            ++v4;
            if (v5 == -1) break;
            result = (result << 8) + (unsigned __int8)*(v4 - 1);
        }
    } else {
        v6 = ref + 5;
        result = 0;
        v7 = 4;
        while (1) {
            ++v6;
            if (!--v7) break;
            result = (result << 8) + (unsigned __int8)*(v6 - 1);
        }
    }
    return result;
}

uint32_t cGZPersistDBSegment::AddRef() { return ++m_uiRefCount; }
uint32_t cGZPersistDBSegment::Release() {
    uint32_t uiRefCount = --m_uiRefCount;
    if (uiRefCount == 0) { delete this; }
    return uiRefCount;
}
bool cGZPersistDBSegment::QueryInterface(uint32_t uiClsId, void **ppvObj) {
    if (ppvObj == 0) { return false; }
    *ppvObj = 0;
    if (uiClsId == GZIID_cIGZPersistDBSegment) { *ppvObj = static_cast<cIGZPersistDBSegment *>(this); }
    else { return 0; }
    AddRef();
    return true;
}
bool cGZPersistDBSegment::CloseRecord(cIGZPersistDBRecord *pRecord) {
    return false;
}
bool cGZPersistDBSegment::Init() {
    if (!field_14) field_14 = 1;
    return true;
}
uint32_t cGZPersistDBSegmentPackedFile::AddRef() { return ++m_uiRefCount; }
uint32_t cGZPersistDBSegmentPackedFile::Release() {
    uint32_t uiRefCount = --m_uiRefCount;
    if (uiRefCount == 0) { delete this; }
    return uiRefCount;
}
bool cGZPersistDBSegmentPackedFile::QueryInterface(uint32_t uiClsId, void **ppvObj) {
    if (ppvObj == 0) { return false; }
    *ppvObj = 0;
    if (uiClsId ==
        GZIID_cIGZPersistDBSegmentPackedFile) { *ppvObj = static_cast<cIGZPersistDBSegmentPackedFile *>(this); }
    else { return cGZPersistDBSegment::QueryInterface(uiClsId, ppvObj); }
    AddRef();
    return true;
}
void cGZPersistDBSegmentPackedFile::SetPath(const cIGZString &path) {
    m_oCriticalSection.Lock();
    if ((flags & 3) != 0) return;
    field_2C = path;
    m_oCriticalSection.Unlock();
}
/*int __cdecl cGZDBSegmentPackedFile::Open(
cGZDBSegmentPackedFile *this,
bool a2,
bool a3)
{
int v3; // esi
int v4; // esi
int v6; // [esp+1Ch] [ebp-1Ch]

if ( (this->flags & 3) != 0
|| !*(this->field_2C.str - 12)
|| !this->field_14
|| !a2 && !a3 )
{
return 0;
}
this->flags = 0;
if ( a2 )
{
this->flags = 1;
v3 = 1;
}
else
{
v3 = 0;
}
if ( a3 )
{
this->flags |= 3u;
v3 |= 3u;
v6 = 3;
}
else
{
v6 = 2;
}
cRZFile::SetPath(&this->file, &this->field_2C);
if ( !cRZFile::Open(&this->file, v3, v6, 1) )
{
this->flags = 0;
return 0;
}
if ( cRZFile::Length(&this->file)
|| (this->flags & 2) != 0
   && cGZDBSegmentPackedFile::WriteEmptyHeaderRecord(this) )
{
if ( cGZDBSegmentPackedFile::ReadHeaderRecord(this) )
{
if ( !cGZDBSegmentPackedFile::ReadIndexRecord(this)
    || !cGZDBSegmentPackedFile::ReadHoleRecord(this)
    || !cGZDBSegmentPackedFile::ReadCompressedSetRecord(this) )
{
    goto LABEL_26;
}
}
else
{
if ( (this->flags & 2) == 0
    || !cGZDBSegmentPackedFile::WriteEmptyHeaderRecord(this) )
{
    goto LABEL_26;
}
if ( !this->field_24 )
{
    v4 = operator new(0x14u);
    __gnu_cxx::hash_map<cGZDBSegmentPackedFile::KeyTGI,cGZDBSegmentPackedFile::RecordDataInfo,cGZDBSegmentPackedFile::KeyTGIHash,std::equal_to<cGZDBSegmentPackedFile::KeyTGI>,std::allocator<cGZDBSegmentPackedFile::RecordDataInfo>>::hash_map(v4);
    this->field_24 = v4;
}
}
return 1;
}
LABEL_26:
cRZFile::Close(&this->file);
return 0;
}*/
bool cGZPersistDBSegmentPackedFile::Open(bool a2, bool a3) {
    if ((flags & 3) != 0) return false;
    if (field_2C.Strlen() == 0) return false;
    if (!field_14) return false;
    if (!a2 && !a3) return false;

    flags = (int)a2;
    int v3 = (int)a2;
    int v6 = 2;

    if (a3) {
        flags |= 3;
        v3 |= 3;
        v6 = 3;
    }
    m_oFile.SetPath(field_2C);
    if (!m_oFile.Open(v3, v6, 1)) {
        flags = 0;
        return false;
    }
    if (m_oFile.Length() ||
        (flags & 2) != 0 && this->WriteEmptyHeaderRecord()) {
        if (this->ReadHeaderRecord()) {
            if (!this->ReadIndexRecord() || !this->ReadHoleRecord() ||
                !this->ReadCompressedSetRecord()) {
                goto LABEL_26;
            }
        } else {
            if ((flags & 2) == 0 || !this->WriteEmptyHeaderRecord()) {
                goto LABEL_26;
            }
            if (!m_oIndexTable) {
                m_oIndexTable = new std::map<KeyTGI, RecordDataInfo>;
            }
        }
        return true;
    }
    LABEL_26:
    m_oFile.Close();
    return false;
}
uint32_t kPackedFileHeaderFileIdentifier = MAKE_FOURCC("DBPF");
bool cGZPersistDBSegmentPackedFile::WriteEmptyHeaderRecord() {
    memset(&m_oHeader, 0, sizeof(m_oHeader));
    m_oHeader.m_nMagic = kPackedFileHeaderFileIdentifier;
#if defined(APP_IS_SC4)
    m_oHeader.m_nVersion = 0x00000001;
#else
    m_oHeader.m_nVersion = 0x00000002;
#endif
    m_oHeader.m_nMinorVersion = 0;
    m_oHeader.field_C = field_B4;
    m_oHeader.field_10 = field_B8;
    m_oHeader.field_14 = 0;
    m_oHeader.m_nDateCreated = time(NULL);
    m_oHeader.m_nDateModified = m_oHeader.m_nDateCreated;
#if defined(APP_IS_SC4)
    m_oHeader.m_nIndexMajorVersion = 7;
    m_oHeader.m_nIndexMinorVersion = 0;
#else
    m_oHeader.m_nIndexMajorVersion = 7;
    m_oHeader.m_nIndexMinorVersion = 1;
#endif
    m_oHeader.m_nIndexEntryCount = 0;
    m_oHeader.m_nIndexTableOffset = 0;
    m_oHeader.m_nIndexTableSize = 0;
    m_oHeader.m_nHoleEntryCount = 0;
    m_oHeader.m_nHoleTableOffset = 0;
    m_oHeader.m_nHoleTableSize = 0;

    return this->WriteHeaderRecord();
}
bool cGZPersistDBSegmentPackedFile::WriteHeaderRecord() {
    if (flags & 2) {
        if (!m_oFile.field_10) return false;
        if (m_oFile.SeekToPosition(field_A8) != field_A8) return false;
        m_oHeader.m_nDateModified = time(NULL);
        if (m_oFile.Write((void *)&m_oHeader, 0x60)) return true;
    }
    return false;
}
bool cGZPersistDBSegmentPackedFile::ReadHeaderRecord() {
    static bool bHereAlready;
    if (m_oFile.SeekToPosition(field_A8) != field_A8) return false;
    if (!m_oFile.Read((void *)&m_oHeader, 0x60)) return false;

    return this->VerifyHeaderRecordIntegrity();
}
bool cGZPersistDBSegmentPackedFile::VerifyHeaderRecordIntegrity() {
    if (m_oHeader.m_nMagic != kPackedFileHeaderFileIdentifier) return false;
    if (m_oHeader.m_nVersion != 1 && m_oHeader.m_nVersion != 2) return false;
    if (m_oHeader.m_nIndexEntryCount > 0x7FFFFFEu) return false;
    if (m_oHeader.m_nIndexTableOffset >= m_oFile.Length()) return false;
    if (m_oHeader.m_nIndexTableSize + m_oHeader.m_nIndexTableOffset >
        m_oFile.Length())
        return false;
    if (m_oHeader.m_nHoleEntryCount > 0x7FFFFFEu) return false;
    if (m_oHeader.m_nHoleTableOffset >= m_oFile.Length()) return false;
    if (m_oHeader.m_nHoleTableSize + m_oHeader.m_nHoleTableOffset >
        m_oFile.Length())
        return false;
    return true;
}
bool cGZPersistDBSegmentPackedFile::ReadIndexRecord() {
    if (m_oHeader.m_nIndexEntryCount > 0) {
        if (m_oFile.SeekToPosition(m_oHeader.m_nOffsetOfFirstIndexEntry +
                                   field_A8) !=
            m_oHeader.m_nOffsetOfFirstIndexEntry + field_A8) {
            return false;
        }
        RecordDataInfo *infoArray =
                new RecordDataInfo[m_oHeader.m_nIndexEntryCount];
        int v3 = 0;
        do {
            ++v3;
        } while (m_oHeader.m_nIndexEntryCount + 1 != v3);
        if (!m_oFile.Read((void *)infoArray,
                          sizeof(RecordDataInfo) *
                          m_oHeader.m_nIndexEntryCount)) {
            delete[] infoArray;
            return false;
        }
        if (!m_oIndexTable) {
            m_oIndexTable = new std::map<KeyTGI, RecordDataInfo>;
        }
        for (int i = 0; i < m_oHeader.m_nIndexEntryCount; ++i) {
            m_oIndexTable->insert(std::pair<KeyTGI, RecordDataInfo>(
                    infoArray[i].m_oKey, infoArray[i]));
            this->IncrementTypeAndGroupUse(infoArray[i].m_oKey.m_nType,
                                           infoArray[i].m_oKey.m_nGroup, true);
        }
        if (this->VerifyIndexRecordIntegrity()) {
            field_28 = 0;
            delete[] infoArray;
            return true;
        }
        delete[] infoArray;
    }
    return false;
}
void cGZPersistDBSegmentPackedFile::IncrementTypeAndGroupUse(uint32_t nType,
                                                             uint32_t nGroup,
                                                             bool b) {
    if (field_15A) {
        if (field_15C.count(nType)) {
            field_15C[nType] += 1;
        } else {
            field_15C[nType] = 1;
        }
        if (field_170.count(nGroup)) {
            field_170[nGroup] += 1;
        } else {
            field_170[nGroup] = 1;
        }
    }
}
/*
 * Function to verify the integrity of an index record:
 * - Declare a variable `record_length` and set it equal to the length of the
 * file
 * - Iterate through each record in the hash table (stored in `recordDataInfo`)
 * - If size of current record is less than or equal to 95 OR if `record_length`
 * is less than size of current record OR if `record_length` is less than
 * location plus size, return 0 (verification failed)
 * - Otherwise, move on to next item in hash table
 * - Return 1 (verification successful)
 */
bool cGZPersistDBSegmentPackedFile::VerifyIndexRecordIntegrity() {
    uint32_t record_length = m_oFile.Length();
    for (auto &record : *m_oIndexTable) {
        if (record.second.m_nSize <= 95 ||
            record_length < record.second.m_nSize ||
            record_length <
            record.second.m_nLocationInArchive + record.second.m_nSize) {
            return false;
        }
    }
    return true;
}
bool cGZPersistDBSegmentPackedFile::ReadHoleRecord() {
    if (m_oHeader.m_nHoleEntryCount > 0) {
        if (m_oFile.SeekToPosition(m_oHeader.m_nHoleTableOffset + field_A8) !=
            m_oHeader.m_nHoleTableOffset + field_A8) {
            return false;
        }
        HoleRecord *holeArray = new HoleRecord[m_oHeader.m_nHoleEntryCount];
        int v3 = 0;
        do {
            ++v3;
        } while (m_oHeader.m_nHoleEntryCount + 1 != v3);
        if (!m_oFile.Read((void *)holeArray,
                          sizeof(HoleRecord) * m_oHeader.m_nHoleEntryCount)) {
            delete[] holeArray;
            return false;
        }
        for (int i = 0; i < m_oHeader.m_nHoleEntryCount; ++i) {
            m_oHoleRecord.insert(holeArray[i]);
        }
        delete[] holeArray;
        if (this->VerifyHoleRecordIntegrity()) {
            field_158 = false;
            return true;
        }
    }
    return false;
}
bool cGZPersistDBSegmentPackedFile::VerifyHoleRecordIntegrity() {
    uint32_t record_length = m_oFile.Length();
    for (auto &record : m_oHoleRecord) {
        if (record_length < record.m_nSize ||
            record_length < record.m_nOffset + record.m_nSize) {
            return false;
        }
    }
    return true;
}
size_t
cGZPersistDBSegmentPackedFile::DoReadRecord(const cGZPersistResourceKey &key,
                                            void *buffer, uint32_t &size) {
    if ((flags == 3) == 0 || (flags & 1) == 0) {
        return -1;
    }
    if (m_oIndexTable->count(key) == 0) {
        return -1;
    }
    RecordDataInfo record = (*m_oIndexTable)[key];
    bool isCompressed = (field_1B0.count(key) != 0);
    if (isCompressed) {
        size = this->GetDecompressedRecordLength(key);
    } else {
        size = record.m_nSize;
    }
    if (!isCompressed) {
        if (m_oFile.SeekToPosition(record.m_nLocationInArchive + field_A8) !=
            record.m_nLocationInArchive + field_A8) {
            return -1;
        }
        if (!m_oFile.Read(buffer, size)) {
            return -1;
        }
    } else {
        if (m_oFile.SeekToPosition(record.m_nLocationInArchive + field_A8) !=
            record.m_nLocationInArchive + field_A8) {
            return -1;
        }
        uint8_t *compressedBuffer = new uint8_t[record.m_nSize];
        if (!m_oFile.Read(compressedBuffer, record.m_nSize)) {
            delete[] compressedBuffer;
            return -1;
        }
        if (!this->DecompressData(compressedBuffer, record.m_nSize, buffer,
                                  size)) {
            delete[] compressedBuffer;
            return -1;
        }
        delete[] compressedBuffer;
    }
    return size;
}
uint32_t cGZPersistDBSegmentPackedFile::GetDecompressedRecordLength(
        const cGZPersistResourceKey &key) {
    if (field_1B0.count(key) == 0) {
        return 0;
    }
    return field_1B0[key].m_nDecompressedSize;
}
bool cGZPersistDBSegmentPackedFile::ReadCompressedSetRecord() {
    cGZPersistResourceKey skDIR{0xE86B1EEF, 0xE86B1EEF, 0x286B1F03};
    uint32_t dirSize;
    int v1 = (int)DoReadRecord(skDIR, NULL, dirSize);
    if (v1 == -1) return true;
    uint8_t *a3 = new uint8_t[v1 + 4];
    if (DoReadRecord(skDIR, a3, dirSize)) {
        uint32_t v10 = dirSize >> 4;   // dirSize divided by 16?
        CompressedRecordData *dataArray = (CompressedRecordData *)a3;
        for (uint32_t i = 0; i < v10; ++i) {
            field_1B0.insert(
                    {{dataArray[i].m_oKey.m_nType, dataArray[i].m_oKey.m_nGroup,
                      dataArray[i].m_oKey.m_nInstance},
                     dataArray[i]});
        }
        delete[] a3;
        return true;
    }
    delete[] a3;
    return false;
}
bool cGZPersistDBSegmentPackedFile::DecompressData(uint8_t *abComprData,
                                                   uint32_t ulComprLen,
                                                   void *&rpvUncomprData,
                                                   uint32_t &rulUncomprLen) {
    cRZFastCompression3 fastCompression;
    if (fastCompression.ValidateCompressedData(abComprData, ulComprLen)) {
        bool v9 = rpvUncomprData == NULL;
        size_t decomprLen =
                fastCompression.GetLengthOfDecompressedData(abComprData);
        if (v9) {
            rulUncomprLen = decomprLen;
            rpvUncomprData = (void *)(new uint8_t[decomprLen]);
        }
        if (rpvUncomprData && decomprLen <= rulUncomprLen) {
            if (!ulComprLen || !rulUncomprLen ||
                fastCompression.DecompressData(abComprData, ulComprLen,
                                               rpvUncomprData, &decomprLen))
                return true;
            rulUncomprLen = 0;
            if (v9) {
                delete[] abComprData;
                abComprData = NULL;
            }
        }
    }
    return false;
}
bool cGZPersistDBSegmentPackedFile::OpenRecord(const cGZPersistResourceKey &key,
                                               cIGZPersistDBRecord **ppRecord,
                                               uint32_t a4) {
    m_oCriticalSection.Lock();
    bool ret = DoOpenRecord(key, ppRecord, a4);
    m_oCriticalSection.Unlock();
    return ret;
}
bool cGZPersistDBSegmentPackedFile::DoOpenRecord(
        const cGZPersistResourceKey &key, cIGZPersistDBRecord **ppRecord,
        uint32_t a4) {
    if ((flags & 3) == 0) return false;
    cGZPersistDBSerialRecord *ret;
    if ((a4 & 2) == 0) {
        if ((a4 & 1) != 0) {
            auto it = m_oRecords.find(key);
            if (it == m_oRecords.end()) {
                auto rdiIt = m_oIndexTable->find(key);
                if (rdiIt != m_oIndexTable->end()) {
                    uint32_t recordSize = (*rdiIt).second.m_nSize;
                    auto crdIt = field_1B0.find(key);
                    if (crdIt == field_1B0.end()) {
                        if (recordSize > field_BC) {
                            ret = new cGZDBReadRecordPackedFile(
                                    (*rdiIt).second.m_nLocationInArchive,
                                    recordSize, key, this, true);
                        } else {
                            uint8_t *buf = new uint8_t[recordSize];
                            if (!ReadFileSpan(
                                    buf, recordSize,
                                    (*rdiIt).second.m_nLocationInArchive)) {
                                if (buf) delete[] buf;
                                return false;
                            }
                            ret = new cGZDBReadRecordRAM(buf, recordSize, key,
                                                         this, true);
                        }
                    } else {
                        uint32_t decompressedSize =
                                (*crdIt).second.m_nDecompressedSize;
                        uint8_t *compr = new uint8_t[(*rdiIt).second.m_nSize];
                        if (!ReadFileSpan(
                                compr, (*rdiIt).second.m_nSize,
                                (*rdiIt).second.m_nLocationInArchive)) {
                            if (compr) delete[] compr;
                            return false;
                        }
                        void *decompr = new uint8_t[decompressedSize];
                        if (!DecompressData(compr, (*rdiIt).second.m_nSize,
                                            decompr, decompressedSize)) {
                            if (compr) delete[] compr;
                            if (decompr) delete[] decompr;
                            return false;
                        }
                        ret = new cGZDBReadRecordRAM((uint8_t *)decompr,
                                                     decompressedSize, key,
                                                     this, true);
                    }
                    m_oRecords.insert({key, ret});
                } else {
                    return false;
                }
            } else {
                ret = (*it).second;
            }
        }
    }
    *ppRecord = ret;
    return true;
}
bool cGZPersistDBSegmentPackedFile::CloseRecord(cIGZPersistDBRecord *pRecord) {
    return false;
}
bool cGZPersistDBSegmentPackedFile::ReadFileSpan(void *pBuffer, uint32_t nSize,
                                                 uint32_t nOffset) {
    m_oCriticalSection.Lock();
    if (m_oFile.SeekToPosition(nOffset) != nOffset) {
        m_oCriticalSection.Unlock();
        return false;
    }
    if (!m_oFile.Read(pBuffer, nSize)) {
        m_oCriticalSection.Unlock();
        return false;
    }
    m_oCriticalSection.Unlock();
    return true;
}
bool cGZPersistDBSegmentPackedFile::WriteFileSpan(const void *data,
                                                  uint32_t size, uint32_t pos) {
    m_oCriticalSection.Lock();
    if (m_oFile.SeekToPosition(pos) != pos) {
        m_oCriticalSection.Unlock();
        return false;
    }
    if (!m_oFile.Write(const_cast<void *>(data), size)) {
        m_oCriticalSection.Unlock();
        return false;
    }
    m_oCriticalSection.Unlock();
    return true;
}
bool cGZPersistDBSegmentPackedFile::DoOpenRecordForWrite(
        const cGZPersistResourceKey &key, cIGZPersistDBRecord **ppRecord) {
    return false;
}
bool cGZPersistDBSegmentPackedFile::DoCreateNewRecord(
        const cGZPersistResourceKey &key, cIGZPersistDBRecord **ppRecord) {
    return false;
}
void cGZPersistDBSegmentPackedFile::Close() {
    m_oCriticalSection.Lock();
    if (m_oFile.IsOpen()) {
        m_oFile.Close();
    }
    m_oCriticalSection.Unlock();
}
void cGZPersistDBSegmentPackedFile::Shutdown() {
    m_oCriticalSection.Lock();
    if (m_oFile.IsOpen()) {
        m_oFile.Close();
    }
    m_oCriticalSection.Unlock();
}

bool cSC4DBSegmentOStream::Open(cIGZPersistDBSegment *segment,
                                const cGZPersistResourceKey &key, bool a3) {
    if (!Close()) return false;
    if (segment) {
        field_14 = segment;
        cIGZPersistDBSegment *v4 = field_14;
        cIGZPersistDBRecord *v15;
        if (!v4->OpenRecord(key, &v15, 2)) {
            // create the record instead
            if (v15) v15->Release();
            if (!v4->CreateNewRecord(key, &v15)) return false;
        }
        if (!v15) return false;
        field_10 = (cGZPersistDBSerialRecord *)v15;
    }
    field_C = field_10 != NULL;
    return field_C;
}
bool cSC4DBSegmentOStream::Close() { return false; }
bool cSC4DBSegmentOStream::IsOpen() { return false; }
cIGZPersistDBRecord *cSC4DBSegmentOStream::GetRecord() {
    return field_10;
}
cIGZPersistDBSegment *cSC4DBSegmentOStream::GetSegment() { return nullptr; }
bool cSC4DBSegmentOStream::WriteGZSerializable(
        const cIGZSerializable *serializable) {
    if (field_C && serializable->Write(*this)) {
        field_C = true;
    } else {
        field_C = false;
    }
    return field_C;
}
bool cSC4DBSegmentOStream::WriteResKey(const cGZPersistResourceKey &key) {
    if (field_C) {
        if (!field_10->SetFieldUint32(key.m_nTypeID)) {
            field_C = false;
            return false;
        }
        if (!field_10->SetFieldUint32(key.m_nGroupID)) {
            field_C = false;
            return false;
        }
        if (!field_10->SetFieldUint32(key.m_nInstanceID)) {
            field_C = false;
            return false;
        }
    }
    return field_C;
}
bool cSC4DBSegmentOStream::WriteVariant(cIGZVariant *variant) {
    auto type = variant->GetType();
    bool success = false;
    if (type == tagRZVariant::IGZUnknown)
        success = SetUint32(0xA9BEE9AC);
    else
        success = SetUint32(0);
    if (!success) {
        field_C = false;
        return false;
    }
    if (type == tagRZVariant::IGZUnknown) {
        cIGZSerializable *v7;
        if (variant->GetValUnknown(0xE4FDA3D4, (void **)&v7)) {
            success = WriteGZSerializable(v7);
            v7->Release();
        }
    } else {
        success = field_10->SetFieldVariant(*variant);
    }
    field_C = success;
    return success;
}
void cSC4DBSegmentOStream::Flush() {  }
bool cSC4DBSegmentOStream::SetSint8(int8_t value) {
    if (field_C) {
        if (!field_10->SetFieldSint8(value)) {
            field_C = false;
            return false;
        }
    }
    return field_C;
}
bool cSC4DBSegmentOStream::SetUint8(uint8_t value) {
    if (field_C) {
        if (!field_10->SetFieldUint8(value)) {
            field_C = false;
            return false;
        }
    }
    return field_C;
}
bool cSC4DBSegmentOStream::SetSint16(int16_t value) {
    if (field_C) {
        if (!field_10->SetFieldSint16(value)) {
            field_C = false;
            return false;
        }
    }
    return field_C;
}
bool cSC4DBSegmentOStream::SetUint16(uint16_t value) {
    if (field_C) {
        if (!field_10->SetFieldUint16(value)) {
            field_C = false;
            return false;
        }
    }
    return field_C;
}
bool cSC4DBSegmentOStream::SetSint32(int32_t value) {
    if (field_C) {
        if (!field_10->SetFieldSint32(value)) {
            field_C = false;
            return false;
        }
    }
    return field_C;
}
bool cSC4DBSegmentOStream::SetUint32(uint32_t value) {
    if (field_C) {
        if (!field_10->SetFieldUint32(value)) {
            field_C = false;
            return false;
        }
    }
    return field_C;
}
bool cSC4DBSegmentOStream::SetSint64(int64_t value) {
    if (field_C) {
        if (!field_10->SetFieldSint64(value)) {
            field_C = false;
            return false;
        }
    }
    return field_C;
}
bool cSC4DBSegmentOStream::SetUint64(uint64_t value) {
    if (field_C) {
        if (!field_10->SetFieldUint64(value)) {
            field_C = false;
            return false;
        }
    }
    return field_C;
}
bool cSC4DBSegmentOStream::SetFloat32(float value) {
    if (field_C) {
        if (!field_10->SetFieldFloat32(value)) {
            field_C = false;
            return false;
        }
    }
    return field_C;
}
bool cSC4DBSegmentOStream::SetFloat64(double value) {
    if (field_C) {
        if (!field_10->SetFieldFloat64(value)) {
            field_C = false;
            return false;
        }
    }
    return field_C;
}
bool cSC4DBSegmentOStream::SetRZCharStr(const char *value) {
    if (field_C) {
        cRZString str(value);
        if (!field_10->SetFieldString(str)) {
            field_C = false;
            return false;
        }
    }
    return field_C;
}
bool cSC4DBSegmentOStream::SetGZStr(const cIGZString &value) {
    if (field_C) {
        if (!field_10->SetFieldString(value)) {
            field_C = false;
            return false;
        }
    }
    return field_C;
}
bool cSC4DBSegmentOStream::SetGZSerializable(const cIGZSerializable &value) {
    if (field_C) {
        if (!value.Write(*this)) {
            field_C = false;
            return false;
        }
    }
    return field_C;
}
bool cSC4DBSegmentOStream::SetVoid(const void *value, uint32_t size) {
    if (field_C) {
        if (field_10->SetFieldVoid(value, size)) {
            field_C = true;
        } else {
            field_C = false;
        }
    }
    return field_C;
}
bool cSC4DBSegmentOStream::GetError() { return !field_C; }
void cSC4DBSegmentOStream::SetUserData(cIGZVariant *value) { field_18 = value; }
cIGZVariant *cSC4DBSegmentOStream::GetUserData() { return field_18; }
bool cSC4DBSegmentOStream::Init(bool b) { return false; }
bool cSC4DBSegmentOStream::Shutdown() { return false; }
bool cSC4DBSegmentOStream::QueryInterface(uint32_t uiClsId, void **ppvObj) {
    if (ppvObj == 0) { return false; }
    *ppvObj = 0;
    if (uiClsId == GZIID_cIGZOStream) { *ppvObj = static_cast<cIGZOStream *>(this); }
    else if (uiClsId == GZIID_cIGZOStream) { *ppvObj = static_cast<cIGZOStream *>(this); }
    else { return cRZUnknown::QueryInterface(uiClsId, ppvObj); }
    cRZUnknown::AddRef();
    return true;
}
bool cSC4DBSegmentIStream::Open(cIGZPersistDBSegment *segment,
                                const cGZPersistResourceKey &key, bool a3) {
    if (!Close()) return false;
    if (segment) {
        field_14 = segment;
        cIGZPersistDBRecord *record;
        if (!segment->OpenRecord(key, &record, 1)) return false;
        if (!record) {
            return false;
        }
        if (field_10) {
            field_10->Release();
            field_10 = nullptr;
        }
        field_10 = (cGZPersistDBSerialRecord *)record;
    }
    field_C = field_10 != nullptr;
    return field_C;
}
bool cSC4DBSegmentIStream::Close() {
    if (!field_14 || !field_10) return field_C;
    if (field_14->CloseRecord(field_10)) {
        field_10 = nullptr;
        field_14 = nullptr;
        return true;
    }
    field_C = false;
    return false;
}
bool cSC4DBSegmentIStream::IsOpen() {
    return (field_10 != nullptr && field_14 != nullptr);
}
cIGZPersistDBRecord *cSC4DBSegmentIStream::GetRecord() {
    return field_10;
}
cIGZPersistDBSegment *cSC4DBSegmentIStream::GetSegment() {
    return field_14;
}
bool cSC4DBSegmentIStream::ReadGZSerializable(cIGZSerializable *serializable) {
    if (field_C) {
        if (!serializable->Read(*this)) {
            field_C = false;
            return false;
        }
    }
    return field_C;
}
bool cSC4DBSegmentIStream::ReadResKey(cGZPersistResourceKey &key) {
    if (field_C) {
        if (!field_10->GetUint32(key.m_nTypeID)) {
            field_C = false;
            return false;
        }
        if (!field_10->GetUint32(key.m_nGroupID)) {
            field_C = false;
            return false;
        }
        if (!field_10->GetUint32(key.m_nInstanceID)) {
            field_C = false;
            return false;
        }
        return field_C;
    }
    return false;
}
bool cSC4DBSegmentIStream::ReadVariant(cIGZVariant *variant) {
    uint32_t clsid;
    GetUint32(clsid);
    if (clsid == GZIID_cIGZSerializable) {
        cIGZSerializable *serializable = nullptr;
        if (variant->GetValUnknown(clsid, (void **)&serializable)) {
            if (serializable) {
                if (!ReadGZSerializable(serializable)) {
                    return false;
                }
            }
        }
    }
    if ((field_10)->GetFieldVariant(*variant)) {
        return true;
    }
    field_C = false;
    return false;
}
bool cSC4DBSegmentIStream::Flush() { return false; }
bool cSC4DBSegmentIStream::GetSint8(int8_t &value) {
    if (field_C) {
        if (!field_10->GetSint8(value)) {
            field_C = false;
            return false;
        }
    }
    return field_C;
}
bool cSC4DBSegmentIStream::GetUint8(uint8_t &value) {
    if (field_C) {
        if (!field_10->GetUint8(value)) {
            field_C = false;
            return false;
        }
    }
    return field_C;
}
bool cSC4DBSegmentIStream::GetSint16(int16_t &value) {
    if (field_C) {
        if (!field_10->GetSint16(value)) {
            field_C = false;
            return false;
        }
    }
    return field_C;
}
bool cSC4DBSegmentIStream::GetUint16(uint16_t &value) {
    if (field_C) {
        if (!field_10->GetUint16(value)) {
            field_C = false;
            return false;
        }
    }
    return field_C;
}
bool cSC4DBSegmentIStream::GetSint32(int32_t &value) {
    if (field_C) {
        if (!field_10->GetSint32(value)) {
            field_C = false;
            return false;
        }
    }
    return field_C;
}
bool cSC4DBSegmentIStream::GetUint32(uint32_t &value) {
    if (field_C) {
        if (!field_10->GetUint32(value)) {
            field_C = false;
            return false;
        }
    }
    return field_C;
}
bool cSC4DBSegmentIStream::GetSint64(int64_t &value) {
    if (field_C) {
        if (!field_10->GetSint64(value)) {
            field_C = false;
            return false;
        }
    }
    return field_C;
}
bool cSC4DBSegmentIStream::GetUint64(uint64_t &value) {
    if (field_C) {
        if (!field_10->GetUint64(value)) {
            field_C = false;
            return false;
        }
    }
    return field_C;
}
bool cSC4DBSegmentIStream::GetFloat32(float &value) {
    if (field_C) {
        if (!field_10->GetFloat32(value)) {
            field_C = false;
            return false;
        }
    }
    return field_C;
}
bool cSC4DBSegmentIStream::GetFloat64(double &value) {
    if (field_C) {
        if (!field_10->GetFloat64(value)) {
            field_C = false;
            return false;
        }
    }
    return field_C;
}
bool cSC4DBSegmentIStream::GetRZCharStr(char *value, uint32_t size) {
    if (field_C) {
        if (!field_10->GetRZCharStr(value, size)) {
            field_C = false;
            return false;
        }
    }
    return field_C;
}
bool cSC4DBSegmentIStream::GetGZStr(cIGZString &value) {
    if (field_C) {
        if (!field_10->GetGZStr(value)) {
            field_C = false;
            return false;
        }
    }
    return field_C;
}
bool cSC4DBSegmentIStream::GetGZSerializable(cIGZSerializable &value) {
    if (field_C) {
        if (!value.Read(*this)) {
            field_C = false;
            return false;
        }
    }
    return field_C;
}
bool cSC4DBSegmentIStream::GetVoid(void *value, uint32_t size) {
    if (field_C) {
        if (!field_10->GetVoid(value, size)) {
            field_C = false;
            return false;
        }
    }
    return field_C;
}
uint32_t cSC4DBSegmentIStream::GetError() { return !field_C; }
void cSC4DBSegmentIStream::SetUserData(cIGZVariant *value) {
    field_18 = value;
}
cIGZVariant *cSC4DBSegmentIStream::GetUserData() { return field_18; }
bool cSC4DBSegmentIStream::Init(bool b) { return false; }
bool cSC4DBSegmentIStream::Shutdown() { return false; }
bool cSC4DBSegmentIStream::QueryInterface(uint32_t uiClsId, void **ppvObj) {
    if (uiClsId == GZIID_cIGZIStream) {
        *ppvObj = (cIGZIStream *)this;
        cRZUnknown::AddRef();
        return true;
    }
    if (uiClsId == GZIID_cISC4DBSegmentIStream) {
        *ppvObj = (cISC4DBSegmentIStream *)this;
        cRZUnknown::AddRef();
        return true;
    }
    return cRZUnknown::QueryInterface(uiClsId, ppvObj);
}
cGZPersistDBRecord::cGZPersistDBRecord(const cGZPersistResourceKey &crKey,
                                       cIGZPersistDBSegment *pDB)
        : m_Key(crKey), m_pDBSeg(pDB) {
    m_pDBSeg->AddRef();
}
cGZPersistDBRecord::cGZPersistDBRecord(cIGZPersistDBSegment *pDB)
        : m_Key{0, 0, 0}, m_pDBSeg(pDB) {
    m_pDBSeg->AddRef();
}
cGZPersistDBRecord::~cGZPersistDBRecord() { m_pDBSeg->Release(); }
uint32_t cGZPersistDBRecord::AddRef() { return ++m_uiRefCount; }
uint32_t cGZPersistDBRecord::Release() {
    uint32_t uiRefCount = --m_uiRefCount;
    if (uiRefCount == 0) { delete this; }
    return uiRefCount;
}
bool cGZPersistDBRecord::QueryInterface(uint32_t uiClsId, void **ppvObj) {
    if (ppvObj == 0) { return false; }
    *ppvObj = 0;
    if (uiClsId == GZIID_cIGZPersistDBRecord) { *ppvObj = static_cast<cIGZPersistDBRecord *>(this); }
    else { return 0; }
    AddRef();
    return true;
}
bool cGZPersistDBRecord::Close() {
    if (!m_pDBSeg) return false;
    bool success = m_pDBSeg->CloseRecord(this);
    if (!m_pDBSeg) return success;
    m_pDBSeg->Release();
    m_pDBSeg = NULL;
    return success;
}
cGZPersistDBSerialRecord::cGZPersistDBSerialRecord(
        const cGZPersistResourceKey &crKey, cIGZPersistDBSegment *pDB, bool a4)
        : cGZPersistDBRecord(crKey, pDB), cRZIStream(a4), cRZOStream(a4) {
    field_3C = a4;
}
cGZPersistDBSerialRecord::cGZPersistDBSerialRecord(cIGZPersistDBSegment *pDB,
                                                   bool a4)
        : cGZPersistDBRecord(pDB), cRZIStream(a4), cRZOStream(a4) {
    field_3C = a4;
}
bool cGZPersistDBSerialRecord::QueryInterface(uint32_t uiClsId,
                                                  void **ppvObj) {
    if (uiClsId == GZIID_cIGZOStream) {
        *ppvObj = static_cast<cIGZOStream *>(this);
    } else if (uiClsId == GZIID_cIGZIStream)
        *ppvObj = static_cast<cIGZIStream *>(this);
    else if (uiClsId == 0xC56B8F08)
        *ppvObj = static_cast<cIGZPersistDBRecord *>(this);   // TODO
    else
        return cGZPersistDBRecord::QueryInterface(uiClsId, ppvObj);
    cGZPersistDBRecord::AddRef();
    return 1;
}
bool cGZPersistDBSerialRecord::GetFieldUint8(uint8_t &val) {
    return GetFieldVoid(&val, sizeof(val));
}
bool cGZPersistDBSerialRecord::GetFieldUint16(uint16_t &val) {
    return GetFieldVoid(&val, sizeof(val));
}
bool cGZPersistDBSerialRecord::GetFieldUint32(uint32_t &val) {
    return GetFieldVoid(&val, sizeof(val));
}
bool cGZPersistDBSerialRecord::GetFieldUint64(uint64_t &val) {
    return GetFieldVoid(&val, sizeof(val));
}
bool cGZPersistDBSerialRecord::GetFieldSint8(int8_t &val) {
    return GetFieldVoid(&val, sizeof(val));
}
bool cGZPersistDBSerialRecord::GetFieldSint16(int16_t &val) {
    return GetFieldVoid(&val, sizeof(val));
}
bool cGZPersistDBSerialRecord::GetFieldSint32(int32_t &val) {
    return GetFieldVoid(&val, sizeof(val));
}
bool cGZPersistDBSerialRecord::GetFieldSint64(int64_t &val) {
    return GetFieldVoid(&val, sizeof(val));
}
bool cGZPersistDBSerialRecord::GetFieldFloat32(float &val) {
    return GetFieldVoid(&val, sizeof(val));
}
// reads float array I think?
bool cGZPersistDBSerialRecord::GetFieldFloat32(float *val, uint32_t count) {
    return GetFieldVoid(val, sizeof(float) * count);
}
bool cGZPersistDBSerialRecord::GetFieldFloat64(double &val) {
    return GetFieldVoid(&val, sizeof(val));
}
bool cGZPersistDBSerialRecord::GetFieldFloat64(double *val, uint32_t count) {
    return GetFieldVoid(val, sizeof(double) * count);
}
bool cGZPersistDBSerialRecord::GetFieldString(cIGZString &val) {
    uint32_t v18;
    if (!GetFieldUint32(v18)) {
        val.Resize(0);
        uint8_t tmp;
        while (GetFieldUint8(tmp))
        {
            if (tmp == 0) break;
            cRZString tmp2;
            tmp2.Sprintf("%c", tmp);
            val.Append(tmp2);
        }
        return true;
    }
    uint32_t v13 = GetSize();
    SeekRelative(-4);
    val.Resize(0);
    for (uint32_t i = 0; i < v13; i++) {
        uint8_t tmp;
        GetFieldUint8(tmp);
        cRZString tmp2;
        tmp2.Sprintf("%c", tmp);
        val.Append(tmp2);
    }
    return true;
}
bool cGZPersistDBSerialRecord::GetFieldVariant(cIGZVariant &val) {
    cRZAutoRefCount<cRZVariant> variant = new cRZVariant();
    uint32_t typetag;
    if (!GetFieldUint32(typetag)) return false;
    tagRZVariant::Type type = tagRZVariant::Type(typetag);
    switch (type) {
        case (tagRZVariant::Void | tagRZVariant::Array): {
            uint32_t size;
            if (!GetFieldUint32(size)) return false;
            uint8_t *data = new uint8_t[size];
            if (!GetFieldVoid(data, size)) {
                delete[] data;
                return false;
            }
            variant->RefVoid(data, size);
            break;
        }
        case tagRZVariant::Bool:
            uint8_t tmp;
            if (!GetFieldUint8(tmp)) return false;
            variant->SetValBool((bool)tmp);
            break;
        case tagRZVariant::Sint8:
            int8_t tmp2;
            if (!GetFieldSint8(tmp2)) return false;
            variant->SetValSint8(tmp2);
            break;
        case tagRZVariant::Uint8:
            uint8_t tmp3;
            if (!GetFieldUint8(tmp3)) return false;
            variant->SetValUint8(tmp3);
            break;
        case tagRZVariant::Sint16:
            int16_t tmp4;
            if (!GetFieldSint16(tmp4)) return false;
            variant->SetValSint16(tmp4);
            break;
        case tagRZVariant::Uint16:
            uint16_t tmp5;
            if (!GetFieldUint16(tmp5)) return false;
            variant->SetValUint16(tmp5);
            break;
        case tagRZVariant::Sint32:
            int32_t tmp6;
            if (!GetFieldSint32(tmp6)) return false;
            variant->SetValSint32(tmp6);
            break;
        case tagRZVariant::Uint32:
            uint32_t tmp7;
            if (!GetFieldUint32(tmp7)) return false;
            variant->SetValUint32(tmp7);
            break;
        case tagRZVariant::Sint64:
            int64_t tmp8;
            if (!GetFieldSint64(tmp8)) return false;
            variant->SetValSint64(tmp8);
            break;
        case tagRZVariant::Uint64:
            uint64_t tmp9;
            if (!GetFieldUint64(tmp9)) return false;
            variant->SetValUint64(tmp9);
            break;
        case tagRZVariant::Float32:
            float tmp10;
            if (!GetFieldFloat32(tmp10)) return false;
            variant->SetValFloat32(tmp10);
            break;
        case tagRZVariant::Float64:
            double tmp11;
            if (!GetFieldFloat64(tmp11)) return false;
            variant->SetValFloat64(tmp11);
            break;
        case (tagRZVariant::RZChar | tagRZVariant::Array): {
            uint32_t size2;
            if (!GetFieldUint32(size2)) return false;
            char *data2 = new char[size2];
            if (!GetFieldVoid(data2, size2)) {
                delete[] data2;
                return false;
            }
            variant->RefRZChar(data2, size2);
            break;
        }
        case (tagRZVariant::Char | tagRZVariant::Array): {
            uint32_t size3;
            if (!GetFieldUint32(size3)) return false;
            char *data3 = new char[size3];
            if (!GetFieldVoid(data3, size3)) {
                delete[] data3;
                return false;
            }
            variant->RefChar(data3, size3);
            break;
        }
        default:
            cRZString excStr;
            excStr.Sprintf("cGZPersistDBSerialRecord::GetFieldVariant: "
                           "Unhandled variant type %d",
                           type);
            RZGetFramework()->ExceptionNotificationObj()->Notify(excStr.c_str(), 0);
    }
    val.CopyFrom(*variant);
    return true;
}
bool cGZPersistDBSerialRecord::GetFieldVoid(void *dst, uint32_t count,
                                            uint32_t size) {
    return GetFieldVoid(dst, count * size);
}
bool cGZPersistDBSerialRecord::SetFieldUint8(uint8_t val) {
    return SetFieldVoid(&val, sizeof(val));
}
bool cGZPersistDBSerialRecord::SetFieldUint16(uint16_t val) {
    return SetFieldVoid(&val, sizeof(val));
}
bool cGZPersistDBSerialRecord::SetFieldUint32(uint32_t val) {
    return SetFieldVoid(&val, sizeof(val));
}
bool cGZPersistDBSerialRecord::SetFieldUint64(uint64_t val) {
    return SetFieldVoid(&val, sizeof(val));
}
bool cGZPersistDBSerialRecord::SetFieldSint8(int8_t val) {
    return SetFieldVoid(&val, sizeof(val));
}
bool cGZPersistDBSerialRecord::SetFieldSint16(int16_t val) {
    return SetFieldVoid(&val, sizeof(val));
}
bool cGZPersistDBSerialRecord::SetFieldSint32(int32_t val) {
    return SetFieldVoid(&val, sizeof(val));
}
bool cGZPersistDBSerialRecord::SetFieldSint64(int64_t val) {
    return SetFieldVoid(&val, sizeof(val));
}
bool cGZPersistDBSerialRecord::SetFieldFloat32(float val) {
    return SetFieldVoid(&val, sizeof(val));
}
bool cGZPersistDBSerialRecord::SetFieldFloat32(const float *val,
                                               uint32_t count) {
    return SetFieldVoid(&val, sizeof(val) * count);
}
bool cGZPersistDBSerialRecord::SetFieldFloat64(double val) {
    return SetFieldVoid(&val, sizeof(val));
}
bool cGZPersistDBSerialRecord::SetFieldFloat64(const double *val,
                                               uint32_t count) {
    return SetFieldVoid(&val, sizeof(val) * count);
}
bool cGZPersistDBSerialRecord::SetFieldString(const cIGZString &val) {
    if (!SetFieldUint32(val.Strlen())) return false;
    return SetFieldVoid(val.ToChar(), val.Strlen());
}
bool cGZPersistDBSerialRecord::SetFieldVariant(cIGZVariant &val) {
    if (!SetFieldUint8(val.GetType())) return false;
    switch (val.GetType()) {
        /*case tagRZVariant::eVoid:
            break;*/
        case tagRZVariant::Bool:
            if (!SetFieldUint8(val.AsBool())) return false;
            break;
        case tagRZVariant::Sint8:
            if (!SetFieldSint8(val.AsSint8())) return false;
            break;
        case tagRZVariant::Uint8:
            if (!SetFieldUint8(val.AsUint8())) return false;
            break;
        case tagRZVariant::Sint16:
            if (!SetFieldSint16(val.AsSint16())) return false;
            break;
        case tagRZVariant::Uint16:
            if (!SetFieldUint16(val.AsUint16())) return false;
            break;
        case tagRZVariant::Sint32:
            if (!SetFieldSint32(val.AsSint32())) return false;
            break;
        case tagRZVariant::Uint32:
            if (!SetFieldUint32(val.AsUint32())) return false;
            break;
        case tagRZVariant::Sint64:
            if (!SetFieldSint64(val.AsSint64())) return false;
            break;
        case tagRZVariant::Uint64:
            if (!SetFieldUint64(val.AsUint64())) return false;
            break;
        case tagRZVariant::Float32:
            if (!SetFieldFloat32(val.AsFloat32())) return false;
            break;
        case tagRZVariant::Float64:
            if (!SetFieldFloat64(val.AsFloat64())) return false;
            break;
        case (tagRZVariant::RZChar | tagRZVariant::Array):
            if (!SetFieldUint32(val.GetCount())) return false;
            if (!SetFieldVoid(val.RefRZChar(), val.GetCount())) return false;
            break;
        case (tagRZVariant::Char | tagRZVariant::Array):
            if (!SetFieldUint32(val.GetCount())) return false;
            if (!SetFieldVoid(val.RefChar(), val.GetCount())) return false;
            break;
        default:
            cRZString excStr;
            excStr.Sprintf("cGZPersistDBSerialRecord::SetFieldVariant: "
                           "Unhandled variant type %d",
                           val.GetType());
            RZGetFramework()->ExceptionNotificationObj()->Notify(excStr.c_str(), 0);
    }
    return true;
}
bool cGZPersistDBSerialRecord::SetFieldVoid(const void *src, uint32_t size,
                                            uint32_t count) {
    return SetFieldVoid(src, size * count);
}
bool cGZPersistDBSerialRecord::Skip(uint32_t s) {
    SeekRelative(s);
    return true;
}
bool cGZPersistDBSerialRecord::GetRZCharStr(char *str, uint32_t len) {
    return false;
}
bool cGZPersistDBSerialRecord::GetGZStr(cIGZString &str) {
    return GetFieldString(str);
}
bool cGZPersistDBSerialRecord::GetVoid(void *data, uint32_t len) {
    return GetFieldVoid(data, len);
}
bool cGZPersistDBSerialRecord::SetRZCharStr(const char *str) {
    cRZString rzStr(str);
    return SetFieldString(rzStr);
}
bool cGZPersistDBSerialRecord::SetGZStr(const cIGZString &str) {
    return SetFieldString(str);
}
bool cGZPersistDBSerialRecord::SetVoid(const void *data, uint32_t len) {
    return SetFieldVoid(data, len);
}
const cGZPersistResourceKey &cGZPersistDBSerialRecord::GetKey() {
    return m_Key;
}
void cGZPersistDBSerialRecord::SeekAbsolute(uint32_t pos) {}
void cGZPersistDBSerialRecord::SeekRelative(int32_t pos) {}
cGZDBRecordPackedFile::cGZDBRecordPackedFile(PackedFileRecordType type,
                                             const cGZPersistResourceKey &key,
                                             cIGZPersistDBSegment *segment,
                                             bool a4)
        : cGZPersistDBSerialRecord(key, segment, a4), m_Type(type) {}

cGZDBReadRecordPackedFile::cGZDBReadRecordPackedFile(
        uint32_t offset, uint32_t size, const cGZPersistResourceKey &crKey,
        cIGZPersistDBSegmentPackedFile *pDBSeg, bool a6)
        : cGZDBRecordPackedFile(PackedFileRecordType::ReadDBPF, crKey,
                                (cIGZPersistDBSegment *)pDBSeg, a6) {
    field_A = true;
    m_nOffset = offset;
    m_nPos = 0;
    m_nSize = size;
    m_oDBSeg = pDBSeg;
}
uint16_t cGZDBReadRecordPackedFile::GetAccessFlags() { return 0; }
uint32_t cGZDBReadRecordPackedFile::GetSize() { return m_nSize; }
void cGZDBReadRecordPackedFile::SetSize(uint32_t size) { m_nSize = size; }
uint32_t cGZDBReadRecordPackedFile::GetPosition() { return m_nPos; }
cGZDBReadRecordPackedFile::~cGZDBReadRecordPackedFile() { DoPostClose(); }
void cGZDBReadRecordPackedFile::SetKey(const cGZPersistResourceKey &key) {
    m_Key = key;
}
bool cGZDBReadRecordPackedFile::GetFieldVoid(void *data, uint32_t len) {
    if (!field_A || len + m_nPos > m_nSize) return false;
    if (!m_oDBSeg->ReadFileSpan(data, len, m_nOffset + m_nPos)) return false;
    m_nPos += len;
    return true;
}
bool cGZDBReadRecordPackedFile::SetFieldVoid(const void *pVoid,
                                             uint32_t uint32) {
    return false;
}
void cGZDBReadRecordPackedFile::Flush() { }
bool cGZDBReadRecordPackedFile::GetError() const { return false; }
void cGZDBReadRecordPackedFile::SeekAbsolute(uint32_t pos) {
    if (!field_A || m_nSize <= pos) return /*false*/;
    m_nPos = pos;
    // return true;
}
void cGZDBReadRecordPackedFile::SeekRelative(int32_t pos) {
    if (!field_A) return /*false*/;
    uint32_t newPos = m_nPos + pos;
    if (newPos >= m_nSize) return /*false*/;
    m_nPos = newPos;
    // return true;
}
void cGZDBReadRecordPackedFile::DoPostClose() {
    field_A = false;
    if (m_oDBSeg) {
        m_oDBSeg->Release();
        m_oDBSeg = nullptr;
    }
    m_nPos = 0;
}
cGZDBWriteRecordPackedFile::cGZDBWriteRecordPackedFile(
        uint32_t a2, uint32_t a3, const cGZPersistResourceKey &crKey,
        cIGZPersistDBSegmentPackedFile *pDBSeg, bool a6)
        : cGZDBRecordPackedFile(PackedFileRecordType::WriteDBPF, crKey,
                                (cIGZPersistDBSegment *)pDBSeg, a6) {
    field_A = true;
    field_44 = a2;
    field_48 = 0;
    field_4C = a3;
    field_18 = pDBSeg;
}
uint16_t cGZDBWriteRecordPackedFile::GetAccessFlags() { return 0; }
uint32_t cGZDBWriteRecordPackedFile::GetSize() { return field_4C; }
void cGZDBWriteRecordPackedFile::SetSize(uint32_t uint32) { field_4C = uint32; }
uint32_t cGZDBWriteRecordPackedFile::GetPosition() { return field_48; }
cGZDBWriteRecordPackedFile::~cGZDBWriteRecordPackedFile() { DoPostClose(); }
void cGZDBWriteRecordPackedFile::SetKey(const cGZPersistResourceKey &key) {
    m_Key = key;
}
bool cGZDBWriteRecordPackedFile::GetFieldVoid(void *data, uint32_t len) {
    return false;
}
bool cGZDBWriteRecordPackedFile::SetFieldVoid(const void *data, uint32_t len) {
    if (!field_A || len + field_48 > field_4C) return false;
    if (!field_18->WriteFileSpan(data, len, field_44 + field_48)) return false;
    field_48 += len;
    return true;
}
void cGZDBWriteRecordPackedFile::Flush() { }
bool cGZDBWriteRecordPackedFile::GetError() const { return false; }
void cGZDBWriteRecordPackedFile::SeekAbsolute(uint32_t pos) {
    if (!field_A || field_4C <= pos) return /*false*/;
    field_48 = pos;
    // return true;
}
void cGZDBWriteRecordPackedFile::SeekRelative(int32_t pos) {
    if (!field_A) return /*false*/;
    uint32_t newPos = field_48 + pos;
    if (newPos >= field_4C) return /*false*/;
    field_48 = newPos;
    // return true;
}
void cGZDBWriteRecordPackedFile::DoPostClose() {
    field_A = false;
    if (field_18) {
        field_18->Release();
        field_18 = nullptr;
    }
    field_48 = 0;
}
cGZDBReadRecordRAM::cGZDBReadRecordRAM(uint8_t *buf, uint32_t size,
                                       const cGZPersistResourceKey &crKey,
                                       cIGZPersistDBSegmentPackedFile *pDBSeg,
                                       bool a6)
        : cGZDBRecordPackedFile(PackedFileRecordType::ReadRAM, crKey,
                                (cIGZPersistDBSegment *)pDBSeg, a6) {
    field_A = true;
    m_pBuf = buf;
    m_nSize = size;
    m_nPos = 0;
}
uint16_t cGZDBReadRecordRAM::GetAccessFlags() { return 0; }
uint32_t cGZDBReadRecordRAM::GetSize() { return m_nSize; }
void cGZDBReadRecordRAM::SetSize(uint32_t uint32) {
    if (uint32 > m_nSize) return;
    m_nSize = uint32;
}
uint32_t cGZDBReadRecordRAM::GetPosition() { return m_nPos; }
cGZDBReadRecordRAM::~cGZDBReadRecordRAM() {
    if (m_pBuf) {
        delete[] m_pBuf;
        m_pBuf = nullptr;
    }
}
void cGZDBReadRecordRAM::SetKey(const cGZPersistResourceKey &key) {
    m_Key = key;
}
bool cGZDBReadRecordRAM::GetFieldVoid(void *data, uint32_t len) {
    if (!field_A || len + m_nPos > m_nSize) return false;
    memcpy(data, m_pBuf + m_nPos, len);
    m_nPos += len;
    return true;
}
bool cGZDBReadRecordRAM::SetFieldVoid(const void *pVoid, uint32_t uint32) {
    return false;
}
void cGZDBReadRecordRAM::Flush() { }
bool cGZDBReadRecordRAM::GetError() const { return false; }
void cGZDBReadRecordRAM::SeekAbsolute(uint32_t pos) {
    if (!field_A || m_nSize <= pos) return /*false*/;
    m_nPos = pos;
    // return true;
}
void cGZDBReadRecordRAM::SeekRelative(int32_t pos) {
    if (!field_A) return /*false*/;
    uint32_t newPos = m_nPos + pos;
    if (newPos >= m_nSize) return /*false*/;
    m_nPos = newPos;
    // return true;
}
void cGZDBReadRecordRAM::DoPostClose() {
    field_A = false;
    if (m_pBuf) {
        delete[] m_pBuf;
        m_pBuf = nullptr;
    }
    m_nPos = 0;
}