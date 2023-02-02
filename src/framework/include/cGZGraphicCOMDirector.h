//
// Created by chris on 1/18/2023.
//

#ifndef SCION_CGZGRAPHICCOMDIRECTOR_H
#define SCION_CGZGRAPHICCOMDIRECTOR_H

#include "cRZCOMDllDirector.h"

struct cGZGraphicCOMDirector : public cRZCOMDllDirector {
    cGZGraphicCOMDirector();
    virtual uint32_t GetDirectorID() const;
    ~cGZGraphicCOMDirector() override = default;

    bool OnStart(cIGZCOM *com) override;
};

#endif //SCION_CGZGRAPHICCOMDIRECTOR_H
