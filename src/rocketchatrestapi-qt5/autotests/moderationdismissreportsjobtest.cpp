/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "moderationdismissreportsjobtest.h"
#include "moderation/moderationuserdeletereportedmessagesjob.h"
#include "ruqola_restapi_helper.h"
#include <QJsonDocument>
#include <QTest>
QTEST_GUILESS_MAIN(ModerationDismissReportsJobTest)
using namespace RocketChatRestApi;
ModerationDismissReportsJobTest::ModerationDismissReportsJobTest(QObject *parent)
    : QObject(parent)
{
}

void ModerationDismissReportsJobTest::shouldHaveDefaultValue()
{
    ModerationUserDeleteReportedMessagesJob job;
    verifyDefaultValue(&job);
    QVERIFY(job.requireHttpAuthentication());
    QVERIFY(job.userIdForMessages().isEmpty());
    QVERIFY(!job.hasQueryParameterSupport());
}

void ModerationDismissReportsJobTest::shouldGenerateRequest()
{
    ModerationUserDeleteReportedMessagesJob job;
    QNetworkRequest request = QNetworkRequest(QUrl());
    verifyAuthentication(&job, request);
    QCOMPARE(request.url(), QUrl(QStringLiteral("http://www.kde.org/api/v1/teams.convertToChannel")));
    QCOMPARE(request.header(QNetworkRequest::ContentTypeHeader).toString(), QStringLiteral("application/json"));
}

void ModerationDismissReportsJobTest::shouldGenerateJson()
{
    ModerationUserDeleteReportedMessagesJob job;
    const QString userIdForMessages = QStringLiteral("foo2");
    job.setUserIdForMessages(userIdForMessages);

    QCOMPARE(job.json().toJson(QJsonDocument::Compact), QStringLiteral(R"({"userId":"%1"})").arg(userIdForMessages).toLatin1());
}

void ModerationDismissReportsJobTest::shouldNotStarting()
{
    ModerationUserDeleteReportedMessagesJob job;

    RestApiMethod method;
    method.setServerUrl(QStringLiteral("http://www.kde.org"));
    job.setRestApiMethod(&method);

    QNetworkAccessManager mNetworkAccessManager;
    job.setNetworkAccessManager(&mNetworkAccessManager);
    QVERIFY(!job.canStart());
    const QString auth = QStringLiteral("foo");
    const QString userId = QStringLiteral("foo");
    job.setAuthToken(auth);
    QVERIFY(!job.canStart());
    job.setUserId(userId);
    QVERIFY(!job.canStart());
    const QString userIdForMessages = QStringLiteral("foo1");
    job.setUserIdForMessages(userIdForMessages);
    QVERIFY(job.canStart());
}

#include "moc_moderationdismissreportsjobtest.cpp"
