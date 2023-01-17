//
// Created by chris on 1/17/2023.
//

#ifndef SCION_CSC4AUTOLOADRECORD_H
#define SCION_CSC4AUTOLOADRECORD_H

#include "GZPersist.h"

struct cSC4AutoLoadRecord {
    cGZPersistDBSegmentPackedFile *m_pDBPF{nullptr};
    cGZPersistDBSerialRecord *m_pRecord{nullptr};
    uint16_t field_C{0xFFFF};

    void openRecord(cGZPersistDBSegmentPackedFile *dbpf, uint32_t a3,
                    uint32_t a4, uint32_t a5);

    cSC4AutoLoadRecord(cGZPersistDBSegmentPackedFile *dbpf, uint32_t a3,
                       uint32_t a4, uint32_t a5);

    void Close();
};

#endif //SCION_CSC4AUTOLOADRECORD_H
