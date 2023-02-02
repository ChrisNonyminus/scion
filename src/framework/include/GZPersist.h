//
// Created by chris on 1/17/2023.
//

#ifndef SCION_GZPERSIST_H
#define SCION_GZPERSIST_H

#include "cRZOStream.h"
#include "cRZIStream.h"
#include "cRZString.h"
#include "cRZCriticalSection.h"
#include "cRZFile.h"
#include "cRZUnknown.h"
#include "cRZAutoRefCount.h"
#include "cRZVariant.h"

#include <map>
#include <set>


// NOTE: SimCity 4 and other games using the Gonzo-Rizzo framework use QFS for
// compression. See
// https://wiki.sc4devotion.com/index.php?title=DBPF_Compression.
struct cIGZCompressor : public cIGZUnknown {

    virtual void CompressData(const void *src, size_t srcLen, void *dst,
                              size_t *dstLen) = 0;
    virtual bool DecompressData(const void *src, size_t srcLen, void *dst,
                                size_t *dstLen) = 0;
    virtual size_t GetMaxLengthRequiredForCompressedData(size_t srcLen) = 0;
    virtual size_t GetLengthOfDecompressedData(const void *src) = 0;
    virtual size_t GetMaxLengthRequiredForDecompressedData(const void *src) = 0;
    virtual void GetCompressionVersionString(cRZString &str) = 0;
    virtual ~cIGZCompressor() {}
};
GZ_IUNKNOWN_ID(cIGZCompressor, 0x8359908E);

struct cRZFastCompression3 : public cIGZCompressor {
    IUNKNOWN_COMMON_DEFS
    uint32_t field_8{0};
    virtual void CompressData(const void *src, size_t srcLen, void *dst,
                              size_t *dstLen);
    virtual bool DecompressData(const void *src, size_t srcLen, void *dst,
                                size_t *dstLen);
    virtual size_t GetMaxLengthRequiredForCompressedData(size_t srcLen);
    virtual size_t GetLengthOfDecompressedData(const void *src);
    virtual size_t GetMaxLengthRequiredForDecompressedData(const void *src);
    virtual void GetCompressionVersionString(cRZString &str);

    static int decoderef(char *a1, int32_t *a2, char *a3);
    static int encoderef(char *a1, int32_t a2, char *a3, const int *a4);
    static int refpack(unsigned char *a1, int32_t a2, unsigned char *a3,
                       int32_t a4, int32_t a5);
    static int isref(char *a1);
    static int sizeref(char *ref);

    bool ValidateCompressedData(const unsigned char *data, uint32_t len);

    virtual ~cRZFastCompression3() = default;
};



struct cGZPersistResourceKey {
    cGZPersistResourceKey(uint32_t type, uint32_t group, uint32_t instance) {
        m_nTypeID = type;
        m_nGroupID = group;
        m_nInstanceID = instance;
    }
    uint32_t m_nTypeID;
    uint32_t m_nGroupID;
    uint32_t m_nInstanceID;

    inline bool operator<(const cGZPersistResourceKey &rhs) const {
        if (m_nTypeID < rhs.m_nTypeID) {
            return true;
        }
        if (m_nTypeID > rhs.m_nTypeID) {
            return false;
        }
        if (m_nGroupID < rhs.m_nGroupID) {
            return true;
        }
        if (m_nGroupID > rhs.m_nGroupID) {
            return false;
        }
        return m_nInstanceID < rhs.m_nInstanceID;
    }

    inline bool operator==(const cGZPersistResourceKey &rhs) const {
        return m_nTypeID == rhs.m_nTypeID && m_nGroupID == rhs.m_nGroupID &&
               m_nInstanceID == rhs.m_nInstanceID;
    }

    inline bool operator!=(const cGZPersistResourceKey &rhs) const {
        return m_nTypeID != rhs.m_nTypeID || m_nGroupID != rhs.m_nGroupID ||
               m_nInstanceID != rhs.m_nInstanceID;
    }

    inline bool operator>(const cGZPersistResourceKey &rhs) const {
        return rhs < *this;
    }

    inline bool operator<=(const cGZPersistResourceKey &rhs) const {
        return !(rhs < *this);
    }

    inline bool operator>=(const cGZPersistResourceKey &rhs) const {
        return !(*this < rhs);
    }
};

