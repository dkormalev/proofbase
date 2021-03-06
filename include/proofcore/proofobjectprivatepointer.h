/* Copyright 2018, OpenSoft Inc.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification, are permitted
 * provided that the following conditions are met:
 *
 *     * Redistributions of source code must retain the above copyright notice, this list of
 * conditions and the following disclaimer.
 *    * Redistributions in binary form must reproduce the above copyright notice, this list of
 * conditions and the following disclaimer in the documentation and/or other materials provided
 * with the distribution.
 *     * Neither the name of OpenSoft Inc. nor the names of its contributors may be used to endorse
 * or promote products derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND
 * FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER
 * IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
 * OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */
#ifndef PROOF_PROOFOBJECTPRIVATEPOINTER_H
#define PROOF_PROOFOBJECTPRIVATEPOINTER_H

#include "proofcore/proofcore_global.h"

#include <QScopedPointer>

namespace Proof {
class ProofObjectPrivate;

class PROOF_CORE_EXPORT ProofObjectPrivatePointer
{
public:
    ProofObjectPrivatePointer(ProofObjectPrivate *d);
    ProofObjectPrivatePointer(const ProofObjectPrivatePointer &) = delete;
    ProofObjectPrivatePointer(ProofObjectPrivatePointer &&) = default;
    ~ProofObjectPrivatePointer();

    ProofObjectPrivatePointer &operator=(const ProofObjectPrivatePointer &) = delete;
    ProofObjectPrivatePointer &operator=(ProofObjectPrivatePointer &&) = default;

    ProofObjectPrivate *data();
    const ProofObjectPrivate *data() const;

private:
    ProofObjectPrivate *m_pointer;
};

// Functions for macro Q_D. Works thanks to ADL
static inline ProofObjectPrivate *qGetPtrHelper(ProofObjectPrivatePointer &p)
{
    return p.data();
}
static inline const ProofObjectPrivate *qGetPtrHelper(const ProofObjectPrivatePointer &p)
{
    return p.data();
}
template <typename T>
static inline T *qGetPtrHelper(T *ptr)
{
    return ptr;
}

} // namespace Proof

#endif // PROOF_PROOFOBJECTPRIVATEPOINTER_H
