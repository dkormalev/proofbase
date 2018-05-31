// clazy:skip

#include <proofcore/future.h>

#include "proofnetwork/restclient.h"

#include <QNetworkReply>
#include <QRegExp>
#include <QScopedPointer>
#include <QSignalSpy>

#include "gtest/test_global.h"

#include <functional>
#include <tuple>

using testing::Test;
using testing::TestWithParam;

using HttpMethodCall =
    std::function<Proof::CancelableFuture<QNetworkReply *>(Proof::RestClient &, const QByteArray & /*body*/)>;
using HttpMethodsTestParam = std::tuple<HttpMethodCall, QString /*fileOfBody*/, QString /*contentType*/>;

class RestClientTest : public TestWithParam<HttpMethodsTestParam>
{
public:
    RestClientTest() {}

protected:
    void SetUp() override
    {
        serverRunner = new FakeServerRunner();
        serverRunner->runServer();

        restClient = Proof::RestClientSP::create();
        restClient->setAuthType(Proof::RestAuthType::NoAuth);
        restClient->setHost("127.0.0.1");
        restClient->setPort(9091);
        restClient->setScheme("http");
        restClient->setClientName("Proof-test");
    }

    void TearDown() override { delete serverRunner; }

protected:
    FakeServerRunner *serverRunner;
    Proof::RestClientSP restClient;
};

using namespace std::placeholders;