struct cIGZPersistDBRecord : public cIGZUnknown {
    //IUNKNOWN_CLSID(0xE56B8F03);
    virtual const cGZPersistResourceKey &GetKey() = 0;
    virtual uint16_t GetAccessFlags() = 0;
    virtual bool Close() = 0;
    virtual bool GetFieldVoid(void *, uint32_t) = 0;
    virtual bool SetFieldVoid(const void *, uint32_t) = 0;
    virtual uint32_t GetSize() = 0;
    virtual void SetSize(uint32_t) = 0;
    virtual uint32_t GetPosition() = 0;
    virtual void SeekAbsolute(uint32_t pos) = 0;
    virtual void SeekRelative(int32_t pos) = 0;
    virtual ~cIGZPersistDBRecord() {}
    virtual void SetKey(const cGZPersistResourceKey &key) = 0;
};

GZ_IUNKNOWN_ID(cIGZPersistDBRecord,0xE56B8F03);

struct cIGZPersistDBSegment : public cIGZUnknown {
    //IUNKNOWN_CLSID(0x656B8F0C);
    virtual bool CloseRecord(cIGZPersistDBRecord *pRecord) = 0;
    virtual bool OpenRecord(cGZPersistResourceKey const &key,
                            cIGZPersistDBRecord **ppRecord, uint32_t a4) = 0;
    virtual bool DoOpenRecord(cGZPersistResourceKey const &key,
                              cIGZPersistDBRecord **ppRecord, uint32_t a4) = 0;
    virtual bool CreateNewRecord(const cGZPersistResourceKey &key,
                                 cIGZPersistDBRecord **ppRecord) = 0;
    virtual bool DoCreateNewRecord(const cGZPersistResourceKey &key,
                                   cIGZPersistDBRecord **ppRecord) = 0;
    virtual bool Init() = 0;
};

GZ_IUNKNOWN_ID(cIGZPersistDBSegment,0x656B8F0C);

struct cGZPersistDBSegment : public cIGZPersistDBSegment {
    IUNKNOWN_COMMON_DEFS
    bool CloseRecord(cIGZPersistDBRecord *pRecord) override;
    bool OpenRecord(cGZPersistResourceKey const &key,
                    cIGZPersistDBRecord **ppRecord, uint32_t a4) {
        return DoOpenRecord(key, ppRecord, a4);
    }
    bool CreateNewRecord(const cGZPersistResourceKey &key,
                         cIGZPersistDBRecord **ppRecord) {
        return DoCreateNewRecord(key, ppRecord);
    }
    bool Init() override;
    cRZCriticalSection m_oCriticalSection{};
    int field_14{0};
};

struct cGZPersistDBRecord : public cIGZPersistDBRecord {
    IUNKNOWN_COMMON_DEFS
    cGZPersistResourceKey m_Key;
    cIGZPersistDBSegment *m_pDBSeg;

    cGZPersistDBRecord(const cGZPersistResourceKey &crKey,
                       cIGZPersistDBSegment *pDB);
    cGZPersistDBRecord(cIGZPersistDBSegment *pDB);
    ~cGZPersistDBRecord() override;
    bool Close() override;
    // TODO
};

struct cIGZPersistDBSegmentPackedFile : public cIGZUnknown {
    //IUNKNOWN_CLSID(0x286D4A6A);
    virtual void SetPath(const cIGZString &path) = 0;
    virtual bool Open(bool a2, bool a3) = 0;
    virtual bool ReadFileSpan(void *pBuffer, uint32_t nSize,
                              uint32_t nOffset) = 0;
    virtual bool WriteFileSpan(const void *data, uint32_t size,
                               uint32_t pos) = 0;
};

GZ_IUNKNOWN_ID(cIGZPersistDBSegmentPackedFile,0x286D4A6A);

