#ifndef LOGS_H
#define LOGS_H

#include "proofcore/proofcore_global.h"

#include <QString>

namespace Proof {
namespace Logs {
PROOF_CORE_EXPORT void setup(const QStringList &defaultLoggingRules = QStringList());
PROOF_CORE_EXPORT void setLogsStoragePath(QString storagePath = QString());
PROOF_CORE_EXPORT void setRulesFromString(const QString &rulesString);
PROOF_CORE_EXPORT void setConsoleOutputEnabled(bool enabled);
PROOF_CORE_EXPORT void installFileHandler(const QString &fileName = QString());
PROOF_CORE_EXPORT void uninstallFileHandler();
} // namespace Logs
} // namespace Proof

#endif // LOGS_H
