#ifndef EXPIRATOR_H
#define EXPIRATOR_H

#include "proofcore/proofcore_global.h"
#include "proofcore/proofobject.h"

#include <QDateTime>

namespace Proof {

class ExpiratorPrivate;
class PROOF_CORE_EXPORT Expirator : public ProofObject // clazy:exclude=ctor-missing-parent-argument
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(Expirator)
public:
    static Expirator *instance();
    void addObject(const QSharedPointer<ProofObject> &object, const QDateTime &expirationTime);

protected:
    void timerEvent(QTimerEvent *ev) override;

private:
    Expirator();
    ~Expirator();
    Expirator(const Expirator &other) = delete;
    Expirator &operator=(const Expirator &other) = delete;
    Expirator(const Expirator &&other) = delete;
    Expirator &operator=(const Expirator &&other) = delete;
};
} // namespace Proof

#endif // EXPIRATOR_H