struct cGZPersistDBSerialRecord : public cGZPersistDBRecord,
                                  public cRZIStream,
                                  public cRZOStream {
    bool field_3C;
    uint32_t AddRef() override { return cGZPersistDBRecord::AddRef(); }
    uint32_t Release() override { return cGZPersistDBRecord::Release(); }
    bool QueryInterface(uint32_t uiClsId, void **ppvObj) override;
    cGZPersistDBSerialRecord(const cGZPersistResourceKey &crKey,
                             cIGZPersistDBSegment *pDB, bool a4);
    cGZPersistDBSerialRecord(cIGZPersistDBSegment *pDB, bool a4);

    virtual bool GetFieldUint8(uint8_t &val);
    virtual bool GetFieldUint16(uint16_t &val);
    virtual bool GetFieldUint32(uint32_t &val);
    virtual bool GetFieldUint64(uint64_t &val);
    virtual bool GetFieldSint8(int8_t &val);
    virtual bool GetFieldSint16(int16_t &val);
    virtual bool GetFieldSint32(int32_t &val);
    virtual bool GetFieldSint64(int64_t &val);
    virtual bool GetFieldFloat32(float &val);
    virtual bool GetFieldFloat32(float *, uint32_t);
    virtual bool GetFieldFloat64(double &val);
    virtual bool GetFieldFloat64(double *, uint32_t);
    virtual bool GetFieldString(cIGZString &val);
    virtual bool GetFieldVariant(cIGZVariant &val);
    virtual bool GetFieldVoid(void *, uint32_t, uint32_t);
    virtual bool GetFieldVoid(void *data, uint32_t len) = 0;

    virtual bool SetFieldUint8(uint8_t val);
    virtual bool SetFieldUint16(uint16_t val);
    virtual bool SetFieldUint32(uint32_t val);
    virtual bool SetFieldUint64(uint64_t val);
    virtual bool SetFieldSint8(int8_t val);
    virtual bool SetFieldSint16(int16_t val);
    virtual bool SetFieldSint32(int32_t val);
    virtual bool SetFieldSint64(int64_t val);
    virtual bool SetFieldFloat32(float val);
    virtual bool SetFieldFloat32(const float *, uint32_t);
    virtual bool SetFieldFloat64(double val);
    virtual bool SetFieldFloat64(const double *, uint32_t);
    virtual bool SetFieldString(const cIGZString &val);
    virtual bool SetFieldVariant(cIGZVariant &val);
    virtual bool SetFieldVoid(const void *, uint32_t, uint32_t);
    virtual bool SetFieldVoid(const void *, uint32_t) = 0;

    virtual bool Skip(uint32_t);

    virtual bool GetRZCharStr(char *str, uint32_t len);
    virtual bool GetGZStr(cIGZString &str);
    virtual bool GetVoid(void *data, uint32_t len);

    virtual bool SetRZCharStr(const char *str);
    virtual bool SetGZStr(const cIGZString &str);
    virtual bool SetVoid(const void *data, uint32_t len);
    const cGZPersistResourceKey &GetKey() override;
    virtual void SeekAbsolute(uint32_t pos);
    virtual void SeekRelative(int32_t pos);

    // TODO
};

enum class PackedFileRecordType {
    ReadDBPF = 1,
    ReadRAM = 2,
    WriteDBPF = 3,
};

struct cGZDBRecordPackedFile : public cGZPersistDBSerialRecord {
    cGZDBRecordPackedFile(PackedFileRecordType type,
                          cGZPersistResourceKey const &key,
                          cIGZPersistDBSegment *segment, bool a4);
    PackedFileRecordType m_Type;
};
struct cGZDBReadRecordPackedFile : public cGZDBRecordPackedFile {
    cGZDBReadRecordPackedFile(uint32_t offset, uint32_t size,
                              const cGZPersistResourceKey &crKey,
                              cIGZPersistDBSegmentPackedFile *pDBSeg, bool a6);
    uint16_t GetAccessFlags() override;
    uint32_t GetSize() override;
    void SetSize(uint32_t uint32) override;
    uint32_t GetPosition() override;
    ~cGZDBReadRecordPackedFile() override;
    void SetKey(const cGZPersistResourceKey &key) override;
    bool GetFieldVoid(void *data, uint32_t len) override;
    bool SetFieldVoid(const void *pVoid, uint32_t uint32) override;
    void Flush() override;
    bool GetError() const override;
    virtual void SeekAbsolute(uint32_t pos);
    virtual void SeekRelative(int32_t pos);

