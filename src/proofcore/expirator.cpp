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
#include "proofcore/expirator.h"

#include "proofcore/proofobject_p.h"

#include <QMultiMap>
#include <QMutex>
#include <QSet>
#include <QThread>
#include <QTimer>
#include <QTimerEvent>

namespace Proof {
class ExpiratorPrivate : public ProofObjectPrivate
{
    Q_DECLARE_PUBLIC(Expirator)
    QMultiMap<QDateTime, QSharedPointer<ProofObject>> m_controlledObjects;
    QMutex *m_mutex;
    QThread *m_thread;
    int m_timerId = 0;
};
} // namespace Proof

using namespace Proof;

Expirator::Expirator() : ProofObject(*new ExpiratorPrivate)
{
    Q_D(Expirator);
    d->m_mutex = new QMutex();
    d->m_thread = new QThread(this);
    moveToThread(d->m_thread);
    d->m_thread->start();

    QTimer *timer = new QTimer();
    connect(timer, &QTimer::timeout, this,
            [this, timer]() {
                Q_D(Expirator);
                d->m_timerId = startTimer(1000 * 60 * 10, Qt::VeryCoarseTimer);
                qCDebug(proofCoreCacheLog) << "Cache expirator timer started";
                timer->deleteLater();
            },
            Qt::QueuedConnection);
    timer->setSingleShot(true);
    timer->start();
}

Expirator::~Expirator()
{
    Q_D(Expirator);
    d->m_thread->quit();
    d->m_thread->wait(500);
    d->m_thread->terminate();
}

Expirator *Expirator::instance()
{
    static Expirator *inst = nullptr;
    if (!inst) {
        inst = new Expirator;
        qCDebug(proofCoreCacheLog) << "Cache expirator instantiated";
    }
    return inst;
}

void Expirator::addObject(const QSharedPointer<ProofObject> &object, const QDateTime &expirationTime)
{
    Q_D(Expirator);
    d->m_mutex->lock();
    d->m_controlledObjects.insertMulti(expirationTime, object);
    d->m_mutex->unlock();
}

void Expirator::timerEvent(QTimerEvent *ev)
{
    Q_D(Expirator);
    if (ev->timerId() != d->m_timerId)
        return;

    d->m_mutex->lock();
    QSet<QDateTime> toRemove;
    for (auto it = d->m_controlledObjects.cbegin(); it != d->m_controlledObjects.cend(); ++it) {
        if (it.key() > QDateTime::currentDateTime())
            break;
        toRemove << it.key();
    }
    qCDebug(proofCoreCacheLog) << "Cache expirator removing" << toRemove.count() << "objects";
    for (const QDateTime &time : qAsConst(toRemove))
        d->m_controlledObjects.remove(time);
    d->m_mutex->unlock();
}
