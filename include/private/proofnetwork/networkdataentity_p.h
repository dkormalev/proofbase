#ifndef NETWORKDATAENTITY_P_H
#define NETWORKDATAENTITY_P_H

#include "proofcore/objectscache.h"
#include "proofcore/proofobject_p.h"
#include "proofcore/spinlock.h"

#include "proofnetwork/networkdataentity.h"
#include "proofnetwork/proofnetwork_global.h"
#include "proofnetwork/proofnetwork_types.h"

#include <QtGlobal>

#include <functional>

namespace Proof {
class PROOF_NETWORK_EXPORT NetworkDataEntityPrivate : public ProofObjectPrivate
{
    Q_DECLARE_PUBLIC(NetworkDataEntity)
public:
    NetworkDataEntityPrivate() : ProofObjectPrivate() {}

    template <typename Q, typename Entity, typename Key, typename KeyFunc, typename NotifySignal, typename Creator>
    auto updateEntityField(QWeakPointer<Entity> &storedEntity, Key &storedKey, const Key &key,
                           ObjectsCache<Key, Entity> &cache, KeyFunc &&keyFunc, Q *notifier,
                           NotifySignal &&notifySignal, Creator &&creator)
        -> decltype(std::function<Key(Entity *)>(keyFunc)(cache.value(Key()).data()) == Key(),
                    std::function<void(Q *, QSharedPointer<Entity>)>(notifySignal)(notifier, storedEntity),
                    cache.add(key, creator()), QSharedPointer<Entity>())
    {
        auto castedKeyFunc = std::function<Key(Entity *)>(std::forward<KeyFunc>(keyFunc));
        auto castedSignal = std::function<void(Q *, QSharedPointer<Entity>)>(std::forward<NotifySignal>(notifySignal));
        QSharedPointer<Entity> result = storedEntity.toStrongRef();
        Key oldKey = result ? castedKeyFunc(result.data()) : storedKey;
        if (oldKey != key || !Entity::isValidAndDirty(result)) {
            result = cache.add(key, std::forward<Creator>(creator));
            storedEntity = result.toWeakRef();
            storedKey = castedKeyFunc(result.data());
            if (oldKey != key)
                emit castedSignal(notifier, result);
        }
        return result;
    }

    template <typename Q, typename Entity, typename Key, typename KeyFunc, typename NotifySignal>
    auto updateEntityField(QWeakPointer<Entity> &storedEntity, Key &storedKey, const Key &key,
                           ObjectsCache<Key, Entity> &cache, KeyFunc &&keyFunc, Q *notifier, NotifySignal &&notifySignal)
        -> decltype(std::function<Key(Entity *)>(keyFunc)(cache.value(Key()).data()) == Key(),
                    std::function<void(Q *, QSharedPointer<Entity>)>(notifySignal)(notifier, storedEntity),
                    QSharedPointer<Entity>())
    {
        return updateEntityField(storedEntity, storedKey, key, cache, std::forward<KeyFunc>(keyFunc), notifier,
                                 std::forward<NotifySignal>(notifySignal), std::bind(&Entity::create, key));
    }

    template <typename Q, typename Entity, typename Key, typename KeyFunc, typename NotifySignal, typename Creator>
    auto updateEntityField(QSharedPointer<Entity> &storedEntity, const Key &key, ObjectsCache<Key, Entity> &cache,
                           KeyFunc &&keyFunc, Q *notifier, NotifySignal &&notifySignal, Creator &&creator)
        -> decltype(std::function<Key(Entity *)>(keyFunc)(cache.value(Key()).data()) == Key(),
                    std::function<void(Q *, QSharedPointer<Entity>)>(notifySignal)(notifier, storedEntity),
                    cache.add(key, creator()), QSharedPointer<Entity>())
    {
        auto castedKeyFunc = std::function<Key(Entity *)>(std::forward<KeyFunc>(keyFunc));
        auto castedSignal = std::function<void(Q *, QSharedPointer<Entity>)>(std::forward<NotifySignal>(notifySignal));
        if (!Entity::isValidAndDirty(storedEntity) || castedKeyFunc(storedEntity.data()) != key) {
            QSharedPointer<Entity> newEntity = cache.add(key, std::forward<Creator>(creator));
            storedEntity = newEntity;
            emit castedSignal(notifier, storedEntity);
        }
        return storedEntity;
    }

    template <typename Q, typename Entity, typename Key, typename KeyFunc, typename NotifySignal>
    auto updateEntityField(QSharedPointer<Entity> &storedEntity, const Key &key, ObjectsCache<Key, Entity> &cache,
                           KeyFunc &&keyFunc, Q *notifier, NotifySignal &&notifySignal)
        -> decltype(std::function<Key(Entity *)>(keyFunc)(cache.value(Key()).data()) == Key(),
                    std::function<void(Q *, QSharedPointer<Entity>)>(notifySignal)(notifier, storedEntity),
                    QSharedPointer<Entity>())
    {
        return updateEntityField(storedEntity, key, cache, std::forward<KeyFunc>(keyFunc), notifier,
                                 std::forward<NotifySignal>(notifySignal), std::bind(&Entity::create, key));
    }

    virtual void updateFrom(const Proof::NetworkDataEntitySP &other);

    bool isFetched = false;
    mutable NetworkDataEntityWP weakSelf;
    mutable SpinLock spinLock;
};
} // namespace Proof

#endif // NETWORKDATAENTITY_P_H