    uint32_t m_nOffset, m_nPos, m_nSize;
    cIGZPersistDBSegmentPackedFile *m_oDBSeg{NULL};
    bool field_A{false};
    void DoPostClose();
};
struct cGZDBReadRecordRAM : public cGZDBRecordPackedFile {
    cGZDBReadRecordRAM(uint8_t *buf, uint32_t size,
                       const cGZPersistResourceKey &crKey,
                       cIGZPersistDBSegmentPackedFile *pDBSeg, bool a6);
    uint16_t GetAccessFlags() override;
    uint32_t GetSize() override;
    void SetSize(uint32_t uint32) override;
    uint32_t GetPosition() override;
    ~cGZDBReadRecordRAM() override;
    void SetKey(const cGZPersistResourceKey &key) override;
    bool GetFieldVoid(void *data, uint32_t len) override;
    bool SetFieldVoid(const void *pVoid, uint32_t uint32) override;
    void Flush() override;
    bool GetError() const override;
    virtual void SeekAbsolute(uint32_t pos);
    virtual void SeekRelative(int32_t pos);
    uint8_t *m_pBuf;
    uint32_t m_nPos, m_nSize;
    cIGZPersistDBSegmentPackedFile *field_18{NULL};
    bool field_A{false};
    void DoPostClose();
};
struct cGZDBWriteRecordPackedFile : public cGZDBRecordPackedFile {
    cGZDBWriteRecordPackedFile(uint32_t a2, uint32_t a3,
                               const cGZPersistResourceKey &crKey,
                               cIGZPersistDBSegmentPackedFile *pDBSeg, bool a6);
    uint16_t GetAccessFlags() override;
    uint32_t GetSize() override;
    void SetSize(uint32_t uint32) override;
    uint32_t GetPosition() override;
    ~cGZDBWriteRecordPackedFile() override;
    void SetKey(const cGZPersistResourceKey &key) override;
    bool GetFieldVoid(void *data, uint32_t len) override;
    bool SetFieldVoid(const void *pVoid, uint32_t uint32) override;
    void Flush() override;
    bool GetError() const override;
    virtual void SeekAbsolute(uint32_t pos);
    virtual void SeekRelative(int32_t pos);
    uint32_t field_44, field_48, field_4C;
    cIGZPersistDBSegmentPackedFile *field_18{NULL};
    bool field_A{false};
    void DoPostClose();
};