INSTANTIATE_TEST_CASE_P(
    RestClientTestInstance, RestClientTest,
    testing::Values(
        // Without vendor, without body
        HttpMethodsTestParam(std::bind(static_cast<Proof::CancelableFuture<QNetworkReply *> (Proof::RestClient::*)(
                                           const QString &, const QUrlQuery &, const QString &)>(&Proof::RestClient::get),
                                       _1, QStringLiteral("/"), QUrlQuery(), QString()),
                             "", "text/plain"),
        HttpMethodsTestParam(
            std::bind(static_cast<Proof::CancelableFuture<QNetworkReply *> (Proof::RestClient::*)(const QUrl &)>(
                          &Proof::RestClient::get),
                      _1, QUrl("http://127.0.0.1:9091/")),
            "", "text/plain"),
        HttpMethodsTestParam(std::bind(static_cast<Proof::CancelableFuture<QNetworkReply *> (Proof::RestClient::*)(
                                           const QString &, const QUrlQuery &, const QByteArray &, const QString &)>(
                                           &Proof::RestClient::post),
                                       _1, "/", QUrlQuery(), _2, QString()),
                             "", "text/plain"),
        HttpMethodsTestParam(std::bind(&Proof::RestClient::put, _1, "/", QUrlQuery(), _2, QString()), "", "text/plain"),
        HttpMethodsTestParam(std::bind(&Proof::RestClient::patch, _1, "/", QUrlQuery(), _2, QString()), "", "text/plain"),
        HttpMethodsTestParam(std::bind(&Proof::RestClient::deleteResource, _1, "/", QUrlQuery(), QString()), "",
                             "text/plain"),
        // With vendor, without body
        HttpMethodsTestParam(std::bind(static_cast<Proof::CancelableFuture<QNetworkReply *> (Proof::RestClient::*)(
                                           const QString &, const QUrlQuery &, const QString &)>(&Proof::RestClient::get),
                                       _1, QStringLiteral("/"), QUrlQuery(), "opensoft"),
                             "", "application/vnd.opensoft"),
        HttpMethodsTestParam(std::bind(static_cast<Proof::CancelableFuture<QNetworkReply *> (Proof::RestClient::*)(
                                           const QString &, const QUrlQuery &, const QByteArray &, const QString &)>(
                                           &Proof::RestClient::post),
                                       _1, "/", QUrlQuery(), _2, "opensoft"),
                             "", "application/vnd.opensoft"),
        HttpMethodsTestParam(std::bind(&Proof::RestClient::put, _1, "/", QUrlQuery(), _2, "opensoft"), "",
                             "application/vnd.opensoft"),
        HttpMethodsTestParam(std::bind(&Proof::RestClient::patch, _1, "/", QUrlQuery(), _2, "opensoft"), "",
                             "application/vnd.opensoft"),
        HttpMethodsTestParam(std::bind(&Proof::RestClient::deleteResource, _1, "/", QUrlQuery(), "opensoft"), "",
                             "application/vnd.opensoft"),
        // Without vendor, with json body
        HttpMethodsTestParam(std::bind(static_cast<Proof::CancelableFuture<QNetworkReply *> (Proof::RestClient::*)(
                                           const QString &, const QUrlQuery &, const QByteArray &, const QString &)>(
                                           &Proof::RestClient::post),
                                       _1, "/", QUrlQuery(), _2, QString()),
                             ":/data/vendor_test_body.json", "application/json"),
        HttpMethodsTestParam(std::bind(&Proof::RestClient::put, _1, "/", QUrlQuery(), _2, QString()),
                             ":/data/vendor_test_body.json", "application/json"),
        HttpMethodsTestParam(std::bind(&Proof::RestClient::patch, _1, "/", QUrlQuery(), _2, QString()),
                             ":/data/vendor_test_body.json", "application/json"),
        // Without vendor, with xml body
        HttpMethodsTestParam(std::bind(static_cast<Proof::CancelableFuture<QNetworkReply *> (Proof::RestClient::*)(
                                           const QString &, const QUrlQuery &, const QByteArray &, const QString &)>(
                                           &Proof::RestClient::post),
                                       _1, "/", QUrlQuery(), _2, QString()),
                             ":/data/vendor_test_body.xml", "text/xml"),
        HttpMethodsTestParam(std::bind(&Proof::RestClient::put, _1, "/", QUrlQuery(), _2, QString()),
                             ":/data/vendor_test_body.xml", "text/xml"),
        HttpMethodsTestParam(std::bind(&Proof::RestClient::patch, _1, "/", QUrlQuery(), _2, QString()),
                             ":/data/vendor_test_body.xml", "text/xml"),
        // With vendor, with json body
        HttpMethodsTestParam(std::bind(static_cast<Proof::CancelableFuture<QNetworkReply *> (Proof::RestClient::*)(
                                           const QString &, const QUrlQuery &, const QByteArray &, const QString &)>(
                                           &Proof::RestClient::post),
                                       _1, "/", QUrlQuery(), _2, "opensoft"),
                             ":/data/vendor_test_body.json", "application/vnd.opensoft+json"),
        HttpMethodsTestParam(std::bind(&Proof::RestClient::put, _1, "/", QUrlQuery(), _2, "opensoft"),
                             ":/data/vendor_test_body.json", "application/vnd.opensoft+json"),
        HttpMethodsTestParam(std::bind(&Proof::RestClient::patch, _1, "/", QUrlQuery(), _2, "opensoft"),
                             ":/data/vendor_test_body.json", "application/vnd.opensoft+json"),
        // With vendor, with xml body
        HttpMethodsTestParam(std::bind(static_cast<Proof::CancelableFuture<QNetworkReply *> (Proof::RestClient::*)(
                                           const QString &, const QUrlQuery &, const QByteArray &, const QString &)>(
                                           &Proof::RestClient::post),
                                       _1, "/", QUrlQuery(), _2, "opensoft"),
                             ":/data/vendor_test_body.xml", "application/vnd.opensoft+xml"),
        HttpMethodsTestParam(std::bind(&Proof::RestClient::put, _1, "/", QUrlQuery(), _2, "opensoft"),
                             ":/data/vendor_test_body.xml", "application/vnd.opensoft+xml"),
        HttpMethodsTestParam(std::bind(&Proof::RestClient::patch, _1, "/", QUrlQuery(), _2, "opensoft"),
                             ":/data/vendor_test_body.xml", "application/vnd.opensoft+xml")));

TEST_P(RestClientTest, vendorTest)
{
    const auto methodCall = std::get<0>(GetParam());
    const auto file = std::get<1>(GetParam());
    const auto expected = std::get<2>(GetParam());
    const QRegExp expectedRegExp(QString("Content-Type:(\\s*)([^\r\n]*)\\r\\n"));

    ASSERT_TRUE(serverRunner->serverIsRunning());

    const QByteArray body = file.isEmpty() ? QByteArray() : dataFromFile(file);

    QScopedPointer<QNetworkReply> reply(methodCall(*restClient, body)->result());

    ASSERT_NE(nullptr, reply);

    QSignalSpy spy(reply.data(), &QNetworkReply::finished);

    ASSERT_TRUE(spy.wait());
    EXPECT_EQ(1, spy.count());

    const auto query = QString::fromLatin1(serverRunner->lastQuery());
    const int position = expectedRegExp.indexIn(query);

    ASSERT_NE(-1, position);
    EXPECT_EQ(expected, expectedRegExp.cap(2));
}
