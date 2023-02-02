//
// Created by chris on 1/17/2023.
//

#include "cRZFile.h"

#include "GZTypes.h"


void AdjustPathSeparatorsForPlatform(const cRZString &in, cRZString &out) {
    out = in;
    std::replace(out.begin(), out.end(), '\\', '/');
}

void cRZFile::SetPath(const cRZString &path) {
    field_4 = path;
    AdjustPathSeparatorsForPlatform(path, field_4);
}
/*int __cdecl cRZFile::Open(
cRZFile *this,
int a2,
int a3,
int a4)
{
int v4; // ebx
int v5; // esi
int v6; // eax
int v7; // eax
cRZString v9; // [esp+14h] [ebp-24h] BYREF

if ( this->__vftable->__ZNK7cRZFile6IsOpenEv(this) )
return 1;
if ( a2 == 1 )
goto LABEL_8;
if ( a2 != 2 )
{
if ( a2 == 3 )
{
v5 = 2;
goto LABEL_9;
}
LABEL_8:
v5 = 0;
goto LABEL_9;
}
v5 = 1;
LABEL_9:
if ( a3 == 1 )
{
v5 |= 0x600u;
}
else if ( a3 > 1 )
{
if ( a3 == 3 )
{
v5 |= 0x200u;
}
else if ( a3 == 4 )
{
v5 |= 0x400u;
}
}
else if ( !a3 )
{
v5 |= 0x800u;
}
cRZString::cRZString(&v9);
ConvertStringEncoding(&this->field_4, &v9, -1);
v6 = open(v9.str, v5, 384);
this->field_14 = v6;
if ( v6 == -1 )
{
if ( *__error() == 13 )
{
this->field_28 = 0x20000002;
}
else if ( *__error() == 17 )
{
this->field_28 = 0x20000001;
}
else
{
this->field_28 = *__error();
}
v4 = 0;
}
else
{
this->field_10 = 1;
this->field_18 = a2;
this->flags = a3;
this->field_20 = a4;
v7 = lseek(v6, 0LL, 1);
this->field_3C = v7;
this->field_38 = v7;
this->field_50 = 0;
this->field_54 = 0;
this->field_68 = 0;
this->field_6C = 0;
v4 = 1;
}
cRZString::~cRZString(&v9);
return v4;
}*/
bool cRZFile::Open(int a2, int a3, int a4) {
    if (IsOpen()) return true;
    int unixFcntlFlags;
    if (a2 == 1) {
        unixFcntlFlags = 0;
    } else if (a2 == 2) {
        unixFcntlFlags = 1;
    } else if (a2 == 3) {
        unixFcntlFlags = 2;
    } else {
        return false;
    }
    if (a3 == 1) {
        unixFcntlFlags |= 0x600;
    } else if (a3 == 2) {
        unixFcntlFlags |= 0x800;
    } else if (a3 == 3) {
        unixFcntlFlags |= 0x200;
    } else if (a3 == 4) {
        unixFcntlFlags |= 0x400;
    } else {
        return false;
    }
    // use fopen instead of open. first, convert the fcntl flags to fopen flags
    cRZString fopenFlags{""};
    if (unixFcntlFlags & 0x200) {
        // create
        fopenFlags += "w+";
    } else if (unixFcntlFlags & 0x400) {
        // truncate
        fopenFlags += "w";
    } else if (unixFcntlFlags & 0x800) {
        // O_EXCL
        fopenFlags += "w";
    } else if (unixFcntlFlags & 1) {
        // write
        fopenFlags += "w+";
    } else if (unixFcntlFlags & 2) {
        // read/write
        fopenFlags += "r+";

    } else {
        fopenFlags += "r";
    }
    if (unixFcntlFlags & 8) {
        // O_APPEND
        fopenFlags += "a";
    }
    if (a4)   // I'm assuming this is checking for binary read/write
        fopenFlags += "b";
    FILE *f = fopen(field_4.ToChar(), fopenFlags.ToChar());
    field_14 = f;
    if (f == NULL) {
        field_28 = errno;
        return false;
    }
    field_10 = 1;
    field_18 = a2;
    field_1C = a3;
    field_20 = a4;
    fseek(f, 0, SEEK_END);
    field_3C = ftell(f);
    field_38 = ftell(f);
    fseek(f, 0, SEEK_SET);
    field_50 = 0;
    field_54 = 0;
    field_68 = 0;
    field_6C = 0;
    return true;
}
size_t cRZFile::Length() {
    if (!IsOpen()) return 0;
    uint32_t pos = ftell(field_14);
    fseek(field_14, 0, SEEK_END);
    uint32_t len = ftell(field_14);
    fseek(field_14, pos, SEEK_SET);
    return len;
}
uint32_t cRZFile::SeekToPosition(uint32_t pos) { return Seek(pos, SEEK_SET); }
/*int __cdecl cRZFile::Seek(cRZFile *this, int a2, int a3)
{
  int v3; // esi
  int v5; // eax
  int v6; // edi
  int v7; // eax
  int v8; // esi
  int v9; // eax

v3 = a2;
if ( !this->field_10 )
    return -1;
if ( a3 != 1 )
{
    if ( a3 == 2 )
    {
        v6 = 2;
        goto LABEL_13;
    }
    if ( a3 )
        return (this->__vftable->__ZN7cRZFile8PositionEv)();
    goto LABEL_10;
}
v5 = this->field_38;
if ( this->field_3C != v5 )
{
    v3 = v5 + a2;
LABEL_10:
    v6 = 0;
    goto LABEL_13;
}
v6 = 1;
LABEL_13:
    cRZFile::FlushWriteBuffer(this);
v7 = lseek(this->field_14, v3, v6);
v8 = v7;
if ( v7 == -1 )
{
    v9 = lseek(this->field_14, 0LL, 1);
    this->field_3C = v9;
    this->field_38 = v9;
    this->field_28 = *__error();
}
else
{
    this->field_3C = v7;
    this->field_38 = v7;
}
return v8;
}*/
uint32_t cRZFile::Seek(uint32_t offset, int whence) {
    if (!IsOpen()) return -1;
    if (whence == SEEK_CUR) {
        if (field_3C != field_38) {
            offset += field_38;
        } else {
            whence = SEEK_END;
        }
    }
    this->FlushWriteBuffer();
    fseek(field_14, offset, whence);
    field_3C = ftell(field_14);
    field_38 = ftell(field_14);
    return field_3C;
}
bool cRZFile::Write(void *src, uint32_t len) {
    // TODO: make functionally equivalent to binary
    if (!IsOpen()) return false;
    if (field_18 != 2 && field_18 != 3) return false;

    fwrite(src, 1, len, field_14);
    field_38 += len;
    field_3C += len;
    return true;
}
bool cRZFile::Read(void *dst, uint32_t len) {
    if (!IsOpen()) return false;
    if (field_18 != 1 && field_18 != 3) return false;

    fread(dst, 1, len, field_14);
    field_38 += len;
    field_3C += len;
    return true;
}
bool cRZFile::Close() {
    if (!IsOpen()) return false;
    if (field_18 == 2 || field_18 == 3) {
        // FlushWriteBuffer();
    }
    fclose(field_14);
    field_14 = NULL;
    field_10 = 0;
    return true;
}
cRZFile::~cRZFile() {
    if (IsOpen()) {
        Close();
    }
}

void cRZFile::FlushWriteBuffer() {
    // TODO
}