struct cGZPersistDBSegmentPackedFile : public cGZPersistDBSegment,
                                       public cIGZPersistDBSegmentPackedFile {
    IUNKNOWN_COMMON_DEFS

    struct KeyTGI {
        KeyTGI(const cGZPersistResourceKey key) {
            m_nType = key.m_nTypeID;
            m_nGroup = key.m_nGroupID;
            m_nInstance = key.m_nInstanceID;
        }
        KeyTGI() = default;
        uint32_t m_nType{0}, m_nGroup{0}, m_nInstance{0};
        inline bool operator<(const KeyTGI &other) const {
            return m_nType < other.m_nType ||
                   (m_nType == other.m_nType &&
                    (m_nGroup < other.m_nGroup ||
                     (m_nGroup == other.m_nGroup &&
                      m_nInstance < other.m_nInstance)));
        }

        inline bool operator==(const KeyTGI &other) const {
            return m_nType == other.m_nType && m_nGroup == other.m_nGroup &&
                   m_nInstance == other.m_nInstance;
        }

        inline bool operator!=(const KeyTGI &other) const {
            return !(*this == other);
        }

        inline bool operator>(const KeyTGI &other) const {
            return other < *this;
        }

        inline bool operator<=(const KeyTGI &other) const {
            return !(other < *this);
        }

        inline bool operator>=(const KeyTGI &other) const {
            return !(*this < other);
        }
    };
    struct CompressedRecordData {
        KeyTGI m_oKey;
        uint32_t m_nDecompressedSize;
    };
    struct RecordDataInfo {
        KeyTGI m_oKey;
#if 0
        uint32_t m_nInstanceID2; // some DBPF 1.1 entries use this
#endif
        uint32_t m_nLocationInArchive;
        uint32_t m_nSize;
    };

    int field_18{0}, flags{0}, field_20{0};
    std::map<KeyTGI, RecordDataInfo> *m_oIndexTable{NULL};
    int field_28{0};
    cRZString field_2C{};
    cRZFile m_oFile{};
    int field_A8{0}, field_AC{-1}, field_B0{-1}, field_B4{0}, field_B8{0},
            field_BC{512};
    std::map<cGZPersistResourceKey, cGZPersistDBSerialRecord *> m_oRecords{};
    struct DBPFHeader {
        uint32_t m_nMagic;
        uint32_t m_nVersion;
        uint32_t m_nMinorVersion;   // Version: Major.Minor.
        // SimCity 4 is 1.0
        uint32_t field_C;    // SC4Devotion says this is unused, but that's
        // wrong, it's used in the code
        uint32_t field_10;   // ditto
        uint32_t field_14;   // should be always 0 in 2.0
        uint32_t m_nDateCreated;
        uint32_t m_nDateModified;
        uint32_t m_nIndexMajorVersion;
        uint32_t m_nIndexEntryCount;
        uint32_t m_nOffsetOfFirstIndexEntry;
        uint32_t m_nIndexTableSize;
        uint32_t m_nHoleEntryCount;
        uint32_t m_nHoleTableOffset;
        uint32_t m_nHoleTableSize;
        uint32_t m_nIndexMinorVersion;
        uint32_t m_nIndexTableOffset;   // DBPF 2.0 only
        uint32_t field_38;              // DBPF 2.0 only
        char m_acReserved[24];
    };
    static_assert(sizeof(DBPFHeader) == 0x60, "DBPFHeader is the wrong size");
    DBPFHeader m_oHeader{};
    uint32_t field_138{3}, field_13C{0x3E19999A};
    struct HoleRecord {
        uint32_t m_nOffset;
        uint32_t m_nSize;

        inline bool operator<(const HoleRecord &other) const {
            return m_nOffset < other.m_nOffset;
        }

        inline bool operator==(const HoleRecord &other) const {
            return m_nOffset == other.m_nOffset && m_nSize == other.m_nSize;
        }

        inline bool operator!=(const HoleRecord &other) const {
            return !(*this == other);
        }

        inline bool operator>(const HoleRecord &other) const {
            return other < *this;
        }

        inline bool operator<=(const HoleRecord &other) const {
            return !(other < *this);
        }

        inline bool operator>=(const HoleRecord &other) const {
            return !(*this < other);
        }
    };
    std::set<HoleRecord> m_oHoleRecord{};
    bool field_158{false}, field_159{false}, field_15A{false};
    std::map<uint32_t, int> field_15C{};
    std::map<uint32_t, int> field_170{};
    bool field_184{false}, field_185{true};
    std::set<uint32_t> field_188{};
    std::set<uint32_t> field_19C{};
    std::map<cGZPersistResourceKey, CompressedRecordData> field_1B0{};
    int field_1C4{0};

    inline bool Init() {
        if (!field_14) field_14 = 1;
        return true;
    }
    void SetPath(const cIGZString &path) override;
    bool Open(bool a2, bool a3) override;
    bool WriteEmptyHeaderRecord();
    bool WriteHeaderRecord();
    bool ReadHeaderRecord();
    bool VerifyHeaderRecordIntegrity();
    bool ReadIndexRecord();
    void IncrementTypeAndGroupUse(uint32_t nType, uint32_t nGroup, bool b);
    bool VerifyIndexRecordIntegrity();
    bool ReadHoleRecord();
    bool VerifyHoleRecordIntegrity();
    bool ReadCompressedSetRecord();
    size_t DoReadRecord(cGZPersistResourceKey const &key, void *buffer,
                        uint32_t &size);
    uint32_t GetDecompressedRecordLength(const cGZPersistResourceKey &key);
    bool DecompressData(uint8_t *abComprData, uint32_t ulComprLen,
                        void *&rpvUncomprData, uint32_t &rulUncomprLen);
    bool OpenRecord(cGZPersistResourceKey const &key,
                    cIGZPersistDBRecord **ppRecord, uint32_t a4);
    bool DoOpenRecord(cGZPersistResourceKey const &key,
                      cIGZPersistDBRecord **ppRecord, uint32_t a4);
    bool DoOpenRecordForWrite(cGZPersistResourceKey const &key,
                              cIGZPersistDBRecord **ppRecord);
    bool CloseRecord(cIGZPersistDBRecord *pRecord) override;
    bool ReadFileSpan(void *pBuffer, uint32_t nSize, uint32_t nOffset) override;
    bool WriteFileSpan(const void *data, uint32_t size, uint32_t pos) override;
    bool DoCreateNewRecord(const cGZPersistResourceKey &key,
                           cIGZPersistDBRecord **ppRecord) override;
    void Close();
    void Shutdown();
};

struct cISC4DBSegmentOStream : public cIGZOStream {
    virtual bool Open(cIGZPersistDBSegment *segment,
                      const cGZPersistResourceKey &key, bool a3) = 0;
    virtual bool Close() = 0;
    virtual bool IsOpen() = 0;
    virtual cIGZPersistDBRecord *GetRecord() = 0;
    virtual cIGZPersistDBSegment *GetSegment() = 0;
    virtual bool WriteGZSerializable(const cIGZSerializable *serializable) = 0;
    virtual bool WriteResKey(const cGZPersistResourceKey &key) = 0;
    virtual bool WriteVariant(cIGZVariant *variant) = 0;
};
struct cISC4DBSegmentIStream : public cIGZIStream {
    //IUNKNOWN_CLSID(0x4999A8F5);
    virtual bool Open(cIGZPersistDBSegment *segment,
                      const cGZPersistResourceKey &key, bool a3) = 0;
    virtual bool Close() = 0;
    virtual bool IsOpen() = 0;
    virtual cIGZPersistDBRecord *GetRecord() = 0;
    virtual cIGZPersistDBSegment *GetSegment() = 0;
    virtual bool ReadGZSerializable(cIGZSerializable *serializable) = 0;
    virtual bool ReadResKey(cGZPersistResourceKey &key) = 0;
    virtual bool ReadVariant(cIGZVariant *variant) = 0;
};

