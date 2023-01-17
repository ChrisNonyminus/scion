//
// Created by chris on 1/17/2023.
//

#include "cSC4AutoLoadRecord.h"

void cSC4AutoLoadRecord::openRecord(cGZPersistDBSegmentPackedFile *dbpf,
                                    uint32_t a3, uint32_t a4, uint32_t a5) {
    if (!dbpf) return;
    m_pDBPF = dbpf;
    m_pDBPF->AddRef();
    cGZPersistResourceKey key(a3, a4, a5);
    cIGZPersistDBRecord *record;
    if (!m_pDBPF->OpenRecord(key, &record, 1)) return;
    m_pRecord = (cGZPersistDBSerialRecord *)record;
    if (m_pRecord) {
        if (!m_pRecord->GetFieldUint16(field_C)) {
            m_pRecord->Release();
            m_pDBPF->CloseRecord(m_pRecord);
            m_pRecord = nullptr;
            m_pDBPF->Release();
            m_pDBPF = nullptr;
        }
    }
}
cSC4AutoLoadRecord::cSC4AutoLoadRecord(
        cGZPersistDBSegmentPackedFile *dbpf, uint32_t a3, uint32_t a4,
        uint32_t a5) {
    openRecord(dbpf, a3, a4, a5);
}
void cSC4AutoLoadRecord::Close() {
    if (m_pRecord) {
        m_pDBPF->CloseRecord(m_pRecord);
        m_pRecord->Release();
        m_pRecord = nullptr;
    }
    if (m_pDBPF) {
        m_pDBPF->Release();
        m_pDBPF = nullptr;
    }
}