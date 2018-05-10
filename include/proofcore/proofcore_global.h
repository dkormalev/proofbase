#ifndef PROOFCORE_GLOBAL_H
#define PROOFCORE_GLOBAL_H

#ifdef PROOF_CORE_LIB
#  define PROOF_CORE_EXPORT Q_DECL_EXPORT
#else
#  define PROOF_CORE_EXPORT Q_DECL_IMPORT
#endif

#include <QLoggingCategory>

Q_DECLARE_LOGGING_CATEGORY(proofCoreSettingsLog)
Q_DECLARE_LOGGING_CATEGORY(proofCoreLoggerLog)
Q_DECLARE_LOGGING_CATEGORY(proofCoreFuturesLog)
Q_DECLARE_LOGGING_CATEGORY(proofCoreCrashLog)
Q_DECLARE_LOGGING_CATEGORY(proofCoreUpdatesLog)
Q_DECLARE_LOGGING_CATEGORY(proofCoreMiscLog)

PROOF_CORE_EXPORT Q_DECLARE_LOGGING_CATEGORY(proofCoreTaskChainExtraLog)
PROOF_CORE_EXPORT Q_DECLARE_LOGGING_CATEGORY(proofCoreTaskChainStatsLog)
PROOF_CORE_EXPORT Q_DECLARE_LOGGING_CATEGORY(proofCoreTasksExtraLog)
PROOF_CORE_EXPORT Q_DECLARE_LOGGING_CATEGORY(proofCoreTasksStatsLog)
PROOF_CORE_EXPORT Q_DECLARE_LOGGING_CATEGORY(proofCoreCacheLog)

namespace Proof {
constexpr long CORE_MODULE_CODE = 100;
}
#endif // PROOFCORE_GLOBAL_H