GZ_IUNKNOWN_ID(cISC4DBSegmentIStream, 0x4999A8F5);

struct cSC4DBSegmentOStream : public cRZUnknown, public cISC4DBSegmentOStream {

    virtual bool Open(cIGZPersistDBSegment *segment,
                      const cGZPersistResourceKey &key, bool a3);
    virtual bool Close();
    virtual bool IsOpen();
    virtual cIGZPersistDBRecord *GetRecord();
    virtual cIGZPersistDBSegment *GetSegment();
    virtual bool WriteGZSerializable(const cIGZSerializable *serializable);
    virtual bool WriteResKey(const cGZPersistResourceKey &key);
    virtual bool WriteVariant(cIGZVariant *variant);
    virtual void Flush();
    virtual bool SetSint8(int8_t value);
    virtual bool SetUint8(uint8_t value);
    virtual bool SetSint16(int16_t value);
    virtual bool SetUint16(uint16_t value);
    virtual bool SetSint32(int32_t value);
    virtual bool SetUint32(uint32_t value);
    virtual bool SetSint64(int64_t value);
    virtual bool SetUint64(uint64_t value);
    virtual bool SetFloat32(float value);
    virtual bool SetFloat64(double value);
    virtual bool SetRZCharStr(const char *value);
    virtual bool SetGZStr(const cIGZString &value);
    virtual bool SetGZSerializable(const cIGZSerializable &value);
    virtual bool SetVoid(const void *value, uint32_t size);
    virtual bool GetError();
    virtual void SetUserData(cIGZVariant *value);
    virtual cIGZVariant *GetUserData();
    virtual bool Init(bool b);
    virtual bool Shutdown();
    bool QueryInterface(uint32_t uiClsId, void **ppvObj) override;
    bool field_C{true};
    cRZAutoRefCount<cGZPersistDBSerialRecord> field_10{NULL};
    cRZAutoRefCount<cIGZPersistDBSegment> field_14{NULL};
    cRZAutoRefCount<cIGZVariant> field_18{NULL};
};
struct cSC4DBSegmentIStream : public cRZUnknown, public cISC4DBSegmentIStream {

    virtual bool Open(cIGZPersistDBSegment *segment,
                      const cGZPersistResourceKey &key, bool a3);
    virtual bool Close();
    virtual bool IsOpen();
    virtual cIGZPersistDBRecord *GetRecord();
    virtual cIGZPersistDBSegment *GetSegment();
    virtual bool ReadGZSerializable(cIGZSerializable *serializable);
    virtual bool ReadResKey(cGZPersistResourceKey &key);
    virtual bool ReadVariant(cIGZVariant *variant);
    virtual bool Flush();
    virtual bool GetSint8(int8_t &value);
    virtual bool GetUint8(uint8_t &value);
    virtual bool GetSint16(int16_t &value);
    virtual bool GetUint16(uint16_t &value);
    virtual bool GetSint32(int32_t &value);
    virtual bool GetUint32(uint32_t &value);
    virtual bool GetSint64(int64_t &value);
    virtual bool GetUint64(uint64_t &value);
    virtual bool GetFloat32(float &value);
    virtual bool GetFloat64(double &value);
    virtual bool GetRZCharStr(char *value, uint32_t size);
    virtual bool GetGZStr(cIGZString &value);
    virtual bool GetGZSerializable(cIGZSerializable &value);
    virtual bool GetVoid(void *value, uint32_t size);
    virtual uint32_t GetError();
    virtual void SetUserData(cIGZVariant *value);
    virtual cIGZVariant *GetUserData();
    virtual bool Init(bool b);
    virtual bool Shutdown();
    bool QueryInterface(uint32_t uiClsId, void **ppvObj) override;
    bool field_C{true};
    cRZAutoRefCount<cGZPersistDBSerialRecord> field_10{NULL};
    cRZAutoRefCount<cIGZPersistDBSegment> field_14{NULL};
    cRZAutoRefCount<cIGZVariant> field_18{NULL};
};


#endif //SCION_GZPERSIST_H
