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
#ifndef PROOF_HTTPPARSER_P_H
#define PROOF_HTTPPARSER_P_H

#include <QByteArray>
#include <QStringList>

namespace Proof {

class HttpParser
{
public:
    enum class Result
    {
        NeedMore,
        Error,
        Success
    };

    HttpParser();
    Result parseNextPart(QByteArray data);

    QString method() const;
    QString uri() const;
    QStringList headers() const;
    QByteArray body() const;

    QString error() const;

private:
    Result initialState(QByteArray &data);
    Result headersState(QByteArray &data);
    Result bodyState(QByteArray &data);

private:
    using State = Result (HttpParser::*)(QByteArray &);

    State m_state = &HttpParser::initialState;
    QByteArray m_data;
    qulonglong m_contentLength = 0;
    QString m_method;
    QString m_uri;
    QStringList m_headers;
    QString m_error;
};

} // namespace Proof

#endif // PROOF_HTTPPARSER_P_H
